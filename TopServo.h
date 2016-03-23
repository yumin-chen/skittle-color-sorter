/*
  Top Servo

  This class defines the continuous rotation servo at the top

*/
#ifndef  _TOP_SERVO_H_
#define  _TOP_SERVO_H_

#include <Arduino.h>
#include <Servo.h>

class TopServo : public Servo 
{
public:
  TopServo();
  void setup();
  void update(unsigned long);
  void remeasureColor();
  void stopRemeasuring();
  boolean isRemeasuring();

private:
  boolean bJammed; // Is the Skittle stuck on the top servo
  boolean bRemeasuring; // Is the Skittle's color being re-measured
  boolean bRemDir; // Is the direction reversed when re-measuring the color
  int iRetry; // How many times have tried to detect the color
};

#endif // _TOP_SERVO_H_ 