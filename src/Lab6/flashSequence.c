/*
 * flashSequence.c
 *
 *  Created on: Nov 17, 2014
 *      Author: tchambs
 */


// This will set the sequence to a simple sequential pattern.
// It starts by flashing the first color, and then increments the index and flashes the first
// two colors and so forth. Along the way it prints info messages to the LCD screen.

#include "flashSequence.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "supportFiles/display.h"
#include "simonDisplay.h"
#include "supportFiles/arduinoTypes.h"
#include "supportFiles/utils.h"
#include "xparameters.h"
#include "globals.h"

#define TEST_SEQUENCE_LENGTH 8	// Just use a short test sequence.
uint8_t flashSequence_testSequence[TEST_SEQUENCE_LENGTH] =
{
		SIMON_DISPLAY_REGION_0,
		SIMON_DISPLAY_REGION_1,
		SIMON_DISPLAY_REGION_2,
		SIMON_DISPLAY_REGION_3,
		SIMON_DISPLAY_REGION_3,
		SIMON_DISPLAY_REGION_2,
		SIMON_DISPLAY_REGION_1,
		SIMON_DISPLAY_REGION_0
};

#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"	// Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"			// Info message.
#define MESSAGE_TEXT_SIZE 2	                                	// Make the text easy to see.

uint8_t flashIndex, priorIndex = 0;

enum flashSequence_states
{
	init, 					//wait for button to be touched state
	draw_square,			//draw the square that is being pressed
	check_for_completion, 	//wait for button to be released state
	wait,					//allows the user to actually see what's happening
	finalWait,				//lets the last square show a bit longer
	final 					//wait for enable==false to move to button_touched
} flashSequence_state = init;

bool flashSequenceEnable = false; //disable state machine by default

uint16_t waitTimer = 0;
#define waitTimerExpiredValue 4

// Standard tick function.
void flashSequence_tick()
{
	// Perform state actions first.
	switch(flashSequence_state) //State Actions
	{
		case init:
			flashIndex = 0;
			break;
		case draw_square:
			break;
		case check_for_completion:
			waitTimer = 0;
			break;
		case wait:
			waitTimer++;
			break;
		case finalWait:
			waitTimer++;
		case final:
			break;
		default: //error, should never reach here
			display_println("reached default case in flashSequence State Actions case statement!\r\n");
			break;
	}

	// Perform state update next.
	switch (flashSequence_state) //Transitions
	{
	case init:
		if ((flashSequenceEnable == true))
		{
			flashSequence_state = draw_square;
			simonDisplay_eraseAllButtons();
//			display_fillScreen(DISPLAY_BLACK);
		}
		else
			flashSequence_state = init;
		break;
	case draw_square:
		uint8_t regionNumber;
		regionNumber = globals_getSequenceValue(flashIndex);
		simonDisplay_drawSquare(regionNumber, false);
		flashSequence_state = check_for_completion;
		break;
	case check_for_completion:
		uint8_t flashSequenceLength;
		flashSequenceLength = globals_getSequenceIterationLength();
		if (flashIndex >= flashSequenceLength - 1) //because flashIndex starts at 0
		{
			flashSequence_state = finalWait;
		}
		else
		{
			priorIndex = flashIndex;
			flashIndex++;
			flashSequence_state = wait;
		}
	 	 break;
	case wait:
		if (waitTimer > waitTimerExpiredValue)
		{
			simonDisplay_drawSquare(globals_getSequenceValue(priorIndex), true);
			flashSequence_state = draw_square;
		}
		else
		{
			flashSequence_state = wait;
		}

		break;
	case finalWait:
		if (waitTimer >= waitTimerExpiredValue)
		{
			flashSequence_state = final;
			simonDisplay_drawSquare(globals_getSequenceValue(priorIndex), true);
		}
		else
			flashSequence_state = finalWait;
			break;
	case final:
		if (flashSequenceEnable == false)
			flashSequence_state = init;
		else
			flashSequence_state = final;
		break;
	default: //error, should never reach here
		display_println("reached default case in flashSequence Transitions case statement!\r\n");
		break;
	}
}

// Turn on the state machine. Part of the interlock.
void flashSequence_enable()
{
	flashSequenceEnable = true;
}

// Turn off the state machine. Part of the interlock.
void flashSequence_disable()
{
	flashSequenceEnable = false;
}

bool flashSequence_completed()
{
	return (flashSequence_state == final);
}
// Print the incrementing sequence message.
void flashSequence_printIncrementingMessage()
{
	simonDisplay_eraseAllButtons();
//	display_fillScreen(DISPLAY_BLACK);			// Otherwise, tell the user that you are incrementing the sequence.
	display_setCursor(0, display_height()/2);	  	// Roughly centered.
	display_println(INCREMENTING_SEQUENCE_MESSAGE1);  // Print the message.
	utils_msDelay(2000);                              // Hold on for 2 seconds.
	simonDisplay_eraseAllButtons();
//	display_fillScreen(DISPLAY_BLACK);		    	// Clear the screen.
}

void flashSequence_runTest()
{
  display_init();						// We are using the display.
  display_fillScreen(DISPLAY_BLACK);	// Clear the display.
  globals_setSequence(flashSequence_testSequence, TEST_SEQUENCE_LENGTH);	// Set the sequence.
  flashSequence_enable();			    // Enable the flashSequence state machine.
  int16_t sequenceLength = 1;	        // Start out with a sequence of length 1.
  globals_setSequenceIterationLength(sequenceLength);	// Set the iteration length.
  display_setTextSize(MESSAGE_TEXT_SIZE);	        	// Use a standard text size.
  while (1)  							// Run forever unless you break.
  {
    flashSequence_tick();	// tick the state machine.
    utils_msDelay(1);		// Provide a 1 ms delay.
    if (flashSequence_completed()) // When you are done flashing the sequence.
    {
      flashSequence_disable();  // Interlock by first disabling the state machine.
      flashSequence_tick();		// tick is necessary to advance the state.
      utils_msDelay(1);			// don't really need this here, just for completeness.
      flashSequence_enable();	// Finish the interlock by enabling the state machine.
      utils_msDelay(1);			// Wait 1 ms for no good reason.
      sequenceLength++;			// Increment the length of the sequence.
      if (sequenceLength > TEST_SEQUENCE_LENGTH) // Stop if you have done the full sequence.
        break;
      flashSequence_printIncrementingMessage();  			// Tell the user that you are going to the next step in the pattern.
      globals_setSequenceIterationLength(sequenceLength);	// Set the length of the pattern.
    }
  }

  // Let the user know that you are finished.
  display_fillScreen(DISPLAY_BLACK);
  display_setCursor(0, display_height()/2);
  display_println(RUN_TEST_COMPLETE_MESSAGE);
}
