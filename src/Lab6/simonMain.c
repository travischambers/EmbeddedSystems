/*
 * simonMain.c
 *
 *  Created on: Nov 3, 2014
 *      Author: tchambs
 */

#include "simonDisplay.h"
#include "stdint.h"
#include "buttonHandler.h"
#include "flashSequence.h"
#include "verifySequence.h"
#include "supportFiles/utils.h"
#include "simonControl.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include "supportFiles/leds.h"
#include <stdio.h>

#define TOTAL_SECONDS 50

#define TIMER_PERIOD .120 //120 milliseconds
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)
extern u32 isrInvocationCount;

int main()
{

	//make sure all state machines are disabled
	buttonHandler_disable();
	verifySequence_disable();
	flashSequence_disable();

	// Initialize the GPIO LED driver and print out an error message if it fails (argument = true).
    leds_init(true); //init the LEDs so that LD4 can function as a heartbeat.
    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
    u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
    printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
    // Allow the timer to generate interrupts.
    interrupts_enableTimerGlobalInts();
    // Keep track of your personal interrupt count. Want to make sure that you don't miss any interrupts.
    int32_t isrInvocationCountt = 0;
    // Start the private ARM timer running.
    interrupts_startArmPrivateTimer();
    // Enable interrupts at the ARM.
    interrupts_enableArmInts();
    // interrupts_isrInvocationCount() returns the number of times that the timer ISR was invoked.
    // This value is maintained by the timer ISR. Compare this number with your own local
    // interrupt count to determine if you have missed any interrupts.

   while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond))
   {
	   if (interrupts_isrFlagGlobal) //this is a global flag that is set by the timer interrupt handler
	   {
	   	   isrInvocationCountt++; //count ticks.
	   	   simonControl_tick();
   		   buttonHandler_tick();
   		   verifySequence_tick();
   		   flashSequence_tick();
	   	   interrupts_isrFlagGlobal = 0;
	   }
   }

   interrupts_disableArmInts();
   printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());

   printf("internal interrupt count: %ld\n\r", isrInvocationCount);

	return 0;
}
