################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cJSON/Src/cJSON.c 

OBJS += \
./cJSON/Src/cJSON.o 

C_DEPS += \
./cJSON/Src/cJSON.d 


# Each subdirectory must supply rules for building sources it contributes
cJSON/Src/cJSON.o: ../cJSON/Src/cJSON.c cJSON/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"H:/Embedded/ST/c8t6_mqtt/Mqtt/Inc" -I"H:/Embedded/ST/c8t6_mqtt/cJSON/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"cJSON/Src/cJSON.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

