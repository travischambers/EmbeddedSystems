/*
 * simonDisplay.c
 *
 *  Created on: Nov 3, 2014
 *      Author: tchambs
 */

#include <stdio.h>
#include "supportFiles/display.h"
#include "supportFiles/utils.h"
#include "simonDisplay.h"
#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 60 // in ms
#define MAX_STR 255
#define TEXT_SIZE 2

// The touch-screen is divided into 4 rectangular regions, numbered 0 - 3.
// Each region will be drawn with a different color. Colored buttons remind
// the user which square is associated with each color. When you press
// a region, computeRegionNumber returns the region number that is used
// by the other routines.

/*
|----------|----------|
|          |          |
|    0     |     1    |
|          |          |
-----------------------
|          |          |
|     2    |    3     |
|          |          |
-----------------------
*/

// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.

// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount)
{
  display_init();  // Always initialize the display.
  char str[MAX_STR];   // Enough for some simple printing.
  uint8_t regionNumber;
  uint16_t touches = 0;
  // Write an informational message and wait for the user to touch the LCD.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); //
  display_setTextSize(TEXT_SIZE);
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);
  sprintf(str, "Touch and release to start the Simon demo.");
  display_println(str);
  display_println();
  sprintf(str, "Demo will terminate after %d touches.", touchCount);
  display_println(str);
  while (!display_isTouched());       // Wait here until the screen is touched.
  while (display_isTouched());        // Now wait until the touch is released.
  display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
  simonDisplay_drawAllButtons();      // Draw all of the buttons.
  bool touched = false;  	      // Keep track of when the pad is touched.
  int16_t x, y;  		      // Use these to keep track of coordinates.
  uint8_t z;      		      // This is the relative touch pressure.
  while (touches < touchCount) {  // Run the loop according to the number of touches passed in.
    if (!display_isTouched() && touched) {         // user has stopped touching the pad.
      simonDisplay_drawSquare(regionNumber, true); // Erase the square.
      simonDisplay_drawButton(regionNumber);	   // Draw the button.
      touched = false;															// Released the touch, set touched to false.
    } else if (display_isTouched() && !touched) {   // User started touching the pad.
      touched = true;                               // Just touched the pad, set touched = true.
      touches++;  																	// Keep track of the number of touches.
      display_clearOldTouchData();  // Get rid of data from previous touches.
      // Must wait this many milliseconds for the chip to do analog processing.
      utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
      display_getTouchedPoint(&x, &y, &z);                  // After the wait, get the touched point.
      regionNumber = simonDisplay_computeRegionNumber(x, y);// Compute the region number.
      simonDisplay_drawSquare(regionNumber, false);	    // Draw the square (erase = false).
    }
  }
  // Done with the demo, write an informational message to the user.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); // Place the cursor in the middle of the screen.
  display_setTextSize(2);                   // Make it readable.
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);  // red is foreground color, black is background color.
  sprintf(str, "Simon demo terminated");    // Format a string using sprintf.
  display_println(str);                     // Print it to the LCD.
  sprintf(str, "after %d touches.", touchCount);  // Format the rest of the string.
  display_println(str);  // Print it to the LCD.
}

// This routine assumes that the touch-screen coordinates have been remapped to the LCD coordinate space.
// It returns a negative number if the x, y coordinates are negative (out of LCD bounds).
int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y)
{
  if (x < 0 || y < 0)
    return -1;
  if (x < display_width()/2)// Must be region 0 or two.
  {
	  if (y < display_height()/2)
		  return 0;
	  else
		  return 2;
  }
  else  // must be region 1 or 3.
  {
      if (y < display_height()/2)
    	  return 1;
      else
    	  return 3;
  }
}

// Just draws four coloDISPLAY_RED non-filled squares that match the colors of the filled squares.
void simonDisplay_drawButton(uint8_t regionNumber)
{
	// Do nothing if the region number is negative (illegal region, off LCD screen).
	if (regionNumber < 0)
	  return;
	// API: fillRect(x, y, width, height)
	switch(regionNumber)
	{
		case 2:
			display_fillRect(display_width()/4-(SIMON_DISPLAY_BUTTON_WIDTH/2), (3*display_height()/4)-(SIMON_DISPLAY_BUTTON_WIDTH/2), SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_BLUE);
			break;
		case 0:
			display_fillRect((display_width()/4)-(SIMON_DISPLAY_BUTTON_WIDTH/2), (display_height()/4)-(SIMON_DISPLAY_BUTTON_WIDTH/2), SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_RED);
			break;
		case 1:
			display_fillRect(3*display_width()/4-(SIMON_DISPLAY_BUTTON_WIDTH/2), display_height()/4-(SIMON_DISPLAY_BUTTON_WIDTH/2), SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_YELLOW);
			break;
		case 3:
			display_fillRect(3*display_width()/4-(SIMON_DISPLAY_BUTTON_WIDTH/2), 3*display_height()/4-(SIMON_DISPLAY_BUTTON_WIDTH/2), SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_GREEN);
			break;
	}
}

// This will light up a colored square on the LCD according to the region number (see above).
void simonDisplay_drawSquare(uint8_t regionNo, bool erase)
{
  // Do nothing if the region number is illegal (off LCD screen).
  if (regionNo < 0)
    return;
  switch(regionNo)
  {
  	  case 0:
  		  if (!erase)
  			  display_fillRect(0, 0, display_width()/2, display_height()/2, DISPLAY_RED);
  		  else
  			  display_fillRect(0, 0, display_width()/2, display_height()/2, DISPLAY_BLACK);
  		  break;
  	  case 1:
  		  if (!erase)
  			  display_fillRect(display_width()/2, 0, display_width()/2, display_height()/2, DISPLAY_YELLOW);
  		  else
  			  display_fillRect(display_width()/2, 0, display_width()/2, display_height()/2, DISPLAY_BLACK);
  		  break;
  	  case 2:
  		  if (!erase)
  			  display_fillRect(0, display_height()/2, display_width()/2, display_height()/2, DISPLAY_BLUE);
  		  else
  			  display_fillRect(0, display_height()/2, display_width()/2, display_height()/2, DISPLAY_BLACK);
  		  break;
  	  case 3:
  		  if (!erase)
  			  display_fillRect(display_width()/2, display_height()/2, display_width()/2, display_height()/2, DISPLAY_GREEN);
  		  else
  			  display_fillRect(display_width()/2, display_height()/2, display_width()/2, display_height()/2, DISPLAY_BLACK);
  		  break;
   }
}

//convenience function that draws all the buttons
void simonDisplay_drawAllButtons()
{
	simonDisplay_drawButton(0);
	simonDisplay_drawButton(1);
	simonDisplay_drawButton(2);
	simonDisplay_drawButton(3);
}

void simonDisplay_eraseAllButtons()
{
	simonDisplay_drawSquare(0, true);
	simonDisplay_drawSquare(1, true);
	simonDisplay_drawSquare(2, true);
	simonDisplay_drawSquare(3, true);
}
