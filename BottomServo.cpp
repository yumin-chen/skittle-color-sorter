/**************************************************************************/
/**
    Bottom Servo

    @file     BottomServo.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
    
    @section  Circuit
    - Bottom Servo
      + Digital pin 10
*/
/**************************************************************************/
#define PIN_BTM_SERVO       10    /**< The pin attaching to the bottom servo */

#include "BottomServo.h"

BottomServo::BottomServo()
{
  // Set the original arm position to the center. 
  mResult = static_cast<colorResult>(2); // 2 is the middle one. 0 1 [2] 3 4 
}

void BottomServo::setup() 
{
  this->attach(PIN_BTM_SERVO);
}

void BottomServo::update() 
{
  // Calculate the angle of the bottom servo arm
#define MAX_BTM_ANGLE   150   // Maximum bottom servo angle
#define MIN_BTM_ANGLE   30    // Minimum bottom servo angle
#define NUM_OF_CUPS     5     // The number of cups at the bottom
#define EACH_CUP_ANGLE_INTERVAL ((MAX_BTM_ANGLE - MIN_BTM_ANGLE) / NUM_OF_CUPS)
  int btmAngle = MIN_BTM_ANGLE + EACH_CUP_ANGLE_INTERVAL * mResult;
  this->write(btmAngle);
}

void BottomServo::setResult(colorResult result){
  mResult = result;
}

