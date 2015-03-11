/*
 * ticTacToeDisplay.c
 *
 *  Created on: Oct 15, 2014
 *      Author: tchambs
 */

#include "ticTacToeDisplay.h"
#include "minimax.h"
#include "ticTacToeControl.h"
#include "supportFiles/utils.h"
#include <stdio.h>
#include "supportFiles/leds.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include <stdbool.h>
#include <stdint.h>
#include "supportFiles/display.h"
#include "xparameters.h"

#define TOTAL_SECONDS 60 //run for 1 minute total

#define TIMER_PERIOD .24 //240 milliseconds
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)


int main()
{


//	MILESTONE 1
//	ticTacToeDisplay_runTest();

//  MILESTONE 2
//	minimax_board_t board;
//	minimax_initBoard(&board);
//  minimax_board_t board1;  // Board 1 is the main example in the web-tutorial that I use on the web-site.
//  board1.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//  board1.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//  board1.squares[0][2] = MINIMAX_PLAYER_SQUARE;
//  board1.squares[1][0] = MINIMAX_PLAYER_SQUARE;
//  board1.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//  board1.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board1.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//  board1.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
//  board1.squares[2][2] = MINIMAX_OPPONENT_SQUARE;
//
//  minimax_board_t board2;
//  board2.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//  board2.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//  board2.squares[0][2] = MINIMAX_PLAYER_SQUARE;
//  board2.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//  board2.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//  board2.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board2.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//  board2.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//  board2.squares[2][2] = MINIMAX_OPPONENT_SQUARE;
//
//  minimax_board_t board3;
//  board3.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//  board3.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//  board3.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//  board3.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
//  board3.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//  board3.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board3.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//  board3.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//  board3.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//  minimax_board_t board4;
//  board4.squares[0][0] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[0][2] = MINIMAX_PLAYER_SQUARE;
//  board4.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[2][0] = MINIMAX_OPPONENT_SQUARE;
//  board4.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//  board4.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//
//  minimax_board_t board5;
//  board5.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//  board5.squares[0][1] = MINIMAX_PLAYER_SQUARE;
//  board5.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//  board5.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//  board5.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
//  board5.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board5.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//  board5.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//  board5.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//  minimax_board_t board6;
//  board6.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//  board6.squares[0][1] = MINIMAX_PLAYER_SQUARE;
//  board6.squares[0][2] = MINIMAX_OPPONENT_SQUARE;
//  board6.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
//  board6.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
//  board6.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board6.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//  board6.squares[2][1] = MINIMAX_PLAYER_SQUARE;
//  board6.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//  minimax_board_t board7;
//  board7.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//  board7.squares[0][1] = MINIMAX_PLAYER_SQUARE;
//  board7.squares[0][2] = MINIMAX_OPPONENT_SQUARE;
//  board7.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
//  board7.squares[1][1] = MINIMAX_PLAYER_SQUARE;
//  board7.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//  board7.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//  board7.squares[2][1] = MINIMAX_PLAYER_SQUARE;
//  board7.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
// uint8_t row, column;
//
// minimax_computeNextMove(&board1, true, &row, &column);
// printf("next move for board1: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board2, true, &row, &column);
// printf("next move for board2: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board3, true, &row, &column);
// printf("next move for board3: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board4, false, &row, &column);
// printf("next move for board4: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board5, false, &row, &column);
// printf("next move for board5: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board6, false, &row, &column);
// printf("next move for board6: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// minimax_computeNextMove(&board7, false, &row, &column);
// printf("next move for board7: (%d, %d)\n\r", row, column);
// printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
// return 0;


// 	MILESTONE 3

	leds_init(true); //init the LEDs so that LD4 can function as a heartbeat.
    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
    u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
    printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
    // Allow the timer to generate interrupts.
    interrupts_enableTimerGlobalInts();
    // Initialization of the clock display is not time-dependent, do it outside of the state machine.
    //ticTacToeDisplay_init();
    // Keep track of your personal interrupt count. Want to make sure that you don't miss any interrupts.
    int32_t personalInterruptCount = 0;
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
	   	   personalInterruptCount++; //count ticks.
	   	   ticTacToeControl_tick();
	   	   interrupts_isrFlagGlobal = 0;
	   }
   }
   interrupts_disableArmInts();
   printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
   printf("internal interrupt count: %ld\n\r", personalInterruptCount);
}
