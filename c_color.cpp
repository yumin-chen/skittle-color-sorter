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
