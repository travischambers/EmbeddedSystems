/*
 * buttonHandler.c
 *
 *  Created on: Nov 10, 2014
 *      Author: tchambs
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "supportFiles/display.h"
#include "simonDisplay.h"
#include "supportFiles/arduinoTypes.h"
#include "supportFiles/utils.h"
#include "buttonHandler.h"
#include "xparameters.h"

#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."
#define RUN_TEST_TEXT_SIZE 2

enum buttonHandler_States {
	wait_for_touch, //wait for button to be touched state
	debounce, 		//wait for adc to settle
	draw_square,	//draw the square that is being pressed
	wait_for_release, //wait for button to be released state
	release_detected, //waits here for enable == false
} buttonHandler_state = wait_for_touch;

bool enable = false; //disable state machine by default
int16_t x, y; //touched coordinates
uint8_t z;
bool release = false;
uint8_t currentRegion = 0;
uint16_t adcTimer = 0;
#define adcTimerDone 1 //because we have a 20ms wait between each tick in our runTest

// Standard tick function.
void buttonHandler_tick()
{
	// Perform state actions first.
	switch(buttonHandler_state) //State Actions
	{
		case wait_for_touch:
			adcTimer = 0;
			break;
		case debounce:
			adcTimer++;
			break;
		case wait_for_release:
			break;
		case draw_square:
			break;
		case release_detected:
			break;
		default: //error, should never reach here
			display_println("reached default case in State Actions case statement!\r\n");
			break;
	}

	// Perform state update next.
	switch (buttonHandler_state) //Transitions
	{
	case wait_for_touch:
		display_clearOldTouchData();
		if ((enable == true) && display_isTouched())
		{
			buttonHandler_state = debounce;
		}
		else
			buttonHandler_state = wait_for_touch;
		break;
	case debounce:
		if ((adcTimer < adcTimerDone) && display_isTouched())
			buttonHandler_state = debounce;
		else if ((adcTimer >= adcTimerDone) && !display_isTouched())
			buttonHandler_state = wait_for_touch;
		else if ((adcTimer < adcTimerDone) && !display_isTouched())
				buttonHandler_state = wait_for_touch;
		else
		{
			buttonHandler_state = draw_square;
		}
		break;
	case draw_square:
		display_clearOldTouchData();
		currentRegion = buttonHandler_getRegionNumber();
		simonDisplay_drawSquare(currentRegion, false);
		buttonHandler_state = wait_for_release;
		break;
	case wait_for_release:
		if (!display_isTouched())
		{
			simonDisplay_drawSquare(currentRegion, true);
			simonDisplay_drawButton(currentRegion);
			buttonHandler_state = release_detected;
		}
		else
			buttonHandler_state = wait_for_release;
		break;
	case release_detected:
		if (enable == false)
			buttonHandler_state = wait_for_touch;
		else
			buttonHandler_state = release_detected;
	 	 break;
	default: //error, should never reach here
		display_println("reached default case in Transitions case statement!\r\n");
		break;
	}
}

// Get the simon region numbers. See the source code for the region numbering scheme.
uint8_t buttonHandler_getRegionNumber()
{
	display_clearOldTouchData();
	display_getTouchedPoint(&x, &y, &z);
	return simonDisplay_computeRegionNumber(x, y);
}

// Turn on the state machine. Part of the interlock.
void buttonHandler_enable()
{
	enable = true;
}

// Turn off the state machine. Part of the interlock.
void buttonHandler_disable()
{
	enable = false;
}

// Other state machines can call this function to see if the user has stopped touching the pad.
bool buttonHandler_releaseDetected()
{
	return (buttonHandler_state == release_detected);
}

void buttonHandler_runTest(int16_t touchCountArg)
{
  int16_t touchCount = 0;             // Keep track of the number of touches.
  display_init();                     // Always have to init the display.
  display_fillScreen(DISPLAY_BLACK);  // Clear the display.
  simonDisplay_drawAllButtons();      // Draw the four buttons.
  buttonHandler_enable();
  while (touchCount < touchCountArg) // Loop here while touchCount is less than the touchCountArg
  {
    buttonHandler_tick();               // Advance the state machine.
    utils_msDelay(1);					// Wait here for 1 ms.
    if (buttonHandler_releaseDetected()) // If a release is detected, then the screen was touched.
    {
      touchCount++;                         // Keep track of the number of touches.
      printf("button released: %d\n\r", buttonHandler_getRegionNumber());  // Get the region number that was touched.
      buttonHandler_tick();               	// Advance the state machine.
      buttonHandler_disable();              // Interlocked behavior: handshake with the button handler (now disabled).
      utils_msDelay(1);                     // wait 1 ms.
      buttonHandler_tick();                 // Advance the state machine.
      buttonHandler_enable();               // Interlocked behavior: enable the buttonHandler.
      utils_msDelay(1);                     // wait 1 ms.
      buttonHandler_tick();                 // Advance the state machine.
    }
  }
  display_fillScreen(DISPLAY_BLACK);			// clear the screen.
  display_setTextSize(RUN_TEST_TEXT_SIZE);		// Set the text size.
  display_setCursor(0, display_height()/2);		// Move the cursor to a rough center point.
  display_println(RUN_TEST_TERMINATION_MESSAGE1);	// Print the termination message on two lines.
  display_println(RUN_TEST_TERMINATION_MESSAGE2);
}
