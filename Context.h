/*
  Context
*/
#ifndef  _CONTEXT_H_
#define  _CONTEXT_H_

#include "C_Color.h"

#define SKITTLE_COLORS 5 // Types of colors of Skittles

// Calibrated Skittle's Colors
// These are the skittle's colors
#define C_COLOR_EMPTY C_Color(66, 89, 71, 170)
#define C_SKITTLE_RED C_Color(97, 73, 62, 117)
#define C_SKITTLE_GREEN C_Color(58, 106, 54, 148)
#define C_SKITTLE_YELLOW C_Color(90, 90, 34, 210)
#define C_SKITTLE_PURPLE C_Color(65, 84, 81, 122)
#define C_SKITTLE_ORANGE C_Color(122, 64, 40, 154) 

#define RESULT_UNKNOWN 255
#define HAS_RESULT(color_index) (color_index >= 0 && color_index < SKITTLE_COLORS)

class Context{
public:
	static const C_Color colorList [SKITTLE_COLORS];
	static int skittleCount; // The number of skittles sorted
	static unsigned long lastSkittleTime; // Time when the last skittle was being sorted
	static boolean isColorBeingMeasured; // Is the color sensor measuring a Skittle right now
	static byte colorResults [256]; // An array to store all the detected colors;
private:
};

#endif // _CONTEXT_H_ 
