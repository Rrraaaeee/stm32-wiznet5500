################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Wiznet_Driver/Ethernet/W5500/w5500.c 

OBJS += \
./Drivers/Wiznet_Driver/Ethernet/W5500/w5500.o 

C_DEPS += \
./Drivers/Wiznet_Driver/Ethernet/W5500/w5500.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Wiznet_Driver/Ethernet/W5500/%.o: ../Drivers/Wiznet_Driver/Ethernet/W5500/%.c Drivers/Wiznet_Driver/Ethernet/W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"/Users/raeee/STM32CubeIDE/BBAUV/WiznetTest/Drivers/Wiznet_Driver/Ethernet" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

