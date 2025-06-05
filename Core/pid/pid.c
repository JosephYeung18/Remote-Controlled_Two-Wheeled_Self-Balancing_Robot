#include <mpu6500.h>
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "stdio.h"
#include "oled.h"
#include "stdlib.h"

// 机器人状态机
typedef enum {
    STATE_NORMAL,       // 正常行驶
    STATE_APPROACHING,  // 接近障碍物
    STATE_MAINTAIN_DISTANCE, // 保持安全距离
    STATE_MANUAL_BACKING // 用户手动后退
} RobotState;

// 定义布尔类型
typedef enum { false, true } bool;


RobotState currentState = STATE_NORMAL;
float targetDistance = 50.0;  // 目标安全距离(cm)
float minApproachDistance = 25.0;  // 最小接近距离(cm)
float emergencyDistance = 10.0;  // 紧急距离(cm)
bool userForwardBlocked = false;

// 统一加减速参数
float acceleration = 0.01;  // 加速度/减速度(前进/后退相同)


//传感器数据变量
extern int Encoder_Left,Encoder_Right;
extern float roll,pitch,yaw;
extern short gyrox,gyroy,gyroz;
extern short aacx,aacy,aacz;
extern int mode;

extern int motor_en;
int motor_flag;		//小车倾斜标志位
extern uint8_t is_bluetooth_connected;  //蓝牙连接标志位

extern float distance;

int Vertical_out,Velocity_out,Turn_out,Target_turn,MOTO1,MOTO2;  //闭环控制中间变量
float Target_Speed;

float Med_Angle = 5;//平衡时角度值偏移量（机械中值）

//pid参数
extern float Vertical_Kp,Vertical_Kd;
extern float Velocity_Kp,Velocity_Ki;
extern float Turn_Kp,Turn_Kd;

extern TIM_HandleTypeDef htim2,htim4;

uint8_t stop;	//停止标志位
extern uint8_t Fore,Back,Left,Right;//前进，后退，左转，右转标志位

int Err_LowOut_last,Encoder_S;	//低通滤波片后的误差，速度积分

#define SPEED_Y 5 //俯仰(前后)最大设定速度
#define SPEED_Z 150//偏航(左右)最大设定速度


//直立环PD控制器
//输入：期望角度、真实角度、角速度
int Vertical(float Med,float Angle,float gyro_Y)
{
	int temp;
	temp=Vertical_Kp*(Angle-Med)+Vertical_Kd*gyro_Y;
	return temp;
}

//速度环PI控制器
//输入：期望速度、左编码器、右编码器
int Velocity(float Target,int encoder_L,int encoder_R)
{

	static float a = 0.7;        // 低通滤波系数（0~1，越大滤波越平滑但滞后越大）
	int Err, Err_LowOut, temp;
	Velocity_Ki = Velocity_Kp / 200;  // 积分系数Ki由比例系数Kp动态计算（Ki = Kp/200）

	// 1、计算速度偏差值（实际速度-目标速度）
	Err = (encoder_L + encoder_R) - Target;
	//   encoder_L/R：左右轮编码器值（反映实际速度）
	//   Target：目标速度设定值

	// 2、低通滤波（抑制高频噪声）
	Err_LowOut = (1 - a) * Err + a * Err_LowOut_last;  // 一阶惯性滤波公式
	Err_LowOut_last = Err_LowOut;  // 保存当前滤波值供下次使用

	// 3、积分环节（消除稳态误差）
	Encoder_S += Err_LowOut;  // 累积滤波后的偏差值

	// 4、积分限幅（防止积分饱和）
	Encoder_S = Encoder_S > 10000 ? 10000 : (Encoder_S < (-10000) ? (-10000) : Encoder_S);
	if (stop == 1) Encoder_S = 0, stop = 0;  // 停止信号触发时重置积分项

	// 5、速度环PID计算（仅使用PD+积分，此处省略微分）
	temp = Velocity_Kp * Err_LowOut + Velocity_Ki * Encoder_S;  // 比例项 + 积分项
	return temp;
}


//转向环PD控制器
//输入：角速度、角度值
int Turn(float gyro_Z,int Target_turn)
{
	int temp;
	temp=Turn_Kp*Target_turn+Turn_Kd*gyro_Z;
	return temp;
}

void Control(void)
{
	int PWM_out;
	//1、读取编码器和陀螺仪的数据
	Encoder_Right=Read_Speed(&htim4);
	Encoder_Left=Read_Speed(&htim2);
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);


	// 遥控逻辑
if(is_bluetooth_connected == 1 && mode == 1){
	if(Fore == 1 && Back == 0) {  // 用户请求前进
	    if(distance > targetDistance) {
	        // 距离大于安全距离，允许正常加速前进
	        currentState = STATE_NORMAL;
	        Target_Speed += acceleration;  // 与手动减速对称
	        userForwardBlocked = false;
	    }
	    else if(distance > minApproachDistance) {
	        // 接近障碍物，允许缓慢前进
	        currentState = STATE_APPROACHING;
	        Target_Speed += acceleration / 2;  // 半速前进
	        userForwardBlocked = false;
	    }
	    else {
	        // 距离过近，阻止前进并开始后退
	        currentState = STATE_MAINTAIN_DISTANCE;
	        userForwardBlocked = true;
	    }
	}
	else if(Back == 1 && Fore == 0) {  // 用户请求后退
	    currentState = STATE_MANUAL_BACKING;
	    Target_Speed -= acceleration;  // 与前进加速度对称
	    userForwardBlocked = false;
	}
	else {  // 无操作，平滑减速
	    if(Target_Speed > 0) {
	        Target_Speed -= acceleration;  // 正向减速与加速对称
	        if(Target_Speed < 0) Target_Speed = 0;
	    }
	    else if(Target_Speed < 0) {
	        Target_Speed += acceleration;  // 负向减速与加速对称
	        if(Target_Speed > 0) Target_Speed = 0;
	    }
	}

	// 障碍物自动响应逻辑（独立于用户输入）
	if(distance < emergencyDistance) {
	    // 紧急情况：距离过近，快速后退
	    currentState = STATE_MAINTAIN_DISTANCE;
	    Target_Speed -= acceleration * 2;  // 双倍减速度
	    userForwardBlocked = true;
	}
	else if(distance < targetDistance) {
	    // 距离小于安全距离，自动调整以保持距离
	    currentState = STATE_MAINTAIN_DISTANCE;

	    // 计算与目标距离的差值，动态调整后退速度
	    if(Target_Speed > -0.03) {  // 限制最大后退速度
	        Target_Speed -= acceleration * (1 + (targetDistance - distance)/30);
	    }

	    // 确保用户无法在距离过近时前进
	    userForwardBlocked = (distance < minApproachDistance);
	}
	else if(currentState == STATE_MAINTAIN_DISTANCE && distance > targetDistance + 5) {
	    // 已远离障碍物，恢复正常状态
	    currentState = STATE_NORMAL;
	}

	// 速度限幅
	Target_Speed = Target_Speed > SPEED_Y ? SPEED_Y : (Target_Speed < -SPEED_Y ? -SPEED_Y : Target_Speed);

		/*左右*/
		if((Left==0)&&(Right==0))Target_turn=0;
		if(Left==1)Target_turn+=30;	//左转
		if(Right==1)Target_turn-=30;	//右转
		Target_turn=Target_turn>SPEED_Z?SPEED_Z:(Target_turn<-SPEED_Z?(-SPEED_Z):Target_turn);//限幅( (20*100) * 100   )

		/*转向约束*/
		if((Left==0)&&(Right==0))Turn_Kd=-4;//若无左右转向指令，则开启转向约束
		else if((Left==1)||(Right==1))Turn_Kd=0;//若左右转向指令接收到，则去掉转向约束
}
	  if(roll<-45 || roll>45){         //小车可能已经倒了
		  motor_flag = 0;               //关闭电机
	  	  Encoder_S = 0;				//速度积分清零
	  }else
		  motor_flag=1;              //开启电机
	//2、将数据传入PID控制器，计算输出结果，即左右电机转速值
	if(motor_en && motor_flag){
		  Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);
		  Vertical_out=Vertical(Velocity_out+Med_Angle,roll,gyrox);
		  Turn_out=Turn(gyroz,Target_turn);
		  PWM_out=Vertical_out;
		  MOTO1=PWM_out-Turn_out;
		  MOTO2=PWM_out+Turn_out;
		  Load(MOTO1,MOTO2);
	}

	else{
		Load(0, 0);
	}
}


