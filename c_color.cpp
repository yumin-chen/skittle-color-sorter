/**************************************************************************/
/**
    Color Class

    A basic color class for some color operations.

    @file     C_Color.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/

#include "C_Color.h"

C_Color::C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, COLOR_UNIT clear)
{
  r = red;
  g = green;
  b = blue;
  c = clear;
}

C_Color C_Color::createFromRawColors(int cycles, uint16_t red, uint16_t green,
                                     uint16_t blue, uint16_t clear)
{
  return C_Color(red / cycles, green / cycles, blue / cycles, clear / cycles);
}

C_Color C_Color::compare(const C_Color& compared_color) const
{
  COLOR_UNIT color_diff_r = abs(int(r) - int(compared_color.r));
  COLOR_UNIT color_diff_g = abs(int(g) - int(compared_color.g));
  COLOR_UNIT color_diff_b = abs(int(b) - int(compared_color.b));
  return C_Color(color_diff_r, color_diff_g, color_diff_b);
}

colorResult C_Color::compareWithColorList(const C_Color color_list [], int allowed_variance) const
{
    // Set the mininum difference to the allowed color variance
  int min_diff = allowed_variance;

  // Set the temperary result to RESULT_UNKNOWN
  colorResult tempResult = RESULT_UNKNOWN;

  for (int i = 0; i < COLOR_LIST_SIZE; i++) {
    // Compare the source color with the color defined in the colorList
    C_Color diff = this->compare(color_list[i]);
    // Add the color difference's primary colors (R + G + B) together
    int agg = diff.aggregate();
    // Check if the aggregated color difference value is less than the minimum color difference
    if (agg < min_diff) {
      // If this is less than the minimun
      min_diff = agg; // Set the minimun difference to this aggregated color difference
      tempResult = static_cast<colorResult>(i);; // Set the result to this color's index
    }
  }

  return tempResult;
}

int C_Color::aggregate() const
{
  return r + g + b;
}

void C_Color::maximize()
{
  float scale = 256.0 / c;
  r *= scale; g *= scale; b *= scale;
}

void C_Color::print() const {
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
