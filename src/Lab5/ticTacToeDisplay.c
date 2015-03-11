/*
 * ticTacToeDisplay.c
 *
 *  Created on: Oct 15, 2014
 *      Author: tchambs
 */

#include "ticTacToeDisplay.h"
#include "supportFiles/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include "switches.h"
#include "buttons.h"
#include <stdbool.h>

#define LEFT 0
#define LEFT_WIDTH display_width()/3
#define MIDDLE_WIDTH 2*display_width()/3
#define RIGHT_WIDTH display_width()
#define WIDTH_CENTER display_width()/6

#define TOP 0
#define TOP_HEIGHT display_height()/3
#define MIDDLE_HEIGHT 2*display_height()/3
#define BOTTOM_HEIGHT display_height()
#define HEIGHT_CENTER display_height()/6

//ROW NAMES
#define RTOP 0
#define RMIDDLE 1
#define RBOTTOM 2

//COLUMN NAMES
#define CLEFT 0
#define CMIDDLE 1
#define CRIGHT 2

// Inits the tic-tac-toe display, draws the lines that form the board.
void ticTacToeDisplay_init()
{
	display_fillScreen(DISPLAY_BLUE);
	ticTacToeDisplay_drawBoardLines();
}

// Draws an X at the specified row and column.
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column)
{
	if (row == RTOP) //top row
	{
		if (column == CLEFT) //left column
		{
			display_drawLine(LEFT, TOP, LEFT_WIDTH, TOP_HEIGHT, DISPLAY_WHITE);
			display_drawLine(LEFT_WIDTH, TOP, LEFT, TOP_HEIGHT, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawLine(LEFT_WIDTH, TOP, MIDDLE_WIDTH, TOP_HEIGHT, DISPLAY_WHITE);
			display_drawLine(MIDDLE_WIDTH, TOP, LEFT_WIDTH, TOP_HEIGHT, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawLine(MIDDLE_WIDTH, TOP, RIGHT_WIDTH, TOP_HEIGHT, DISPLAY_WHITE);
			display_drawLine(RIGHT_WIDTH, TOP, MIDDLE_WIDTH, TOP_HEIGHT, DISPLAY_WHITE);
		}
	}
	else if (row == RMIDDLE) //middle row
	{
		if (column == CLEFT) //left column
		{
			display_drawLine(LEFT, TOP_HEIGHT, LEFT_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
			display_drawLine(LEFT_WIDTH, TOP_HEIGHT, LEFT, MIDDLE_HEIGHT, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawLine(LEFT_WIDTH, TOP_HEIGHT, MIDDLE_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
			display_drawLine(MIDDLE_WIDTH, TOP_HEIGHT, LEFT_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawLine(MIDDLE_WIDTH, TOP_HEIGHT, RIGHT_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
			display_drawLine(RIGHT_WIDTH, TOP_HEIGHT, MIDDLE_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
		}
	}
	else //bottom row
	{
		if (column == CLEFT) //left column
		{
			display_drawLine(LEFT, MIDDLE_HEIGHT, LEFT_WIDTH, BOTTOM_HEIGHT, DISPLAY_WHITE);
			display_drawLine(LEFT_WIDTH, MIDDLE_HEIGHT, LEFT, BOTTOM_HEIGHT, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawLine(LEFT_WIDTH, MIDDLE_HEIGHT, MIDDLE_WIDTH, BOTTOM_HEIGHT, DISPLAY_WHITE);
			display_drawLine(MIDDLE_WIDTH, MIDDLE_HEIGHT, LEFT_WIDTH, BOTTOM_HEIGHT, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawLine(MIDDLE_WIDTH, MIDDLE_HEIGHT, RIGHT_WIDTH, BOTTOM_HEIGHT, DISPLAY_WHITE);
			display_drawLine(RIGHT_WIDTH, MIDDLE_HEIGHT, MIDDLE_WIDTH, BOTTOM_HEIGHT, DISPLAY_WHITE);
		}
	}
}

// Draws an O at the specified row and column.
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column)
{
	if (row == RTOP) //top row
	{
		if (column == CLEFT) //left column
		{
			display_drawCircle(LEFT+WIDTH_CENTER, TOP+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawCircle(LEFT_WIDTH+WIDTH_CENTER, TOP+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawCircle(MIDDLE_WIDTH+WIDTH_CENTER, TOP+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
	}
	else if (row == RMIDDLE) //middle row
	{
		if (column == CLEFT) //left column
		{
			display_drawCircle(LEFT+WIDTH_CENTER, TOP_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawCircle(LEFT_WIDTH+WIDTH_CENTER, TOP_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawCircle(MIDDLE_WIDTH+WIDTH_CENTER, TOP_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
	}
	else //bottom row
	{
		if (column == CLEFT) //left column
		{
			display_drawCircle(LEFT+WIDTH_CENTER, MIDDLE_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else if (column == CMIDDLE) //middle column
		{
			display_drawCircle(LEFT_WIDTH+WIDTH_CENTER, MIDDLE_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
		else //right column
		{
			display_drawCircle(MIDDLE_WIDTH+WIDTH_CENTER, MIDDLE_HEIGHT+HEIGHT_CENTER, HEIGHT_CENTER, DISPLAY_WHITE);
		}
	}
}

//Row and Column numbers of TIC TAC TOE BOARD
//
// 	  0   |   1  |   2
//   R0C0 | R0C1 | R0C2
//  ------|------|------
//    3   |  4   |   5
//   R1C0 | R1C1 | R1C2
//  ------|------|------
//    6   |  7   |   8
//   R2C0 | R2C1 | R2C2
//

// After a touch has been detected and after the proper delay, this sets the row and column arguments
// according to where the user touched the board.
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column)
{
	int16_t x, y;  // use these to keep track of coordinates
	uint8_t z;     // this is the relative touch pressure
	display_getTouchedPoint(&x, &y, &z);

	if (x < 0 || y < 0 || x > display_width() || y > display_width()) //this is outside of the LCD screen
	{
		printf("TOUCHED REGION OUTSIDE OF LCD SCREEN AREA!!\n\r");
		return;
	}
	if (x < LEFT_WIDTH) //must be in left column
	{
		if (y < TOP_HEIGHT)
		{
			*row = 0; 		//top
			*column = 0; 	//left
		}
		else if (y < MIDDLE_HEIGHT)
		{
			*row = 1; 		//middle
			*column = 0; 	//left
		}
		else
		{
			*row = 2; 		//bottom
			*column = 0;	//left
		}
	}
	else if (x < MIDDLE_WIDTH) //must be middle column
	{
		if (y < TOP_HEIGHT)
		{
			*row = 0;		//top
			*column = 1; 	//middle
		}
		else if (y < MIDDLE_HEIGHT)
		{
			*row = 1;		//middle
			*column = 1;	//middle
		}
		else
		{
			*row = 2;		//bottom
			*column = 1;		//middle
		}
	}
	else //must be right column
	{
		if (y < TOP_HEIGHT)
		{
			*row = 0; 		//top
			*column = 2; 	//right
		}
		else if (y < MIDDLE_HEIGHT)
		{
			*row = 1; 		//middle
			*column = 2; 	//right
		}
		else
		{
			*row = 2; 		//bottom
			*column = 2; 	//right
		}
	}
}

// Runs a test of the display. Does the following.
// Draws the board. Each time you touch one of the screen areas, the screen will paint
// an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
// When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.
void ticTacToeDisplay_runTest()
{
	uint8_t column, row;  // use these to keep track of coordinates
	ticTacToeDisplay_init();
	buttons_init();
	switches_init();
	while (1)
	{
		bool alreadyReset = false; //so we can only "reset" once
		bool testOver = false; //so we can terminate the game
		while(!display_isTouched()) //wait for user to touch
		{
			if (buttons_read() & 0x01 && !alreadyReset) //BTN 0 is pressed AND we haven't already reset
			{
				display_clearOldTouchData();
				display_fillScreen(DISPLAY_BLUE);
				ticTacToeDisplay_drawBoardLines();
				alreadyReset = true;
			}
			if (buttons_read() & 0x02) //BTN 1 is pressed
			{
				testOver = true;
				break;
			}

		}
		if (testOver) //
		{
			display_fillScreen(DISPLAY_MAGENTA);
			display_println("TEST TERMINATED");
			return;
		}

		int32_t value = switches_read();
		while(display_isTouched()){} //wait for user to let go
		display_clearOldTouchData();
		utils_msDelay(50); //wait for ADC to settle
		ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
		utils_msDelay(150); //add a 150ms delay so it seems to interact better
		if (value == 0x01) //if SW0 is up
			ticTacToeDisplay_drawO(row, column);
		else
			ticTacToeDisplay_drawX(row, column);
	}

//DRAW A CHARACTER IN EACH SQUARE
//				for (int i = 0; i < 3; i++)
//				{
//					for (int j = 0; j < 3; j++)
//					{
//						ticTacToeDisplay_drawO(i,j);
//						utils_msDelay(750);
//					}
//				}
}

// This will draw the four board lines.
void ticTacToeDisplay_drawBoardLines()
{
	//vertical lines
	display_drawLine(LEFT_WIDTH, BOTTOM_HEIGHT, LEFT_WIDTH, TOP, DISPLAY_WHITE);
	display_drawLine(MIDDLE_WIDTH, BOTTOM_HEIGHT, MIDDLE_WIDTH, TOP, DISPLAY_WHITE);

	//horizontal lines
	display_drawLine(LEFT, TOP_HEIGHT, display_width(), TOP_HEIGHT, DISPLAY_WHITE);
	display_drawLine(LEFT, MIDDLE_HEIGHT, RIGHT_WIDTH, MIDDLE_HEIGHT, DISPLAY_WHITE);
}
