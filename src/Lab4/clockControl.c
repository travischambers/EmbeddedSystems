/*
 * clockControl.c
 *
 *  Created on: Oct 7, 2014
 *      Author: tchambs
 */

#include "clockControl.h"
#include "clockDisplay.h"
#include "supportFiles/display.h"
#include <stdio.h>

enum CTRL_States {
	init, //start here and stay for one tick
	never_touched, //wait until first touch - clock is disabled until set
	waiting_for_touch, //waiting for touch, clock is enabled and running
	ad_timer_running, //waiting for touch controller ADC to settle
	auto_timer_running, //waiting for the auto-update delay to expire
						//(user is holding down auto-inc/dec button
	rate_timer_running, //waiting for the rate-timer to expire to know when to perform inc/dec
	rate_timer_expired, //when rate timer expires perform the inc/dec function
	add_second_to_clock //add a second to clock and reset ms counter
} currentState = init;


#define expiredValue 1 //only wait one tick -- works quicker
#define msWait 19 //wait for 19 clock ticks before advancing one second

//timers to wait for ADC, auto-increment, auto-increment speed, and advancing a second
int adTimer, autoTimer, rateTimer, msCounter = 0;


void clockControl_tick()
{
	// Perform state action first.
	switch(currentState) //State Actions
	{
		case init:
			break;
		case never_touched:
			break;
		case waiting_for_touch: //reset all timers
			adTimer = 0;
			autoTimer = 0;
			rateTimer = 0;
			break;
		case ad_timer_running: //increment adTimer
			adTimer++;
			msCounter = 0; //reset the second advancement counter
			break;
		case auto_timer_running: //increment autoTimer
			autoTimer++;
			msCounter = 0; //reset the second advancement counter
			break;
		case rate_timer_running: //increment rateTimer
			rateTimer++;
			msCounter = 0; //reset the second advancement counter
			break;
		case rate_timer_expired: //set rateTimer to 0
			rateTimer = 0;
			msCounter = 0; //reset the second advancement counter
			break;
		case add_second_to_clock:
			clockDisplay_advanceTimeOneSecond();
			msCounter = 0; //reset the second advancement counter
			break;
		default:
			printf("clockControl_tick state update: hit default!!!\n\r");
			break;
	}

	// Perform state update next.
	switch (currentState) //Transitions
	{
		case init:
			currentState = never_touched;
			break;
		case never_touched:
			if (display_isTouched())
			{
				currentState = waiting_for_touch;
				display_clearOldTouchData();
			}
			else
			{
				currentState = never_touched;
			}
			break;
		case waiting_for_touch:
			if (display_isTouched())
			{
				currentState = ad_timer_running;
				display_clearOldTouchData();
			}
			else if (msCounter >= msWait)
			{
				currentState = add_second_to_clock;
				display_clearOldTouchData();
			}
			else
			{
				msCounter++;
				currentState = waiting_for_touch;
			}
			break;
		case ad_timer_running:
			if (!display_isTouched() && adTimer >= expiredValue)
			{
				currentState = waiting_for_touch;
				clockDisplay_performIncDec();
			}
			else if (display_isTouched() && adTimer >= expiredValue)
			{
				currentState = auto_timer_running;
			}
			else
			{
				currentState = ad_timer_running;
			}
			break;
		case auto_timer_running:
			if (!display_isTouched())
			{
				currentState = waiting_for_touch;
				clockDisplay_performIncDec();
			}
			else if (display_isTouched() && autoTimer == expiredValue)
			{
				currentState = rate_timer_running;
				clockDisplay_performIncDec();
			}
			else
			{
				currentState = auto_timer_running;
			}
			break;
		case rate_timer_running:
			if (!display_isTouched())
			{
				currentState = waiting_for_touch;
			}
			else if (display_isTouched() && rateTimer == expiredValue)
			{
				currentState = rate_timer_expired;
			}
			else
			{
				currentState = rate_timer_running;
			}
			break;
		case rate_timer_expired:
			if (!display_isTouched())
			{
				currentState = waiting_for_touch;
			}
			else // if (display_isTouched())
			{
				currentState = rate_timer_running;
				clockDisplay_performIncDec();
			}
			break;
		case add_second_to_clock:
			if (!display_isTouched())
			{
				currentState = waiting_for_touch;
			}
			else //if display_isTouched()
			{
				currentState = ad_timer_running;
			}

			break;
		default:
			printf("clockControl_tick state update: hit default!!!\n\r");
	}

}
