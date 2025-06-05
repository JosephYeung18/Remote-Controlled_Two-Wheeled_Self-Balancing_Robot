################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/pid/pid.c 

OBJS += \
./Core/pid/pid.o 

C_DEPS += \
./Core/pid/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/pid/%.o Core/pid/%.su Core/pid/%.cyclo: ../Core/pid/%.c Core/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Li_Zixu/Desktop/car/Core/oled" -I"C:/Users/Li_Zixu/Desktop/car/Core/mpu6500" -I"C:/Users/Li_Zixu/Desktop/car/Core/motor" -I"C:/Users/Li_Zixu/Desktop/car/Core/encoder" -I"C:/Users/Li_Zixu/Desktop/car/Core/pid" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Li_Zixu/Desktop/car/Core/sr04" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-pid

clean-Core-2f-pid:
	-$(RM) ./Core/pid/pid.cyclo ./Core/pid/pid.d ./Core/pid/pid.o ./Core/pid/pid.su

.PHONY: clean-Core-2f-pid

