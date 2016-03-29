/**************************************************************************/
/**
    Color View (LED)

    A RGB (Red, Green, Blue) LED that can be used to show different colors
    by adjusting the brightness of each of the three LEDs. This class
    defines how the Skittle's color results are showed on the LED.

    @file     ColorView.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/
#ifndef  _COLOR_VIEW_H_
#define  _COLOR_VIEW_H_

#include <Arduino.h>
#include "Adafruit_TCS34725.h"
#include "Context.h"
#include "C_Color.h"

/** Class ColorView defines how the Skittle's color results are showed on
    the LED. */
class ColorView : private Context
{
  public:

    /** Constructor. Initialize members. */
    ColorView();

    /** Updates the color view logic. This function should be called on the
        arduino's main loop. */
    void update();

    /** Writes a desired color to the LED. This sets the LED's color by
        writing analog color values to the LED.
        @param c The desired color to be written. */
    void write(const C_Color& c);

    /** Set the color view to show a disired result.
        @param result The result that is to be shown on the LED.*/
    void setResult(colorResult result);

  private:
    colorResult mResult;  /**< The result to show. */
    boolean   bDirty;     /**< Does color view need to refresh. */

};

#endif // _COLOR_VIEW_H_ 
