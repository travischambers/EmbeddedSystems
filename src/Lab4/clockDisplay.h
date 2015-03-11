/*
 * clockDisplay.h
 *
 *  Created on: Oct 7, 2014
 *      Author: tchambs
 */

#ifndef CLOCKDISPLAY_H_
#define CLOCKDISPLAY_H_

#include <stdbool.h>

void clockDisplay_init();  // Called only once - performs any necessary inits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll);  // Updates the time display with latest time.
void clockDisplay_performIncDec();         // Performs the increment or decrement, depending upon the touched region.
void clockDisplay_advanceTimeOneSecond();  // Advances the time forward by 1 second.
void clockDisplay_runTest();               // Run a test of clock-display functions.

#endif /* CLOCKDISPLAY_H_ */
