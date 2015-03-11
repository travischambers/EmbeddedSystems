/*
 * intervalTimer.c
 *
 *  Created on: Sep 23, 2014
 *      Author: tchambs
 */

#include "xil_io.h"
#include <stdio.h>
#include "supportFiles/arduinoTypes.h"
#include "buttons.h"

#define XPAR_AXI_TIMER_0_BASEADDR 0x42800000
#define XPAR_AXI_TIMER_1_BASEADDR 0x42840000
#define XPAR_AXI_TIMER_2_BASEADDR 0x42880000
#define TCSR0_OFFSET 0x00
#define TLR0_OFFSET 0x04
#define TCR0_OFFSET 0x08
#define TCSR1_OFFSET 0x10
#define TLR1_OFFSET 0x14
#define TCR1_OFFSET 0x18
#define XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ 100000000
#define XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ 100000000
#define XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ 100000000

#define SET_CASC_BIT 0x0800
#define ENABLE_ENT0_BIT 0x080
#define CLEAR_ENT0_BIT 0xF7F
#define ZERO 0x00
#define ENABLE_LOAD0_BIT 0x020
#define LEFT_SHIFT_BITS 32

// Helper functions for run_test() and testAll()
u32 readTimer0Register(uint32_t registerOffset)
{
	uint32_t address = XPAR_AXI_TIMER_0_BASEADDR + registerOffset;
	return Xil_In32(address);
}

u32 readTimer1Register(uint32_t registerOffset)
{
	uint32_t address = XPAR_AXI_TIMER_1_BASEADDR + registerOffset;
	return Xil_In32(address);
}

u32 readTimer2Register(uint32_t registerOffset)
{
	uint32_t address = XPAR_AXI_TIMER_2_BASEADDR + registerOffset;
	return Xil_In32(address);
}

uint32_t intervalTimer_start(uint32_t timerNumber) //timerNumber refers to WHICH timer (possible values: 0, 1, 2)
{
	if (timerNumber == 0)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET); //find the current value of the timer
		uint32_t inputValue = timer_Val | ENABLE_ENT0_BIT; //enable the ENT0 bit
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, inputValue); //write new value to the register
	}
	else if (timerNumber == 1)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET); //find the current value of the timer
		uint32_t inputValue = timer_Val | ENABLE_ENT0_BIT; //enable the ENT0 bit
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, inputValue); //write new value to the register
	}
	else if (timerNumber == 2)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET); //find the current value of the timer
		uint32_t inputValue = timer_Val | ENABLE_ENT0_BIT; //enable the ENT0 bit
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, inputValue); //write new value to the register
	}
	else
		printf("Error. Invalid timerNumber");

	return 0;
}
uint32_t intervalTimer_stop(uint32_t timerNumber)
{
	if (timerNumber == 0)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET); //read value in TCSR0
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, (timer_Val & CLEAR_ENT0_BIT)); // write a 0 to the ENT0 bit of TCSR0
	}
	else if (timerNumber == 1)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET); //read value in TCSR0
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, (timer_Val & CLEAR_ENT0_BIT)); //write a 0 to the ENT0 bit of TCSR0
	}
	else if (timerNumber == 2)
	{
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET); //read value in TCSR0
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, (timer_Val & CLEAR_ENT0_BIT)); //write a 0 to the ENT0 bit of TCSR0
	}
	else
		printf("Error. Invalid timerNumber");

	return 0;
}

uint32_t intervalTimer_init(uint32_t timerNumber)
{
	if (timerNumber == 0) //init timer_0
	{
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
		uint32_t timer_0Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET);
		timer_0Val = timer_0Val | SET_CASC_BIT;
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register
	}
	else if (timerNumber == 1) //init timer_1
	{
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
		uint32_t timer_0Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET);
		timer_0Val = timer_0Val | SET_CASC_BIT;
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register
	}
	else if (timerNumber == 2) //init timer_2
	{
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
		uint32_t timer_0Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET);
		timer_0Val = timer_0Val | SET_CASC_BIT;
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register
	}
	else
		printf("Error. Invalid timerNumber");

	return 0;
}
uint32_t intervalTimer_reset(uint32_t timerNumber)
{
	if (timerNumber == 0) //reset timer_0
	{
		//store a 0 in counter 0
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TLR0_OFFSET, ZERO);
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, timer_Val);

		//store a 0 in counter 1
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TLR1_OFFSET, ZERO);
		timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET, timer_Val);

	}
	else if (timerNumber == 1) //reset timer_1
	{
		//store a 0 in counter 0
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TLR0_OFFSET, ZERO);
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, timer_Val);

		//store a 0 in counter 1
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TLR1_OFFSET, ZERO);
		timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET, timer_Val);
	}
	else if (timerNumber == 2) // reset timer_2
	{
		//store a 0 in counter 0
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TLR0_OFFSET, ZERO);
		uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, timer_Val);

		//store a 0 in counter 1
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TLR1_OFFSET, ZERO);
		timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET);
		timer_Val = timer_Val | ENABLE_LOAD0_BIT; //enable LOAD0 bit
		Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET, timer_Val);

	}
	else
		printf("Error. Invalid timerNumber");

	intervalTimer_init(timerNumber);

	return 0;
}
uint32_t intervalTimer_initAll()
{
	//init timer_0
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
	uint32_t timer_0Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET);
	timer_0Val = timer_0Val | SET_CASC_BIT;
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register

	//init timer_1
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
	timer_0Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET);
	timer_0Val = timer_0Val | SET_CASC_BIT;
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register

	//init timer_2
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, ZERO); //write a 0 to the TCSR0 register.
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET, ZERO); //write a 0 to the TCSR1 register.
	timer_0Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET);
	timer_0Val = timer_0Val | SET_CASC_BIT;
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, timer_0Val); //set CASC bit and clear UDT0 bit in TCSR0 register

	return 0;
}
uint32_t intervalTimer_resetAll()
{
	//reset timer_0
	//store a 0 in counter 0
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TLR0_OFFSET, ZERO);
	uint32_t timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR0_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	//store a 0 in counter 1
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TLR1_OFFSET, ZERO);
	timer_Val = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + TCSR1_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	//reset timer_1
	//store a 0 in counter 0
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TLR0_OFFSET, ZERO);
	timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR0_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	//store a 0 in counter 1
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TLR1_OFFSET, ZERO);
	timer_Val = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + TCSR1_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	//reset timer_2
	//store a 0 in counter 0
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TLR0_OFFSET, ZERO);
	timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR0_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	//store a 0 in counter 1
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TLR1_OFFSET, ZERO);
	timer_Val = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET);
	Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + TCSR1_OFFSET, timer_Val | ENABLE_LOAD0_BIT); //enable LOAD0 bit

	intervalTimer_initAll();

	return 0;
}
uint32_t intervalTimer_testAll()
{
	  intervalTimer_initAll(); //initialize all timers
	  intervalTimer_resetAll(); //reset all the timers to 0

	  // Show that the timer is reset.
	  printf("timer_0 TCR0 should be 0 at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR1 should be 0 at this point:%ld\n\r", readTimer0Register(TCR1_OFFSET));

	  printf("timer_1 TCR0 should be 0 at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR1 should be 0 at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));

	  printf("timer_2 TCR0 should be 0 at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR1 should be 0 at this point:%ld\n\r", readTimer2Register(TCR1_OFFSET));

	  //start all three timers
	  intervalTimer_start(0);
	  intervalTimer_start(1);
	  intervalTimer_start(2);

	  // Show that the timer is running.
	  printf("The following register values should be changing while reading them.\n\r");
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));

	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));

	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));

	  //stop all three timers
	  intervalTimer_stop(0);
	  intervalTimer_stop(1);
	  intervalTimer_stop(2);

	  printf("The following register values should be stagnant while reading them.\n\r");
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));

	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));

	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));

	  //start timers again
	  intervalTimer_start(0);
	  intervalTimer_start(1);
	  intervalTimer_start(2);

	  printf("The following register values should be changing while reading them.\n\r");
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));

	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	  printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));

	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	  printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));

	return 0;
}
uint32_t intervalTimer_runTest(uint32_t timerNumber)
{
	intervalTimer_init(timerNumber);
	intervalTimer_reset(timerNumber);
	// Show that the timer is reset.
	if (timerNumber == 0)
	{
		printf("timer_0 TCR0 should be 0 at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
		printf("timer_0 TCR1 should be 0 at this point:%ld\n\r", readTimer0Register(TCR1_OFFSET));
	}
	else if (timerNumber == 1)
	{
		printf("timer_1 TCR0 should be 0 at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
		printf("timer_1 TCR1 should be 0 at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));
	}
	else if (timerNumber == 2)
	{
		printf("timer_2 TCR0 should be 0 at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
		printf("timer_2 TCR1 should be 0 at this point:%ld\n\r", readTimer2Register(TCR1_OFFSET));
	}
	else
	{
		printf("\n!!!!!!! Invalid Timer Number!!!!!!!\n");
		return 0;
	}

	intervalTimer_start(timerNumber);

	// Show that the timer is running.
	printf("The following register values should be changing while reading them.\n\r");
	if (timerNumber == 0)
	{
		printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
		printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
		printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
		printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
		printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer0Register(TCR0_OFFSET));
	}
	else if (timerNumber == 1)
	{
		printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
		printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
		printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
		printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
		printf("timer_1 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
	}
	else if (timerNumber == 2)
	{
		printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
		printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
		printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
		printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
		printf("timer_2 TCR0 should be changing at this point:%ld\n\r", readTimer2Register(TCR0_OFFSET));
	}
	else
	{
		printf("\n!!!!!!! Invalid Timer Number!!!!!!!\n");
		return 0;
	}

	return 0;
}

uint32_t intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber, double *seconds)
{
	//initialize variables to 0
	uint32_t address0 = 0;
	uint32_t address1 = 0;
	uint64_t duration1 = 0;
	uint64_t duration1_again = 0;
	uint64_t duration2 = 0;
	uint64_t fullduration = 0;

	if (timerNumber == 0)
	{
		address0 = XPAR_AXI_TIMER_0_BASEADDR + TCR1_OFFSET;
		duration1 = Xil_In32(address0);
		address1 = XPAR_AXI_TIMER_0_BASEADDR + TCR0_OFFSET;
		duration2 = Xil_In32(address1);
		duration1_again = Xil_In32(address0);
		if (duration1 != duration1_again)
			duration2 = Xil_In32(address1);
		duration1_again = duration1_again << LEFT_SHIFT_BITS; //shift duration left 32 bits
		fullduration = duration1_again + duration2; //add the bottom 32 bits to the top 32 bits
		*seconds = fullduration/XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ; //find the duration in seconds
	}
	else if (timerNumber == 1)
	{
		address0 = XPAR_AXI_TIMER_1_BASEADDR + TCR1_OFFSET;
		duration1 = Xil_In32(address0);
		address1 = XPAR_AXI_TIMER_1_BASEADDR + TCR0_OFFSET;
		duration2 = Xil_In32(address1);
		duration1_again = Xil_In32(address0);
		if (duration1 != duration1_again)
			duration2 = Xil_In32(address1);
		duration1_again = duration1_again << LEFT_SHIFT_BITS; //shift duration left 32 bits
		fullduration = duration1_again + duration2; //add the bottom 32 bits to the top 32 bits
		*seconds = fullduration/XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ; //find the duration in seconds
	}
	else if (timerNumber == 2)
	{
		address0 = XPAR_AXI_TIMER_2_BASEADDR + TCR1_OFFSET;
		duration1 = Xil_In32(address0);
		address1 = XPAR_AXI_TIMER_2_BASEADDR + TCR0_OFFSET;
		duration2 = Xil_In32(address1);
		duration1_again = Xil_In32(address0);
		if (duration1 != duration1_again) //if duration1 has changed, check duration 2 again
			duration2 = Xil_In32(address1);
		duration1_again = duration1_again << LEFT_SHIFT_BITS; //shift duration left 32 bits
		fullduration = duration1_again + duration2; //add the bottom 32 bits to the top 32 bits
		*seconds = fullduration/XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ; //find the duration in seconds
	}
	else
		printf("\n!!!!!!!Invalid TimerNumber!!!!!!!\n");

	return 0;
}
