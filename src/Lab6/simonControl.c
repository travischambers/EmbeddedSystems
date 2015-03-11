/*
 * simonControl.c
 *
 *  Created on: Nov 20, 2014
 *      Author: tchambs
 */


#include "simonControl.h"
#include "simonDisplay.h"
#include "flashSequence.h"
#include "verifySequence.h"
#include "stdlib.h"
#include "string.h"
#include <time.h>
#include "stdio.h"
#include "supportFiles/display.h"
#include "globals.h"
#include "buttons.h"

#define BTN0 1

enum simonControl_states
{
	init,			//initial state
	touch_to_start, //wait for a button press
	flash,			//flash the sequence
	verify,			//check if verifysequence is done
	verify_check,   //check if user touched correctly
	victory_wait,   //user correctly tapped sequence, give them a break
	keep_playing,   //check if the user wants to keep playing
	increment,		//add a button to sequence
	wait,			//used to help user experience
	final			//final state
} simonControl_state = init;

#define INITIALSEQUENCELENGTH 4
uint16_t seed = 99;
uint16_t gameLength = 0;
uint8_t randomSquare = 0;
uint16_t controlWaitTimer = 0;
#define controlWaitTimerExpired 5
#define longWait controlWaitTimerExpired*3
time_t t;

void simonControl_tick()
{
	//increment seed on each tick, this gives us randomness
	seed++;

	// Perform state actions first.
	switch(simonControl_state) //State Actions
	{
	case init:
		display_init();						// We are using the display.
		simonDisplay_eraseAllButtons();		// Clear the display.
		break;
	case touch_to_start:
		break;
	case flash:
		break;
	case verify:
		break;
	case verify_check:
		break;
	case victory_wait:
		controlWaitTimer++;
		break;
	case keep_playing:
		controlWaitTimer++;
		break;
	case increment:
		break;
	case wait:
		controlWaitTimer++;
		break;
	case final:
		controlWaitTimer++;
		break;
	default:
		break;
	}

	// Perform state update next.
	switch (simonControl_state) //Transitions
	{
	case init:
		simonControl_state = touch_to_start;
		display_setTextSize(3); //medium text size
		display_setCursor(display_height()/10, display_width()/6); // mostly centered
		display_println("Touch to start!");
		break;
	case touch_to_start:
		if (display_isTouched())
		{

			/* Intializes random number generator */
			
			gameLength = 1;	// Start out with a sequence of length 1.
			srand(seed);
			for (int i = 1; i < INITIALSEQUENCELENGTH; i++) //initialize a sequence length of 4 -- we started with 1
			{
				randomSquare = rand() % 4;  // gives us a number between 0 and 3
				globals_setSequenceValue(gameLength, randomSquare);	// add randomSquare to sequence
				gameLength++;

			}
			globals_setSequenceIterationLength(gameLength);

			//prepare the flashSequence state machine
			flashSequence_enable();

			simonControl_state = flash;
		}
		else
			simonControl_state = touch_to_start;
		break;
	case flash:
		if (flashSequence_completed())
		{
			flashSequence_disable();

			verifySequence_enable();

			verifySequence_drawButtons();
			simonControl_state = verify;
		}
		else
		{
			simonControl_state = flash;
		}
		break;
	case verify:
		if (verifySequence_isComplete())
		{
			verifySequence_disable();
			simonDisplay_eraseAllButtons();
			simonControl_state = verify_check;
		}
		else
			simonControl_state = verify;
		break;
	case verify_check:
		if (verifySequence_isTimeOutError() || verifySequence_isUserInputError()) //user error
		{
			display_setCursor(2*display_height()/5, display_width()/6); // mostly centered
			display_setTextSize(4);
			display_println("FAIL!");
			controlWaitTimer = 0;
			simonControl_state = final;
	    }
		else
	    {
			controlWaitTimer = 0;
			display_setCursor(2*display_height()/5, display_width()/6); // mostly centered
			display_setTextSize(4);
			display_println("GOOD!");
			simonControl_state = victory_wait;
	    }
		break;
	case victory_wait:
		if (controlWaitTimer > controlWaitTimerExpired)
		{
			simonDisplay_eraseAllButtons();
			//display_fillScreen(DISPLAY_BLACK);
			display_setTextSize(2);
			char str[100];
			sprintf(str, "Longest Sequence: %d", gameLength);
			display_setCursor(display_height()/8, display_width()/6); // mostly centered
			display_println(str);
			controlWaitTimer = 0;
			simonControl_state = keep_playing;
		}
		else
			simonControl_state = victory_wait;
		break;
	case keep_playing:
		if (controlWaitTimer > controlWaitTimerExpired)
		{
			simonDisplay_eraseAllButtons();
			display_setCursor(display_height()/8, display_width()/6); // mostly centered
			display_println("Touch for New Level");
			controlWaitTimer = 0; //reset wait timer
			simonControl_state = wait;
		}
		else
			simonControl_state = keep_playing;
		break;
	case increment:
		//increment our sequence
		randomSquare = rand() % 4;  // gives us a number between 0 and 3
		gameLength++;	// increment our sequence length
		globals_setSequenceIterationLength(gameLength);	//update our length
		globals_setSequenceValue(gameLength, randomSquare);	// add randomSquare to sequence
		flashSequence_enable(); 		//enable flashSequence state machine
		simonControl_state = flash;
		break;
	case wait:
		if (controlWaitTimer > longWait) //wait 3 times as long
		{
			controlWaitTimer = 0; //reset waitTimer
			simonControl_state = final;
		}
		else if (display_isTouched())
		{
			controlWaitTimer = 0;
			simonControl_state = increment;
		}
		else
			simonControl_state = wait;
		break;
	case final:
		if (controlWaitTimer > controlWaitTimerExpired)
		{
			controlWaitTimer = 0;
			simonControl_state = init;
		}
		else
			simonControl_state = final;
		break;
	default:
		break;
	}


}