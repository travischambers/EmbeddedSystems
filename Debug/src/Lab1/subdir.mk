################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lab1/helloWorld.c 

OBJS += \
./src/Lab1/helloWorld.o 

C_DEPS += \
./src/Lab1/helloWorld.d 


# Each subdirectory must supply rules for building sources it contributes
src/Lab1/%.o: ../src/Lab1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -Wall -O2 -finline-functions -g3 -I"J:\Fall2014\ECEN330\TestAreaV01\My330Fall2014_SW_Env\Consolidated_330_SW" -c -fmessage-length=0 -I../../HW3_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


