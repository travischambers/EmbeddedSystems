/*
 * clockDisplay.c
 *
 *  Created on: Oct 7, 2014
 *      Author: tchambs
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "supportFiles/display.h"
#include "supportFiles/utils.h"

#define SCALE_FACTOR 5

#define BOX_HEIGHT display_height()/6 	//height of clock area BEFORE scaling
#define BOX_WIDTH display_width()/6 	//width of clock area BEFORE scaling
#define TOTAL_HEIGHT BOX_HEIGHT*SCALE_FACTOR 	//height of box AFTER scaling
#define TOTAL_WIDTH BOX_WIDTH*SCALE_FACTOR 		//width of box AFTER scaling
#define TEXT_BLOCK_WIDTH TOTAL_WIDTH/8 		//split box into 8 columns
#define TEXT_BLOCK_HEIGHT TOTAL_HEIGHT/3 	//split box into 3 rows
#define TEXT_BLOCK_CENTER_HEIGHT TEXT_BLOCK_HEIGHT/4 	//find center so text spacing looks good
#define TEXT_BLOCK_CENTER_WIDTH TEXT_BLOCK_WIDTH/4 		//find center so text spacing looks good
#define HEIGHT_MARGIN (BOX_HEIGHT*6 - TOTAL_HEIGHT)/2 	//height above/below scaled box
#define WIDTH_MARGIN (BOX_WIDTH*6 - TOTAL_WIDTH)/2 		//width to the left/right of scaled box
#define CURSOR_START_HEIGHT TOTAL_HEIGHT+HEIGHT_MARGIN 	//where we start drawing vertically
#define CURSOR_START_WIDTH 320-(TOTAL_WIDTH+WIDTH_MARGIN) //where we start drawing horizontally
#define BUFFER (10/6)*SCALE_FACTOR 	//spacing to keep arrows apart from numbers

#define COLUMN_0 CURSOR_START_WIDTH							//far left side of the box
#define COLUMN_1 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH)	//one column of text over
#define COLUMN_2 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*2)	//two columns of text over
#define COLUMN_3 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*3)	//three columns..
#define COLUMN_4 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*4)	//four columns
#define COLUMN_5 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*5)	//you get the point
#define COLUMN_6 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*6)	//
#define COLUMN_7 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*7)	//
#define COLUMN_8 CURSOR_START_WIDTH + (TEXT_BLOCK_WIDTH*8)	//

#define ROW_0 CURSOR_START_HEIGHT							//top of box
#define ROW_1 CURSOR_START_HEIGHT - (TEXT_BLOCK_HEIGHT)		//one row down
#define ROW_2 CURSOR_START_HEIGHT - (TEXT_BLOCK_HEIGHT*2)	//two rows down
#define ROW_3 CURSOR_START_HEIGHT - (TEXT_BLOCK_HEIGHT*3)	//bottom row

#define INITIAL_HOURS 12 	//initial value to display for hours
#define INITIAL_MINUTES 59	//initial value to display for minutes
#define INITIAL_SECONDS 55	//initial value to display for seconds

uint8_t hours = INITIAL_HOURS;
int8_t minutes = INITIAL_MINUTES;
int8_t seconds = INITIAL_SECONDS;

#define SECONDS_IN_A_MINUTE 60
#define MINUTES_IN_AN_HOUR 60
#define HOURS_IN_A_DAY 12

#define ARRAY_SIZE 9 //number of characters we can print

//used to figure out which region has been pressed
int8_t clockDisplay_computeRegionNumber(int16_t x, int16_t y);

// Called only once - performs any necessary inits.
void clockDisplay_init()
{
	display_init();  // initialize the display. setRotation(1) happens here.

	display_fillScreen(DISPLAY_BLACK); //change the screen to BLACK

	//top row first triangle
	display_fillTriangle(COLUMN_0, ROW_2-BUFFER, COLUMN_1, ROW_3, COLUMN_2, ROW_2-BUFFER, DISPLAY_GREEN);

	//top row second triangle
	display_fillTriangle(COLUMN_3, ROW_2-BUFFER, COLUMN_4, ROW_3, COLUMN_5, ROW_2-BUFFER, DISPLAY_GREEN);

	//top row third triangle
	display_fillTriangle(COLUMN_6, ROW_2-BUFFER, COLUMN_7, ROW_3, COLUMN_8, ROW_2-BUFFER, DISPLAY_GREEN);

	//middle row third digit -- first ":"
	display_drawChar(COLUMN_2, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, ':', DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row sixth digit -- second ":"
	display_drawChar(COLUMN_5, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, ':', DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//bottom row first triangle
	display_fillTriangle(COLUMN_0, ROW_1+BUFFER, COLUMN_1, ROW_0, COLUMN_2, ROW_1+BUFFER, DISPLAY_GREEN);

	//bottom row second triangle
	display_fillTriangle(COLUMN_3, ROW_1+BUFFER, COLUMN_4, ROW_0, COLUMN_5, ROW_1+BUFFER, DISPLAY_GREEN);

	//bottom row third triangle
	display_fillTriangle(COLUMN_6, ROW_1+BUFFER, COLUMN_7, ROW_0, COLUMN_8, ROW_1+BUFFER, DISPLAY_GREEN);


}

// Updates the time display with latest time.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll)
{
	char oldTime[ARRAY_SIZE];
	char curTime[ARRAY_SIZE];


	if (seconds >= SECONDS_IN_A_MINUTE)
	{
		seconds = 0;
		if (forceUpdateAll)
			minutes++;
	}
	if (seconds < 0)
	{
		seconds = SECONDS_IN_A_MINUTE - 1; //set the seconds to 59
		if (forceUpdateAll)
			minutes--;
	}
	if (minutes >= MINUTES_IN_AN_HOUR)
	{
		minutes = 0;
		if (forceUpdateAll)
			hours++;
	}
	if (minutes < 0)
	{
		minutes = MINUTES_IN_AN_HOUR - 1; //set the minutes to 59
		if (forceUpdateAll)
			hours--;
	}
	if (hours > HOURS_IN_A_DAY)
	{
		hours = 1; 	//rollover hours
					//hours go from 1-12 instead of zero based
	}
	if (hours < 1) //hours start at 1
	{
		hours = HOURS_IN_A_DAY;
	}

	sprintf(curTime, "%2d:%02d:%02d", hours, minutes, seconds);

	//middle row first digit -- tens place for the hours
	if (curTime[0] != oldTime[0])
		display_drawChar(COLUMN_0, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[0], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row second digit -- ones place for the hours
	if (curTime[1] != oldTime[1])
		display_drawChar(COLUMN_1, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[1], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row fourth digit -- tens place for the minutes
	if (curTime[3] != oldTime[3])
		display_drawChar(COLUMN_3, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[3], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row fifth digit -- ones place for the minutes
	if (curTime[4] != oldTime[4])
		display_drawChar(COLUMN_4, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[4], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row seventh digit -- tens place for the seconds
	if (curTime[6] != oldTime[6])
		display_drawChar(COLUMN_6, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[6], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	//middle row eighth digit -- ones place for the seconds
	if (curTime[7] != oldTime[7])
		display_drawChar(COLUMN_7, ROW_2+TEXT_BLOCK_CENTER_HEIGHT, curTime[7], DISPLAY_GREEN, DISPLAY_BLACK, SCALE_FACTOR);

	strcpy(oldTime, curTime);
}

// The touch-screen is divided into 6 rectangular regions, numbered 0 - 5.
// Each region will have a triangle in it. When you press
// a region, computeRegionNumber returns the region number that is used
// by the other routines.
/*
|----------|----------|----------|
|          |          |          |
|    0     |     1    |     2    |
|          |          |          |
----------------------------------
|          |          |          |
|     3    |     4    |     5    |
|          |          |          |
|----------|----------|----------|
*/
int8_t clockDisplay_computeRegionNumber(int16_t x, int16_t y) {
  if (x < 0 || y < 0) //this is outside of the LCD screen
    return -1;
  if (x < COLUMN_3) //must be region 0 or 3
  {
	  if (y < ROW_2+TEXT_BLOCK_CENTER_HEIGHT)
		  return 0; //top left
	  else
		  return 3; //bottom left
  }
  else if (x < COLUMN_5) //must be region 1 or 4
  {
      if (y < ROW_2+TEXT_BLOCK_CENTER_HEIGHT)
    	  return 1; //top middle
      else
    	  return 4; //bottom middle
  }
  else //must be region 2 or 5
  {
	  if (y < ROW_2+TEXT_BLOCK_CENTER_HEIGHT)
		  return 2; //top right
	  else
		  return 5; //bottom right
  }
}

// Performs the increment or decrement, depending upon the touched region.
void clockDisplay_performIncDec()
{

	int16_t x, y;  // use these to keep track of coordinates
	uint8_t z;     // this is the relative touch pressure
	int8_t regionNumber;
	display_getTouchedPoint(&x, &y, &z);
	regionNumber = clockDisplay_computeRegionNumber(x, y);

	switch (regionNumber)
	{
		case -1:
			printf("ERROR! Touched Point outside region of the screen\n\r");
			break;
		case 0:
			hours++;
			break;
		case 1:
			minutes++;
			break;
		case 2:
			seconds++;
			break;
		case 3:
			hours--;
			break;
		case 4:
			minutes--;
			break;
		case 5:
			seconds--;
			break;
		default:
			printf("ERROR! not a legitimate regionNumber\n\r");
	}

	clockDisplay_updateTimeDisplay(0); //don't rollover to minutes/hours
}

// Advances the time forward by 1 second.
void clockDisplay_advanceTimeOneSecond()
{
	seconds++;
	clockDisplay_updateTimeDisplay(1); //this is a normal increment, update minutes/hours if necessary
}

// Run a test of clock-display functions.
void clockDisplay_runTest()
{
	clockDisplay_init();

	while(!display_isTouched()); // wait for touch to initiate runTest
	while (display_isTouched()); // wait for user to let go of touchscreen
	while(!display_isTouched()) // increment until user presses touchscreen again
	{
		seconds++;
		clockDisplay_performIncDec();
		utils_msDelay(1000);
	}
	while(display_isTouched()); //wait for user to let go of touchscreen
	while(!display_isTouched()) // increment until screen is touched again
	{
		seconds--;
		clockDisplay_performIncDec();
		utils_msDelay(1000);
	}
	while(display_isTouched()); //wait for user to let go of touchscreen
	while(!display_isTouched()) // increment until screen is touched again
	{
		seconds++;
		clockDisplay_performIncDec();
		utils_msDelay(100);
	}
}
