/**************************************************************************/
/**
    Color Class

    A basic color class for some color operations.

    @file     C_Color.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/

#include "C_Color.h"

C_Color::C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, uint16_t clear)
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

int C_Color::compare(const C_Color& compared_color) const
{
  int d_r = r - compared_color.r;
  int d_g = g - compared_color.g;
  int d_b = b - compared_color.b;
  return sqrt(d_r*d_r + d_g*d_g + d_b*d_b);
}

colorResult C_Color::compareWithColorList(const C_Color color_list [], int allowed_variance) const
{
    // Set the mininum difference to the allowed color variance
  int min_diff = allowed_variance;

  // Set the temperary result to RESULT_UNKNOWN
  colorResult tempResult = RESULT_UNKNOWN;

  for (int i = 0; i < COLOR_LIST_SIZE; i++) {
    // Compare the source color with the color defined in the colorList
    int diff = this->compare(color_list[i]);
    // Check if the aggregated color difference value is less than the minimum color difference
    if (diff < min_diff) {
      // If this is less than the minimun
      min_diff = diff; // Set the minimun difference to this aggregated color difference
      tempResult = static_cast<colorResult>(i);; // Set the result to this color's index
    }
  }

  return tempResult;
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
