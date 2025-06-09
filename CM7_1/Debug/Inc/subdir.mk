################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/Char_lib.c \
../Inc/LCD_draw_lib.c \
../Inc/LCD_out.c \
../Inc/board.c \
../Inc/board_functions.c 

OBJS += \
./Inc/Char_lib.o \
./Inc/LCD_draw_lib.o \
./Inc/LCD_out.o \
./Inc/board.o \
./Inc/board_functions.o 

C_DEPS += \
./Inc/Char_lib.d \
./Inc/LCD_draw_lib.d \
./Inc/LCD_out.d \
./Inc/board.d \
./Inc/board_functions.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/%.o Inc/%.su Inc/%.cyclo: ../Inc/%.c Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32H755ZITx -DSTM32 -DSTM32H7DUAL -DSTM32H7 -DNUCLEO_H755ZI_Q -c -I"C:/Users/Felix/STM32CubeIDE/H755_CMSIS/CMSIS/STM32CubeH7-master/Drivers/CMSIS/Core/Include" -I"C:/Users/felix/STM32CubeIDE/workspace_1.18.1/H755_ILI9341/CM7/Inc" -I../Inc -I"C:/Users/Felix/STM32CubeIDE/H755_CMSIS/CMSIS/STM32CubeH7-master/Drivers/CMSIS/Device/ST/STM32H7xx/cmsis-device-h7-74b62d0bfa82e082c0610ff6a9137805df3cd192/Include" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Inc

clean-Inc:
	-$(RM) ./Inc/Char_lib.cyclo ./Inc/Char_lib.d ./Inc/Char_lib.o ./Inc/Char_lib.su ./Inc/LCD_draw_lib.cyclo ./Inc/LCD_draw_lib.d ./Inc/LCD_draw_lib.o ./Inc/LCD_draw_lib.su ./Inc/LCD_out.cyclo ./Inc/LCD_out.d ./Inc/LCD_out.o ./Inc/LCD_out.su ./Inc/board.cyclo ./Inc/board.d ./Inc/board.o ./Inc/board.su ./Inc/board_functions.cyclo ./Inc/board_functions.d ./Inc/board_functions.o ./Inc/board_functions.su

.PHONY: clean-Inc

