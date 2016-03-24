/*
  Bottom Servo

  This class defines the standard servo at the bottom

*/

// The circuit:
// * Bottom Servo (Arm)
//    Digital pin 10
#define PIN_BTM_SERVO 10

#include "BottomServo.h"

BottomServo::BottomServo(){
}

void BottomServo::setup(){
	/* Attaches the corresponding pins to the servo object.
	*/
	this->attach(PIN_BTM_SERVO);
}

void BottomServo::update(){
	/* This function should be called on the main loop. 
	*/

  // Calculate the angle of the bottom servo arm
#define MAX_BTM_ANGLE 150 // Maximum bottom servo angle
#define MIN_BTM_ANGLE 30 // Minimum bottom servo angle
#define NUM_OF_CUPS 5 // The number of cups at the bottom
#define EACH_CUP_ANGLE_INTERVAL ((MAX_BTM_ANGLE - MIN_BTM_ANGLE) / NUM_OF_CUPS)
	int index = skittleCount;
	int result = 2; // Set default result to 2 so the bottom arm is centered
	while(index >= 0){
		// Check if the current Skittle has a color result
		if(HAS_RESULT(colorResults[index])){
			result = colorResults[index];
			break;
		}
		// Go to the previous Skittle when not
		index --;
	}
	int btmAngle = MIN_BTM_ANGLE + EACH_CUP_ANGLE_INTERVAL * result;
	this->write(btmAngle);
}


