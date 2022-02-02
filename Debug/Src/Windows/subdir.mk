################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Windows/WIN1DLG.c 

OBJS += \
./Src/Windows/WIN1DLG.o 

C_DEPS += \
./Src/Windows/WIN1DLG.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Windows/WIN1DLG.o: ../Src/Windows/WIN1DLG.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/Inklud -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../STemWinLib/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Windows/WIN1DLG.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

