/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "sr04.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint8_t display_buf[30];  //显示缓冲区，存放字符串，写入show_mem

int mode;				//控制模式（mode=1 遥控模式，mode=0;自平衡模式)
extern int stop_key;	//紧急停止按键，控制函数失效，小车制动

extern uint8_t show_mem[OLED_WIDE][OLED_LENGTH];	//显示缓冲区，存放发送到屏幕寄存器的数据

extern int Encoder_Left,Encoder_Right;	//左右编码器速度

extern float distance;		//超声波测距距离
extern uint8_t rx_buf[2];	//蓝牙接收缓冲区

extern float pitch,roll,yaw; //欧拉角

int adc;

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim5;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTaskOled */
osThreadId_t myTaskOledHandle;
const osThreadAttr_t myTaskOled_attributes = {
  .name = "myTaskOled",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTaskCtrl */
osThreadId_t myTaskCtrlHandle;
const osThreadAttr_t myTaskCtrl_attributes = {
  .name = "myTaskCtrl",
  .stack_size = 1500 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskOled(void *argument);
void StartTaskCtrl(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTaskOled */
  myTaskOledHandle = osThreadNew(StartTaskOled, NULL, &myTaskOled_attributes);

  /* creation of myTaskCtrl */
  myTaskCtrlHandle = osThreadNew(StartTaskCtrl, NULL, &myTaskCtrl_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  /* 获取超声波测距数据 */
	 GET_Distance();
	 /* 启动ADC1转换，用于读取模式选择电位器值 */
	  HAL_ADC_Start(&hadc1);
	  /* 轮询ADC转换结果（超时100ms） */
	  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
		  adc = HAL_ADC_GetValue(&hadc1);
		  /* 根据ADC值切换控制模式（阈值2000） */
		  if (adc < 2000) {
			  mode = 0; // 自平衡模式
		  } else {
			  mode = 1; // 遥控模式
		  }
	  }
	 vTaskDelay(pdMS_TO_TICKS(10));
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskOled */
/**
* @brief Function implementing the myTaskOled thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskOled */
void StartTaskOled(void *argument)
{
  /* USER CODE BEGIN StartTaskOled */
  /* Infinite loop */
  for(;;)
  {

	  /* 显示左右编码器计数值（电机转速反馈） */
	  sprintf((char *)display_buf, "L:%d  R:%d   ", Encoder_Left, Encoder_Right);
	  OLED_ShowString(0, 0, display_buf, 16, 0);
	  /* 显示姿态角：横滚角(roll) */
	  sprintf((char *)display_buf, "roll:%.1f   ", roll);
	  OLED_ShowString(0, 2, display_buf, 16, 0);
	  /* 显示姿态角：偏航角(yaw) */
	  sprintf((char *)display_buf, "yaw:%.1f   ", yaw);
	  OLED_ShowString(0, 4, display_buf, 16, 0);
	  /* 显示超声波测量距离 */
	  sprintf((char *)display_buf, "dist:%.1f   ", distance);
	  OLED_ShowString(0, 6, display_buf, 16, 0);
	  /* 更新OLED显示内容 */
	  OLED_Refresh();
	  /* 任务延时50ms，控制刷新频率 */
	  vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END StartTaskOled */
}

/* USER CODE BEGIN Header_StartTaskCtrl */
/**
* @brief Function implementing the myTaskCtrl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskCtrl */
void StartTaskCtrl(void *argument)
{
  /* USER CODE BEGIN StartTaskCtrl */
  /* Infinite loop */
  for(;;)
  {
	  if(stop_key == 0){
		  Control();
	  }else{
		  Load(0, 0);
	  }
	  vTaskDelay(pdMS_TO_TICKS(1));//延时1ms，控制函数执行频率1KHZ
  }

  /* USER CODE END StartTaskCtrl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

