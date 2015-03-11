/*
 * main.c
 *
 *  Created on: Sep 23, 2014
 *      Author: tchambs
 */

#include "intervalTimer.h"
#include "xil_io.h"
#include <stdio.h>


#define XPAR_AXI_TIMER_0_BASEADDR 0x42800000
#define XPAR_AXI_TIMER_1_BASEADDR 0x42840000
#define XPAR_AXI_TIMER_2_BASEADDR 0x42880000
#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x18

//code used for milestone 1

u32 readTimer1Register(uint32_t registerOffset) {
	uint32_t address = XPAR_AXI_TIMER_0_BASEADDR + registerOffset;
	return Xil_In32(address);
}

u32 readTimer2Register(uint32_t registerOffset) {
	uint32_t address = XPAR_AXI_TIMER_1_BASEADDR + registerOffset;
	return Xil_In32(address);
}

u32 readTimer3Register(uint32_t registerOffset) {
	uint32_t address = XPAR_AXI_TIMER_2_BASEADDR + registerOffset;
	return Xil_In32(address);
}

#define DELAY_COUNT 3
void waitALongTime() {
	volatile int32_t a = 0;
	int32_t i, j;
	for (i=0; i<DELAY_COUNT; i++)
		for (j=0; j<INT32_MAX; j++)
			a++;
}

int main() {

  intervalTimer_initAll(); //initialize all timers
  intervalTimer_resetAll(); //reset all the timers to 0

  // Show that the timer is reset.
  printf("timer_0 TCR0 should be 0 at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR1 should be 0 at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));

  printf("timer_1 TCR0 should be 0 at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR1 should be 0 at this point:%ld\n\r", readTimer2Register(TCR1_OFFSET));

  printf("timer_2 TCR0 should be 0 at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR1 should be 0 at this point:%ld\n\r", readTimer3Register(TCR1_OFFSET));

  //start all three timers
  intervalTimer_start(0);
  intervalTimer_start(1);
  intervalTimer_start(2);

  // Show that the timer is running.
  printf("The following register values should be changing while reading them.\n\r");
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));

  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));

  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));

  //stop all three timers
  intervalTimer_stop(0);
  intervalTimer_stop(1);
  intervalTimer_stop(2);

  printf("The following register values should be changing while reading them.\n\r");
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));

  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));

  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer3Register(TCR0_OFFSET));

  intervalTimer_start(0);
  intervalTimer_start(1);
  intervalTimer_start(2);

  // Wait about 2 minutes so that you roll over to TCR1.
  // If you don't see a '1' in TCR1 after this long wait you probably haven't programmed the timer correctly.
  waitALongTime();
  printf("timer_0 TCR0 value after wait:%lx\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR1 should have changed at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));

  printf("timer_1 TCR0 value after wait:%lx\n\r", readTimer2Register(TCR0_OFFSET));
  printf("timer_1 TCR1 should have changed at this point:%ld\n\r", readTimer2Register(TCR1_OFFSET));

  printf("timer_2 TCR0 value after wait:%lx\n\r", readTimer3Register(TCR0_OFFSET));
  printf("timer_2 TCR1 should have changed at this point:%ld\n\r", readTimer3Register(TCR1_OFFSET));
}
