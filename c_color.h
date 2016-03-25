/**************************************************************************/
/**
    Color Class

    A basic color class for some color operations.

    @file     C_Color.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/

#ifndef  _C_COLOR_H_
#define  _C_COLOR_H_

#include <Arduino.h>

#define COLOR_UNIT uint8_t

/** A basic color class for some color operations. */
class C_Color
{
  public:

    /** Initialize a color by red, green, blue, clear (optional) channel values. */
    C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, COLOR_UNIT clear = 0);

    /** Create a color from raw colors reported by the color sensor. This will
        convert the color to the standard unified 8-bits wide RGB channel for
        convinience of further analyzation.
        @return a C_Color object created from raw colors, whose RGB channel
        values converted to 8-bits wide.*/
    static C_Color createFromRawColors(int cycles,
                                       uint16_t red, uint16_t green, uint16_t blue, uint16_t clear = 0);

    /**  Compares the color with another color.
         @return the color difference. */
    C_Color compare(const C_Color&)const;

    /**  Maximizes color based on the clear value. */
    void maximize();

    /** Get the sum of RGB channels. @return the sum of RGB channels.*/
    int aggregate() const;

    /** Prints the color information onto the Serial. */
    void print() const;

    COLOR_UNIT  r;  /**< The red channel value    */
    COLOR_UNIT  g;  /**< The green channel value  */
    COLOR_UNIT  b;  /**< The blue channel value   */
    COLOR_UNIT  c;  /**< The clear channel value  */

  private:
};

#endif // _C_COLOR_H_ 
