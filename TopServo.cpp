/**************************************************************************/
/**
    Top Servo

    @file     TopServo.cpp
    @author   Charlie Chen <Charlie@CharmySoft.com>

    @section  Circuit
    - Top Servo
      + Digital pin 13
*/
/**************************************************************************/
#define PIN_TOP_SERVO       13    /**< The pin attaching to the top servo */

/** Enable the timeout for the top servo. If enabled, the top servo will
    move backwards when the color sensor's measuring cycle is longer than
    the defined TIMEOUT value. This is used to solve jamming issues as
    the Skittles get stuck in the top servo. */
#define F_TOP_SERVO_TIMEOUT_ENABLED true
#define C_TOP_SERVO_TIMEOUT         6000  /**< Timeout value in milliseconds */

/** How many times to retry if the color cannot be determined by the color
    sensor. If set to 0, then this will be disabled. */
#define C_RETRY_UNKNOWN_COLOR       1

#include "TopServo.h"
#include "ColorSensor.h"

TopServo::TopServo()
{
#if F_TOP_SERVO_TIMEOUT_ENABLED
  bJammed = false;
#endif
  bRemeasuring = false;
  bRemDir = false;
  iRetry = 0;
}

void TopServo::setup()
{
  this->attach(PIN_TOP_SERVO);
}

void TopServo::update()
{
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
  if (!bJammed && millis() - colorSensor.getLastSkittleTime() > C_TOP_SERVO_TIMEOUT
      && millis() - lastStuckTime > 1000) {
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

void TopServo::remeasureColor()
{
  if (iRetry < C_RETRY_UNKNOWN_COLOR) {
    iRetry++;
    bRemeasuring = true;
    Serial.println("Color cannot be determined. Start going back and re-measuring it.");
  } else {
    Serial.println("Color cannot be determined.");
  }

  if (bRemeasuring) {
    bRemDir = !bRemDir; // Reverse the remeasuring direction
    Serial.print("The top servo is now going ");
    Serial.println(bRemDir ? "backwards" : "forwards");
    if (iRetry >= C_RETRY_UNKNOWN_COLOR) {
      // If tried too many times and still can't get the color
      // Just f*ck it cuz we ain't gonna get color however we try
      Serial.println("Tried re-measuring but still couldn't get the color.");
      stopRemeasuring();
    }
  }
}

void TopServo::stopRemeasuring() {
  bRemeasuring = false;
  bRemDir = false;
  iRetry = 0;
}

boolean TopServo::isRemeasuring() {
  return bRemeasuring;
}
