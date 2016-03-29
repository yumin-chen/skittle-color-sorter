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

#include "ColorView.h"
#include "LCD.h"

ColorView::ColorView()
{
  bDirty = false;
}

void ColorView::update()
{
  if (!bDirty) return;

  // Generate the colorResult enum-indexed constant array colorViewList
#define COLOR_DEF( identifier, name, color, color_view )  color_view
  const C_Color colorViewList [] = { COLORS_DEFINITION };
#undef COLOR_DEF

  // Write the color to LED
  this->write(colorViewList[mResult]);
}

void ColorView::write(const C_Color& c)
{
  analogWrite(PIN_COLOR_RED, (c.r));
  analogWrite(PIN_COLOR_GREEN, (c.g));
  analogWrite(PIN_COLOR_BLUE, (c.b));
}

void ColorView::setResult(colorResult result)
{
  mResult = result;
}

