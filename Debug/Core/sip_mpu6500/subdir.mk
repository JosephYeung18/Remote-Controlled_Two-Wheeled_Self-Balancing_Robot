################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/sip_mpu6500/mpu6500_spi.c 

OBJS += \
./Core/sip_mpu6500/mpu6500_spi.o 

C_DEPS += \
./Core/sip_mpu6500/mpu6500_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/sip_mpu6500/%.o Core/sip_mpu6500/%.su Core/sip_mpu6500/%.cyclo: ../Core/sip_mpu6500/%.c Core/sip_mpu6500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/qinzechao/Desktop/cubeide_pro/car/Core/oled" -I"C:/Users/qinzechao/Desktop/cubeide_pro/car/Core/mpu6500" -I"C:/Users/qinzechao/Desktop/cubeide_pro/car/Core/sip_mpu6500" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-sip_mpu6500

clean-Core-2f-sip_mpu6500:
	-$(RM) ./Core/sip_mpu6500/mpu6500_spi.cyclo ./Core/sip_mpu6500/mpu6500_spi.d ./Core/sip_mpu6500/mpu6500_spi.o ./Core/sip_mpu6500/mpu6500_spi.su

.PHONY: clean-Core-2f-sip_mpu6500

