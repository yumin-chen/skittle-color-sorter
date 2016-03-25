/**************************************************************************/
/**
    Color Sensor

    The TCS34725 color sensor that is used to measure Skittle's color
    values. All the algorithms regarding to color detection, analyzation
    are defined here.

    @file     ColorSensor.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/
#ifndef  _COLOR_SENSOR_H_
#define  _COLOR_SENSOR_H_

#include <Arduino.h>
#include "Adafruit_TCS34725.h"
#include "Context.h"
#include "C_Color.h"

/** A subclass of Adafruit_TCS34725 Color Sensor. This class contains all
    the algorithms regarding to color detection, analyzation. */
class ColorSensor : public Adafruit_TCS34725, private Context
{
  public:

    /** Constructor */
    ColorSensor();

    /** Initializes I2C and configures the sensor. This function should be
        called before you do anything else. */
    void setup();

    /** The main logic of the color sensor. This function should be called
        on the main loop. */
    void update();

    /** Get the time when the last time the color sensor was measuring a Skittle.
      @return The last time when the Skittle arrives at the color sensor the
      number of milliseconds since the Arduino board began. */
    unsigned long getLastSkittleTime();

  protected:

    /** The update logic used to test and get the average color of certain
        Skittle. */
    void _calibrating(const C_Color&);

    /** Performs certain actions after the best color has been picked up
        by the color sensor. This function will be called each time the
        color sensor is through a measuring cycle. */
    void _analyzeColor(const C_Color&);

    unsigned long   lastSkittleTime;    /**< Time when the last skittle arrived         */
    boolean         isMeasuring;        /**< Is the a Skittle being measured right now  */

};

#endif // _COLOR_SENSOR_H_ 
