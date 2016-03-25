/**************************************************************************/
/**
    Top Servo

    Controls the continuous rotation servo at the top that spins and moves
    Skittles to let the color sensor read the color.

    @file     TopServo.h
    @author   Charlie Chen <Charlie@CharmySoft.com>
*/
/**************************************************************************/
#ifndef  _TOP_SERVO_H_
#define  _TOP_SERVO_H_

#include <Arduino.h>
#include <Servo.h>
#include "Context.h"

/** The continuous rotation servo at the top. It spins around and moves
    Skittles to let the color sensor read the color. */
class TopServo : public Servo, private Context
{
  public:

    /** Constructor. Initialize all the members variables. */
    TopServo();

    /** Attaches the corresponding pins to the top servo object. */
    void setup();

    /** Updates the top servo. This writes the updated speed to the servo.
        This function should be called in the main loop. */
    void update();

    /** Starts re-measuring the Skittle's color. This will not work if the
        retried times is no less than C_RETRY_UNKNOWN_COLOR. */
    void remeasureColor();

    /** Stops re-measuring the Skittle's color. */
    void stopRemeasuring();
    
    /** Is the Skittle being re-measured its color.
        @return true if the Skittle is being re-measured.*/
    boolean isRemeasuring();

  protected:
    boolean   bJammed;      /**< Is the Skittle stuck on the top servo */
    boolean   bRemeasuring; /**< Is the Skittle's color being re-measured */
    boolean   bRemDir;      /**< Is the direction reversed when re-measuring the color */
    int       iRetry;       /**< How many times have tried to detect the color */
};

#endif // _TOP_SERVO_H_ 
