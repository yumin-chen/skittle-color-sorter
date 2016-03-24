/*
  Top Servo

  This class defines the continuous rotation servo at the top

*/

// The circuit:
// * Top Servo
//    Digital pin 13
#define PIN_TOP_SERVO 13

#define F_TOP_SERVO_TIMEOUT_ENABLED true // Enable the timeout for the top servo
#define C_TOP_SERVO_TIMEOUT 6000 // Timeout to determine if the top servo is stuck
#define C_RETRY_UNKNOWN_COLOR 0 // If the color cannot be detected, how many times should retry. If 0, no retry.

#include "TopServo.h"

TopServo::TopServo(){
#if F_TOP_SERVO_TIMEOUT_ENABLED
	bJammed = false;
#endif
	bRemeasuring = false;
	bRemDir = false;
	iRetry = 0;
}

void TopServo::setup(){
	/* Attaches the corresponding pins to the servo object.
	*/
	this->attach(PIN_TOP_SERVO);
}

void TopServo::update(unsigned long lastSkittleTime){
	/* This function should be called on the main loop. 
	*/
	boolean dirction = false; // Set direction to forwards

	// Check if the color is being re-measured
	if (bRemeasuring) dirction = bRemDir;

#if F_TOP_SERVO_TIMEOUT_ENABLED
	// If jammed, reverse direction
	if (bJammed) dirction = !dirction; 
#endif

	// Set the speed of the top servo
	// with 0 being full-speed in one direction, 180 being full speed in the other, and a value near 90 being no movement.
	this->write(dirction ? 180 : 0);

#if F_TOP_SERVO_TIMEOUT_ENABLED
	static unsigned long lastStuckTime = 0;
	// If the top servo gets stuck
	if (!bJammed && millis() - lastSkittleTime > C_TOP_SERVO_TIMEOUT && millis() - lastStuckTime > 1000) {
		Serial.println("Top servo is stuck; Direction reversed.");
		// Reserve the top servo's direction
		bJammed = true;

		lastStuckTime = millis();
	}

	if (bJammed && millis() - lastStuckTime > 1000) {
		// Set direction back to normal after 1s
		bJammed = false;
	}
#endif
}

void TopServo::remeasureColor(){
	/* Starts re-measuring the Skittle's color if the retried times is less than C_RETRY_UNKNOWN_COLOR
	*/
	if(iRetry < C_RETRY_UNKNOWN_COLOR){
		iRetry++;
		bRemeasuring = true;
		Serial.println("Color cannot be determined. Start going back and re-measuring it.");
	}else{
		Serial.println("Color cannot be determined.");
	}

	if (bRemeasuring) {
		bRemDir = !bRemDir;
		Serial.print("The top servo is now going ");
		Serial.println(bRemDir ? "backwards" : "forwards");
		if (iRetry >= C_RETRY_UNKNOWN_COLOR) {
			// If tried many times still can't get the color
			// Just f*ck it cuz we ain't gonna get color however we try
			Serial.println("Tried re-measuring but still couldn't get the color.");
			stopRemeasuring();
		}
	}
}

void TopServo::stopRemeasuring(){
	/* Stops re-measuring the Skittle's color
	*/
	bRemeasuring = false;
	bRemDir = false;
	iRetry = 0;
}

boolean TopServo::isRemeasuring(){
	/* Returns true if the top servo is re-measuring the Skittle's color
	*/
	return bRemeasuring;
}