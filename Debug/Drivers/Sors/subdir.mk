################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Sors/SSD1963.c \
../Drivers/Sors/STMPE610.c 

OBJS += \
./Drivers/Sors/SSD1963.o \
./Drivers/Sors/STMPE610.o 

C_DEPS += \
./Drivers/Sors/SSD1963.d \
./Drivers/Sors/STMPE610.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Sors/SSD1963.o: ../Drivers/Sors/SSD1963.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/Inklud -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../STemWinLib/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Sors/SSD1963.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Sors/STMPE610.o: ../Drivers/Sors/STMPE610.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/Inklud -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../STemWinLib/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Sors/STMPE610.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

