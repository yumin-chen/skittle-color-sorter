/*
  Context
*/
#ifndef  _CONTEXT_H_
#define  _CONTEXT_H_

#include "C_Color.h"

#define SKITTLE_COLORS 5 // Types of colors of Skittles

// Calibrated Skittle's Colors
// These are the skittle's colors
#define C_COLOR_EMPTY		C_Color(66, 89, 71, 170)	/* Empty hole		*/
#define C_SKITTLE_RED		C_Color(97, 73, 62, 117)	/* Red Skittle		*/
#define C_SKITTLE_GREEN		C_Color(58, 106, 54, 148)	/* Green Skittle	*/
#define C_SKITTLE_YELLOW	C_Color(90, 90, 34, 210)	/* Yellow Skittle	*/
#define C_SKITTLE_PURPLE	C_Color(65, 84, 81, 122)	/* Purple Skittle	*/
#define C_SKITTLE_ORANGE	C_Color(122, 64, 40, 154) 	/* Orange Skittle	*/

typedef enum : uint8_t
{
	RESULT_RED				= 0x00,		/* Red Skittle		*/
	RESULT_GREEN			= 0x01,		/* Green Skittle	*/
	RESULT_YELLOW			= 0x02,		/* Yellow Skittle	*/
	RESULT_PURPLE			= 0x03,		/* Purple Skittle	*/
	RESULT_ORANGE			= 0x04,		/* Orange Skittle	*/
	RESULT_UNKNOWN			= 0x7F		/* Unknown Color	*/
} colorResult;

#define HAS_RESULT(result) (result != RESULT_UNKNOWN)

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

	static const C_Color colorList [SKITTLE_COLORS];
	static int skittleCount; // The number of skittles sorted
	static unsigned long lastSkittleTime; // Time when the last skittle was being sorted
	static boolean isColorBeingMeasured; // Is the color sensor measuring a Skittle right now
	static colorResult colorResults [128]; // An array to store all the detected colors;
private:
};

#endif // _CONTEXT_H_ 
