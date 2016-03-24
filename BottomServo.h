/*
  Bottom Servo

  This class defines the standard servo at the top

*/
#ifndef  _BOTTOM_SERVO_H_
#define  _BOTTOM_SERVO_H_

#include <Arduino.h>
#include <Servo.h>
#include "Context.h"

class BottomServo : public Servo, private Context
{
public:
  BottomServo();
  void setup();
  void update();

private:
};

#endif // _BOTTOM_SERVO_H_ 
