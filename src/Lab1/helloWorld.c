// Print out "hello world" on both the console and the LCD screen.

#include <stdio.h>
#include "supportFiles/display.h"
int main() {
	display_init();  // Must init all of the software and underlying hardware for LCD.
	display_fillScreen(DISPLAY_BLACK);  // Blank the screen.


	//uint16_t DISPLAY_WIDTH = display_width();
	//uint16_t DISPLAY_HEIGHT = display_height();

	display_setRotation(0);

	//display_drawLine(x0, y0, x1, y1, color);
	display_drawLine(0, 0, display_width(), display_height(), DISPLAY_GREEN);
	display_drawLine(0, display_height(), display_width(), 0, DISPLAY_GREEN);

	//size constants
	uint16_t XFourth = display_width()/4;
	uint16_t XHalf = display_width()/2;
	uint16_t XThreeFourths = (display_width() * 3) / 4;
	uint16_t YFourth = display_height()/4;
	uint16_t YHalf = display_height()/2;
	uint16_t YThreeFourths = (display_height() * 3) / 4;

	//drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
	display_drawCircle(XHalf, YFourth, 25, DISPLAY_RED);
	display_drawCircle(XHalf, YThreeFourths, 25, DISPLAY_RED);
	display_fillCircle(XHalf, YThreeFourths, 25, DISPLAY_RED);

	//drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	display_drawTriangle(XFourth - 24, YHalf - 24, XFourth - 24, YHalf + 24, XFourth + 24, YHalf, DISPLAY_YELLOW);
	display_drawTriangle(XThreeFourths + 24, YHalf - 24, XThreeFourths + 24, YHalf + 24, XThreeFourths - 24, YHalf, DISPLAY_YELLOW);
	display_fillTriangle(XThreeFourths + 24, YHalf - 24, XThreeFourths + 24, YHalf + 24, XThreeFourths - 24, YHalf, DISPLAY_YELLOW);

}
