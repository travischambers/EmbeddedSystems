################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lab6/buttonHandler.c \
../src/Lab6/buttons.c \
../src/Lab6/flashSequence.c \
../src/Lab6/globals.c \
../src/Lab6/simonControl.c \
../src/Lab6/simonDisplay.c \
../src/Lab6/simonMain.c \
../src/Lab6/verifySequence.c 

OBJS += \
./src/Lab6/buttonHandler.o \
./src/Lab6/buttons.o \
./src/Lab6/flashSequence.o \
./src/Lab6/globals.o \
./src/Lab6/simonControl.o \
./src/Lab6/simonDisplay.o \
./src/Lab6/simonMain.o \
./src/Lab6/verifySequence.o 

C_DEPS += \
./src/Lab6/buttonHandler.d \
./src/Lab6/buttons.d \
./src/Lab6/flashSequence.d \
./src/Lab6/globals.d \
./src/Lab6/simonControl.d \
./src/Lab6/simonDisplay.d \
./src/Lab6/simonMain.d \
./src/Lab6/verifySequence.d 


# Each subdirectory must supply rules for building sources it contributes
src/Lab6/%.o: ../src/Lab6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -Wall -O2 -finline-functions -g3 -I"F:\My330Fall2014_SW_Env\Consolidated_330_SW" -c -fmessage-length=0 -I../../HW3_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


