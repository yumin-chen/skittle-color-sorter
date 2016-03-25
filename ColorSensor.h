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

    /** Compares the color with the pre-defined colors in the colorList.
        This function is to find the color result that is the closest to
        the source color.
        @param color The source color that is used to compare with colors
        in the colorList.
        @return The color result that is the closest to the source color.
        Returns RESULT_UNKNOWN if no color in the colorList is matched. */
    colorResult compareWithColorList(const C_Color& color);

  protected:

    /** The update logic used to test and get the average color of certain
        Skittle. */
    void _calibrating(const C_Color&);

    /** Performs certain actions after the best color has been picked up
        by the color sensor. This function will be called each time the
        color sensor is through a measuring cycle. */
    void _analyzeColor(const C_Color&);

};

#endif // _COLOR_SENSOR_H_ 
