/*
 * ticTacToeControl.c
 *
 *  Created on: Oct 29, 2014
 *      Author: tchambs
 */

#include "ticTacToeControl.h"
#include "ticTacToeDisplay.h"
#include "supportFiles/display.h"
#include "minimax.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "buttons.h"
#include <string.h>
#include "supportFiles/arduinoTypes.h"

enum CTRL_States {
	show_instructions, //show instructions
	instructions_wait, //wait
	init, //start here and stay for one tick
	never_touched, //wait until first touch - clock is disabled until set
	player_turn, //waiting for touch, clock is enabled and running
	computer_turn, //computer is making their move
	ad_timer_running, //waiting for touch controller ADC to settle
	game_over, //wait for a restart
	reset, //start the game over
} currentState = show_instructions;


int adTimer, firstMoveTimer, instructionsTimer = 0; //initialize timers to 0
uint8_t row, column;
minimax_board_t board;
bool playerFirst; //controls whether human is X or O

#define expiredValue 1 //only wait 1 tick
#define firstMove 10 //give the user some time to go first
#define instructionsTimeout 25 //give the user time to read instructions

//helper function declaration
void CTRL_updateBoard(minimax_board_t* board, uint8_t row, uint8_t column, bool player);

void ticTacToeControl_tick()
{
	// Perform state actions first.
	switch(currentState) //State Actions
	{
		case show_instructions:
			instructionsTimer = 0;
			display_init();
			display_fillScreen(DISPLAY_BLUE);
			display_setTextSize(3);
			display_println("Touch a square to play, \r\nor\r\n wait for the \r\n computer to go.");
			break;
		case instructions_wait:
			instructionsTimer++;
			break;
		case init:
			firstMoveTimer = 0;
			break;
		case never_touched:
			firstMoveTimer++;
			break;
		case player_turn: //reset timer
			adTimer = 0;
			break;
		case computer_turn:
			break;
		case ad_timer_running: //increment adTimer
			adTimer++;
			break;
		case reset:
			break;
		case game_over:
			break;
		default:
			printf("ticTacToe state update: hit default!!!\n\r");
			break;
	}

	// Perform state update next.
	switch (currentState) //Transitions
	{

		case show_instructions: //just come to this state for 1 tick
			currentState = instructions_wait;
			break;
		case instructions_wait: //show instructions for a little while
			if (instructionsTimer < instructionsTimeout)
			{
				currentState = instructions_wait;
			}
			else
				currentState = init;
			break;
		case init: //now display the board
			buttons_init();
			ticTacToeDisplay_init();
			minimax_initBoard(&board);
			currentState = never_touched;
			break;
		case never_touched: //wait to see if human will go first
			if (display_isTouched() && (firstMoveTimer < firstMove))
			{
				currentState = player_turn;
				display_clearOldTouchData();
				playerFirst = true; //human went first
			}
			else if (!display_isTouched() && (firstMoveTimer < firstMove))
			{
				currentState = never_touched; //keep waiting
			}
			else //if (!display_isTouched() && firstMoveTimer >= firstMove)
			{
				playerFirst = false; //computer is going first
				currentState = computer_turn;
			}
			break;
		case player_turn:
			if (display_isTouched() && !(buttons_read() & 0x01))
			{
				currentState = ad_timer_running; //make sure user did touch it
				display_clearOldTouchData();
			}
			else if (buttons_read() & 0x01)
			{
				currentState = reset; //reset game whenever human gives up
			}
			else //otherwise just wait until human goes
			{
				currentState = player_turn;
			}
			if (minimax_isGameOver(minimax_computeBoardScore(&board, playerFirst)))
			{
				currentState = game_over; //game is over, no more moves allowed
			}
			break;
		case computer_turn:
			if (playerFirst) //computer is O
			{
				minimax_computeNextMove(&board, false, &row, &column);
				CTRL_updateBoard(&board, row, column, false); //add the move to the board
				ticTacToeDisplay_drawO(row, column); //draw the move on the board
			}
			else //computer is X
			{
				minimax_computeNextMove(&board, true, &row, &column);
				CTRL_updateBoard(&board, row, column, true); //add the move to the board
				ticTacToeDisplay_drawX(row, column); //draw the move on the board
			}
			currentState = player_turn;
			break;
		case ad_timer_running:
			if (adTimer < expiredValue) //wait 1 tick for ADC to settle
			{
				currentState = ad_timer_running;
			}
			else
			{
				if (playerFirst) //human is X
				{
					ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
					CTRL_updateBoard(&board, row, column, true);
					ticTacToeDisplay_drawX(row, column);
				}
				else //human is O
				{
					ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
					CTRL_updateBoard(&board, row, column, false);
					ticTacToeDisplay_drawO(row, column);
				}
				currentState = computer_turn;
			}
			break;
		case game_over: //draw or computer wins
			if (buttons_read() & 0x01) //BTN 0 is pressed
				currentState = reset;
			else
				currentState = game_over;
			break;
		case reset:
			currentState = init;
			break;
		default: //error, should never reach here
			printf("ticTacToeControl_tick state update: hit default!!!\n\r");
	}
}

//helper to update our board HERE instead of in minimax
void CTRL_updateBoard(minimax_board_t* board, uint8_t row, uint8_t column, bool player)
{
	if (player) //put an X
		board->squares[row][column] = MINIMAX_PLAYER_SQUARE;
	else //put an O
		board->squares[row][column] = MINIMAX_OPPONENT_SQUARE;
}
