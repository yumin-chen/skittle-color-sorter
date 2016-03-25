/**************************************************************************/
/**
    Color View (LED)

    @file     ColorView.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>

    @section  Circuit
    - Color View
      + Red pin to pin 7
      + Green pin to pin 9
      + Blue pin to pin 6
*/
/**************************************************************************/
#define PIN_COLOR_RED       7     /**< The pin to write Red channel value   */
#define PIN_COLOR_GREEN     9     /**< The pin to write Green channel value */
#define PIN_COLOR_BLUE      6     /**< The pin to write Blue channel value  */

#define C_COLOR_SIGNAL_TIME 5000  /**< The amount of time a signal lasts in milliseconds */

#include "ColorView.h"
#include "LCD.h"

void ColorView::update() {
  static int index = 0;
  unsigned long lastColorSignalTime = 0;
  if (HAS_RESULT(colorResults[index]))
  {
    // Generate the colorResult enum-indexed constant array nameList
#define COLOR_DEF( identifier, name, color, color_view )  name
    const String nameList [] = { COLORS_DEFINITION };
#undef COLOR_DEF

    // Generate the colorResult enum-indexed constant array colorViewList
#define COLOR_DEF( identifier, name, color, color_view )  color_view
    const C_Color colorViewList [] = { COLORS_DEFINITION };
#undef COLOR_DEF

    // Print out the color result's name
    Serial.println(nameList[colorResults[index]]);
    lcd.setBottomText(nameList[colorResults[index]]);

    // Write the color to LED
    this->write(colorViewList[colorResults[index]]);

    index++;
    lastColorSignalTime = millis();
  }

  // Check if the color view signal has lasted enough time
  if (millis() - lastColorSignalTime > C_COLOR_SIGNAL_TIME) {
    // Set it back to black
    this->write(C_Color(0, 0, 0));
  }
}

void ColorView::write(const C_Color& c) {
  analogWrite(PIN_COLOR_RED, (c.r));
  analogWrite(PIN_COLOR_GREEN, (c.g));
  analogWrite(PIN_COLOR_BLUE, (c.b));
}

