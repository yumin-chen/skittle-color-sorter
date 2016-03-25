/**************************************************************************/
/**
    Bottom Servo

    Controls the standard servo at the bottom that acts as an arm to put
    the Skittle into designated cups.

    @file     BottomServo.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/

#ifndef  _BOTTOM_SERVO_H_
#define  _BOTTOM_SERVO_H_

#include <Arduino.h>
#include <Servo.h>
#include "Context.h"

/** The standard servo at the bottom. It acts as an arm to put the Skittles
    into designated cups. */
class BottomServo : public Servo, private Context
{
  public:
  
    /** Attaches the corresponding pins to the bottom servo object. */
    void setup();
    
    /** Updates the bottom servo. This writes the updated bottom arm angle 
     *  to the servo. This function should be called in the main loop. */
    void update();

  private:
};

#endif // _BOTTOM_SERVO_H_ 
