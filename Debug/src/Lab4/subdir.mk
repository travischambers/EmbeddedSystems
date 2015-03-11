################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lab4/clockControl.c \
../src/Lab4/clockDisplay.c \
../src/Lab4/clockMain.c 

OBJS += \
./src/Lab4/clockControl.o \
./src/Lab4/clockDisplay.o \
./src/Lab4/clockMain.o 

C_DEPS += \
./src/Lab4/clockControl.d \
./src/Lab4/clockDisplay.d \
./src/Lab4/clockMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/Lab4/%.o: ../src/Lab4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -Wall -O2 -finline-functions -g3 -I"J:\Fall2014\ECEN330\TestAreaV01\My330Fall2014_SW_Env\Consolidated_330_SW" -c -fmessage-length=0 -I../../HW3_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


