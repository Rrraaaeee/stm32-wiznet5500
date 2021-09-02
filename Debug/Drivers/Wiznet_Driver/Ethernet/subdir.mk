################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Wiznet_Driver/Ethernet/socket.c \
../Drivers/Wiznet_Driver/Ethernet/wizchip_conf.c 

OBJS += \
./Drivers/Wiznet_Driver/Ethernet/socket.o \
./Drivers/Wiznet_Driver/Ethernet/wizchip_conf.o 

C_DEPS += \
./Drivers/Wiznet_Driver/Ethernet/socket.d \
./Drivers/Wiznet_Driver/Ethernet/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Wiznet_Driver/Ethernet/%.o: ../Drivers/Wiznet_Driver/Ethernet/%.c Drivers/Wiznet_Driver/Ethernet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"/Users/raeee/STM32CubeIDE/BBAUV/WiznetTest/Drivers/Wiznet_Driver/Ethernet" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

