#include "motor.h"


extern TIM_HandleTypeDef htim1;
/**
 * @brief 控制两个电机的转速和方向
 * @param motor1 电机1的占空比，范围-8400~8400（正数正转，负数反转）
 * @param motor2 电机2的占空比，范围-8400~8400（正数正转，负数反转）
 */
void Load(int motor1, int motor2) {
//    // 限制输入范围
    motor1 = (motor1 > 8400) ? 8400 : (motor1 < -8400) ? -8400 : motor1;
    motor2 = (motor2 > 8400) ? 8400 : (motor2 < -8400) ? -8400 : motor2;

    // 控制电机1
    if (motor1 == 0) {
        // 停止电机1
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    } else if (motor1 < 0) {
        // 电机1正转
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, -motor1);
    } else {
        // 电机1反转
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, motor1);
    }

    // 控制电机2
    if (motor2 == 0) {
        // 停止电机2
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    } else if (motor2 < 0) {
        // 电机2正转
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, -motor2);
    } else {
        // 电机2反转
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, motor2);
    }
}
