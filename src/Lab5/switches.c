/*
 * switches.c
 *
 *  Created on: Sep 11, 2014
 *      Author: tchambs
 */

#include "xil_io.h"
#include "supportFiles/leds.h"

#define SWITCHES_INIT_STATUS_OK 1
#define SWITCHES_INIT_STATUS_FAIL 0
#define XPAR_GPIO_SLIDE_SWITCHES_BASEADDR 0x41280000
#define SWITCHES_GPIO_DEVICE_BASE_ADDRESS XPAR_GPIO_SLIDE_SWITCHES_BASEADDR
#define ALLSWITCHESUP 0x0F




// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int switches_init()
{
	//0x0F configures these as INPUTS
	Xil_Out32(SWITCHES_GPIO_DEVICE_BASE_ADDRESS + 4, 0x0F);

	return SWITCHES_INIT_STATUS_OK;
}

// Returns the current value of all 4 SWITCHESs as the lower 4 bits of the returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read()
{
	//read the value at the specified address -- SWITCHES_GPIO_DEVICE_BASE_ADDRESS
	u32 switchesValues = Xil_In32(SWITCHES_GPIO_DEVICE_BASE_ADDRESS);

	//cast switches value to an in32_t and return
	return (int32_t)switchesValues;
}

// Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
// The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
// this function will return.
void switches_runTest()
{

	switches_init(); //initialize switches

	leds_init(1); //initialize LEDs - 1 print debug statements, 0 don't print debug statements
	int32_t value = 0;

	//read the switches and write to the LEDs as long as all switches aren't on
	do {
		value = switches_read();
		leds_write(value);
	} while (value != ALLSWITCHESUP);


	leds_write(0x00); //turn off LEDs when we're done
}
