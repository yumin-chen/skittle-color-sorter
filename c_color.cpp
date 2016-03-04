/*
  C_Color

  This class defines some of the color operations for the Skittle Color Sorter

*/
#include "C_Color.h"

C_Color::C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, COLOR_UNIT clear){
	/* Initializes a C_Color object with defined rgbc values.
		*/
	r = red;
	g = green;
	b = blue;
	c = clear;
}

C_Color C_Color::createFromRawColors(int cycles, uint16_t red, uint16_t green, uint16_t blue, uint16_t clear){
	/* Returns a C_Color object created from raw colors, with color values converted to 8-bits wide.
	*/
	return C_Color(red / cycles, green / cycles, blue / cycles, clear / cycles);
}

C_Color C_Color::compare(C_Color compared_color){
	/* Compares the color with another color and returns the difference between them
	*/
	COLOR_UNIT color_diff_r = abs(int(r) - int(compared_color.r));
	COLOR_UNIT color_diff_g = abs(int(g) - int(compared_color.g));
	COLOR_UNIT color_diff_b = abs(int(b) - int(compared_color.b));
	return C_Color(color_diff_r, color_diff_g, color_diff_b);
}

int C_Color::aggregate(){
	/* Returns the sum of all primary color elements.
	*/
	return r + g + b;
}


void C_Color::maximize(){
	/* Maximizes color based on the clear value.
	*/
	float scale = 256.0 / c;
  	r *= scale; g *= scale; b *= scale;
}


void C_Color::print(){
	/* Prints color values.
	*/
	Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
	Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
	Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
	Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
	Serial.println(" ");
}
