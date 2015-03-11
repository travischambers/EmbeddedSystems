################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lab5/buttons.c \
../src/Lab5/minimax.c \
../src/Lab5/switches.c \
../src/Lab5/ticTacToeControl.c \
../src/Lab5/ticTacToeDisplay.c \
../src/Lab5/ticTacToeMain.c 

OBJS += \
./src/Lab5/buttons.o \
./src/Lab5/minimax.o \
./src/Lab5/switches.o \
./src/Lab5/ticTacToeControl.o \
./src/Lab5/ticTacToeDisplay.o \
./src/Lab5/ticTacToeMain.o 

C_DEPS += \
./src/Lab5/buttons.d \
./src/Lab5/minimax.d \
./src/Lab5/switches.d \
./src/Lab5/ticTacToeControl.d \
./src/Lab5/ticTacToeDisplay.d \
./src/Lab5/ticTacToeMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/Lab5/%.o: ../src/Lab5/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -Wall -O2 -finline-functions -g3 -I"F:\My330Fall2014_SW_Env\Consolidated_330_SW" -c -fmessage-length=0 -I../../HW3_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


