/*
  Context
*/
#ifndef  _CONTEXT_H_
#define  _CONTEXT_H_

#include "C_Color.h"

// Calibrated Skittle's Colors
// These are the skittle's colors
#define COLORS_DEFINITION \
	/* Put your valid colors first*/\
	COLOR_DEF( RESULT_RED,		"Red Skittle",		C_Color(97, 73, 62, 117) ),		/* Red Skittle		*/\
	COLOR_DEF( RESULT_GREEN,	"Green Skittle",	C_Color(58, 106, 54, 148) ),	/* Green Skittle	*/\
	COLOR_DEF( RESULT_YELLOW,	"Yellow Skittle",	C_Color(90, 90, 34, 210) ),		/* Yellow Skittle	*/\
	COLOR_DEF( RESULT_PURPLE,	"Purple Skittle",	C_Color(65, 84, 81, 122) ),		/* Purple Skittle	*/\
	COLOR_DEF( RESULT_ORANGE,	"Orange Skittle",	C_Color(122, 64, 40, 154) ),	/* Orange Skittle	*/\
	/* Don't forget to exclude these invalid colors in the HAS_COLOR macro function */\
	COLOR_DEF( RESULT_EMPTY,	"Empty hole",		C_Color(66, 89, 71, 170) )		/* Empty hole		*/

// Use macro to generate the colorResult enum
#define COLOR_DEF( identifier, name, color )  identifier
typedef enum : uint8_t 
{ 
	COLORS_DEFINITION,
	RESULT_UNKNOWN			/* Add a special key for unknown colors */
} colorResult;
#undef COLOR_DEF

#define COLOR_LIST_SIZE RESULT_UNKNOWN 

// Define a macro function to check if the result is a valid color
#define HAS_RESULT(result) (result != RESULT_UNKNOWN && result != RESULT_EMPTY)

// Declare some of the components
class LCD;
class TopServo;
class BottomServo;
class ColorView;
class ColorSensor;

class Context{
public:
	static LCD lcd; 
	static TopServo servoTop; 
	static BottomServo servoBtm; 
	static ColorView colorView;
	static ColorSensor colorSensor; 

	static const C_Color colorList [];
	static int skittleCount; // The number of skittles sorted
	static unsigned long lastSkittleTime; // Time when the last skittle was being sorted
	static boolean isColorBeingMeasured; // Is the color sensor measuring a Skittle right now
	static colorResult colorResults [128]; // An array to store all the detected colors;
private:
};

#endif // _CONTEXT_H_ 
