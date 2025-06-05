################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/motor/motor.c 

OBJS += \
./Core/motor/motor.o 

C_DEPS += \
./Core/motor/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/motor/%.o Core/motor/%.su Core/motor/%.cyclo: ../Core/motor/%.c Core/motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Li_Zixu/Desktop/car/Core/oled" -I"C:/Users/Li_Zixu/Desktop/car/Core/mpu6500" -I"C:/Users/Li_Zixu/Desktop/car/Core/motor" -I"C:/Users/Li_Zixu/Desktop/car/Core/encoder" -I"C:/Users/Li_Zixu/Desktop/car/Core/pid" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Li_Zixu/Desktop/car/Core/sr04" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-motor

clean-Core-2f-motor:
	-$(RM) ./Core/motor/motor.cyclo ./Core/motor/motor.d ./Core/motor/motor.o ./Core/motor/motor.su

.PHONY: clean-Core-2f-motor

