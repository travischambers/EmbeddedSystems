/*
 * buttons.c
 *
 *  Created on: Sep 11, 2014
 *      Author: tchambs
 */

#include "supportFiles/arduinoTypes.h"
#include "supportFiles/leds.h"
#include "supportFiles/display.h"

#define BUTTONS_INIT_STATUS_OK 1
#define BUTTONS_INIT_STATUS_FAIL 0
#define XPAR_GPIO_PUSH_BUTTONS_BASEADDR 0x41240000
#define BUTTONS_GPIO_DEVICE_BASE_ADDRESS XPAR_GPIO_PUSH_BUTTONS_BASEADDR
#define ALLBUTTONSPRESSED 0x0F

//constants for convenience when working with sections of the LCD
uint16_t XFOURTH = display_width()/4;
uint16_t XHALF = display_width()/2;
uint16_t XTHREEFOURTHS = (display_width() * 3) / 4;
uint16_t YFOURTH = display_height()/4;
uint16_t YHALF = display_height()/2;
uint16_t YTHREEFOURTHS = (display_height() * 3) / 4;

// Helper function to read GPIO registers.
int32_t buttons_readGpioRegister(int32_t offset) {
  // Note that you have to include a cast (uint32_t *) to keep the compiler happy.
  uint32_t *ptr = (uint32_t *) BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset;
  return *ptr;
}

// Initializes the button driver software and hardware. Returns one of the defined status values (above).
int buttons_init()
{
	//write all 1s to the tri-state driver to configure my buttons for inputs
	uint32_t *ptr = (uint32_t *) BUTTONS_GPIO_DEVICE_BASE_ADDRESS + 4;
	*ptr = 0x0F;

	return 1;
}

// Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read()
{
	int32_t registerValue = buttons_readGpioRegister(0);

	return registerValue;
}

void buttons_lcd_write(int32_t buttonsPressed)
{



	if (buttonsPressed & 0x08)
	{
		//to get rid of flashing text, I could figure out a way to check the screen color and only call display_fillRect if its NOT already blue
		display_fillRect(0, 0, XHALF, YHALF, DISPLAY_BLUE);
		display_setCursor(XFOURTH, YFOURTH); //top left corner - in portrait mode this makes sense
		display_println("BTN 1"); //in portrait mode the top button is actually button 0x08, but i'm labeling it as button 1 for simplicity
	}
	else
	{
		display_fillRect(0, 0, XHALF, YHALF, DISPLAY_WHITE); //blank the LCD area for this button
	}
	if (buttonsPressed & 0x04)
	{
		//to get rid of flashing text, I could figure out a way to check the screen color and only call display_fillRect if its NOT already green
		display_fillRect(XHALF, 0, display_width(), YHALF, DISPLAY_GREEN);
		display_setCursor(XTHREEFOURTHS, YFOURTH); //top left corner
		display_println("BTN 2"); //second button down, labeled as button 2
	}
	else
	{
		display_fillRect(XHALF, 0, display_width(), YHALF, DISPLAY_WHITE); //blank the LCD area for this button
	}
	if (buttonsPressed & 0x02)
	{
		//to get rid of flashing text, I could figure out a way to check the screen color and only call display_fillRect if its NOT already red
		display_fillRect(0, YHALF, XHALF, display_height(), DISPLAY_RED);
		display_setCursor(XFOURTH, YTHREEFOURTHS); //bottom left corner
		display_println("BTN 3"); //third button down, labeled as button 3
	}
	else
	{
		display_fillRect(0, YHALF, XHALF, display_height(), DISPLAY_WHITE); //blank the LCD area for this button
	}
	if (buttonsPressed & 0x01)
	{
		//to get rid of flashing text, I could figure out a way to check the screen color and only call display_fillRect if its NOT already yellow
		display_fillRect(XHALF, YHALF, display_width(), display_height(), DISPLAY_YELLOW);
		display_setCursor(XTHREEFOURTHS, YTHREEFOURTHS); //bottom right corner
		display_println("BTN 4"); //last button, labeled as button 4
	}
	else
	{
		display_fillRect(XHALF, YHALF, display_width(), display_height(), DISPLAY_WHITE); //blank the LCD area for this button
	}

}

// Runs a test of the buttons. As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest()
{
	buttons_init();
	int32_t value = 0;
	display_init();  // Must init all of the software and underlying hardware for LCD.
	display_setRotation(0); //puts the display in portrait mode
	display_fillScreen(DISPLAY_WHITE); //change the screen to WHITE to show we're ready to begin the test
	//display_testFillScreen();  // flash colors on the screen.

	do {
		value = buttons_read();
		lcd_write(value);
	} while (value != ALLBUTTONSPRESSED);

	display_fillScreen(DISPLAY_GREEN); //show we're done with the test by changing the screen to green
}
