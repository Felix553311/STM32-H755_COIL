################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32H755ZITx -DSTM32 -DSTM32H7DUAL -DSTM32H7 -DNUCLEO_H755ZI_Q -c -I"C:/Users/felix/Documents/H755_CMSIS/CMSIS/STM32CubeH7-master/Drivers/CMSIS/Device/ST/STM32H7xx/cmsis-device-h7-74b62d0bfa82e082c0610ff6a9137805df3cd192/Include" -I"C:/Users/felix/STM32CubeIDE/workspace_1.18.1/H755_ILI9341/CM7/Inc" -I../Inc -I"C:/Users/felix/Documents/H755_CMSIS/CMSIS/STM32CubeH7-master/Drivers/CMSIS/Core/Include" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

