/*
 * buttons.h
 *
 *  Created on: Sep 11, 2014
 *      Author: tchambs
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "supportFiles/display.h"
#include "supportFiles/arduinoTypes.h"

// Initializes the button driver software and hardware. Returns one of the defined status values (above).
int buttons_init();

// Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read();

//draws a rectangle on the screen in a defined area and writes the value of the button
void buttons_lcd_write(int32_t);

// Runs a test of the buttons. Each button pressed will display a rectangle of color and the number of
// the button being pressed. Test terminates when all 4 buttons have been pressed
void buttons_runTest();

// Helper function to read GPIO registers.
int32_t buttons_readGpioRegister(int32_t);

#endif /* BUTTONS_H_ */
