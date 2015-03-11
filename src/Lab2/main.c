/*
 * main.c
 *
 *  Created on: Sep 11, 2014
 *      Author: tchambs
 */

#include <stdio.h>
#include "supportFiles/leds.h"
#include "buttons.h"
#include "switches.h"

int main()
{
	leds_runTest();

	//continuously loop through buttons and switches
	while(1)
	{
		//run the buttons test
		buttons_runTest();
		//run the switches test
		switches_runTest();
	}
}
