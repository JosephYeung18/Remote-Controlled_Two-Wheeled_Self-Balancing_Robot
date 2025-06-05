################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mpu6500/IIC.c \
../Core/mpu6500/inv_mpu.c \
../Core/mpu6500/inv_mpu_dmp_motion_driver.c \
../Core/mpu6500/mpu6500.c 

OBJS += \
./Core/mpu6500/IIC.o \
./Core/mpu6500/inv_mpu.o \
./Core/mpu6500/inv_mpu_dmp_motion_driver.o \
./Core/mpu6500/mpu6500.o 

C_DEPS += \
./Core/mpu6500/IIC.d \
./Core/mpu6500/inv_mpu.d \
./Core/mpu6500/inv_mpu_dmp_motion_driver.d \
./Core/mpu6500/mpu6500.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mpu6500/%.o Core/mpu6500/%.su Core/mpu6500/%.cyclo: ../Core/mpu6500/%.c Core/mpu6500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Li_Zixu/Desktop/car/Core/oled" -I"C:/Users/Li_Zixu/Desktop/car/Core/mpu6500" -I"C:/Users/Li_Zixu/Desktop/car/Core/motor" -I"C:/Users/Li_Zixu/Desktop/car/Core/encoder" -I"C:/Users/Li_Zixu/Desktop/car/Core/pid" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Li_Zixu/Desktop/car/Core/sr04" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-mpu6500

clean-Core-2f-mpu6500:
	-$(RM) ./Core/mpu6500/IIC.cyclo ./Core/mpu6500/IIC.d ./Core/mpu6500/IIC.o ./Core/mpu6500/IIC.su ./Core/mpu6500/inv_mpu.cyclo ./Core/mpu6500/inv_mpu.d ./Core/mpu6500/inv_mpu.o ./Core/mpu6500/inv_mpu.su ./Core/mpu6500/inv_mpu_dmp_motion_driver.cyclo ./Core/mpu6500/inv_mpu_dmp_motion_driver.d ./Core/mpu6500/inv_mpu_dmp_motion_driver.o ./Core/mpu6500/inv_mpu_dmp_motion_driver.su ./Core/mpu6500/mpu6500.cyclo ./Core/mpu6500/mpu6500.d ./Core/mpu6500/mpu6500.o ./Core/mpu6500/mpu6500.su

.PHONY: clean-Core-2f-mpu6500

