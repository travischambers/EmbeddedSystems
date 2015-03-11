################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lab3/buttons.c \
../src/Lab3/intervalTimer.c \
../src/Lab3/main-1.c \
../src/Lab3/main-2.c 

OBJS += \
./src/Lab3/buttons.o \
./src/Lab3/intervalTimer.o \
./src/Lab3/main-1.o \
./src/Lab3/main-2.o 

C_DEPS += \
./src/Lab3/buttons.d \
./src/Lab3/intervalTimer.d \
./src/Lab3/main-1.d \
./src/Lab3/main-2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Lab3/%.o: ../src/Lab3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -Wall -O2 -finline-functions -g3 -I"J:\Fall2014\ECEN330\TestAreaV01\My330Fall2014_SW_Env\Consolidated_330_SW" -c -fmessage-length=0 -I../../HW3_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


