################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../include/driver_gpio.c \
../include/driver_rs232.c \
../include/driver_rs232_custom.c 

OBJS += \
./include/driver_gpio.o \
./include/driver_rs232.o \
./include/driver_rs232_custom.o 

C_DEPS += \
./include/driver_gpio.d \
./include/driver_rs232.d \
./include/driver_rs232_custom.d 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imc -mabi=ilp32 -msmall-data-limit=8 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:\EDAptix\projects\arduissimo\dev\tests\drivers\drivers_rs232\ws_drivers_rs232\drivers_rs232_0\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


