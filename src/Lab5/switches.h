/*
 * switches.h
 *
 *  Created on: Sep 11, 2014
 *      Author: tchambs
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_
#include "supportFiles/display.h"
#include "supportFiles/arduinoTypes.h"

// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int switches_init();

// Returns the current value of all 4 SWITCHESs as the lower 4 bits of the returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read();

// Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
// The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
// this function will return.
void switches_runTest();

int32_t switches_writeGpioRegister(int32_t);
int32_t switches_readGpioRegister(int32_t);




#endif /* SWITCHES_H_ */
