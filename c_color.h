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
#include "Context.h"

#define COLOR_UNIT uint8_t

/** A basic color class for some color operations. */
class C_Color
{
  public:

    /** Initialize a color by red, green, blue, clear (optional) channel values. */
    C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, uint16_t clear = 0);

    /** Create a color from raw colors reported by the color sensor. This will
        convert the color to the standard unified 8-bits wide RGB channel for
        convinience of further analyzation.
        @return a C_Color object created from raw colors, whose RGB channel
        values converted to 8-bits wide.*/
    static C_Color createFromRawColors(int cycles,
                                       uint16_t red, uint16_t green, uint16_t blue, uint16_t clear = 0);

    /**  Compares the color with another color.
         @return the euclidean distance (standard variance) of the colors. */
    int compare(const C_Color&)const;

    /** Compares the color with an array of colors. This function is to find
        the color result that is the closest to the source color.
        @param colors An array of colors that the source color compares with.
        @param allowed_variance Allowed variance to ensure the integrity of 
        the result. If the result's difference is still bigger than the 
        allowed value then RESULT_UNKNOWN will be returned.
        @return The color result that is the closest to the source color.
        Returns RESULT_UNKNOWN if no color in the colorList is matched. */
    colorResult compareWithColorList(const C_Color color_list [], int allowed_variance) const;

    /**  Maximizes color based on the clear value. */
    void maximize();

    /** Prints the color information onto the Serial. */
    void print() const;

    COLOR_UNIT  r;  /**< The red channel value    */
    COLOR_UNIT  g;  /**< The green channel value  */
    COLOR_UNIT  b;  /**< The blue channel value   */
    uint16_t  c;  /**< The clear channel value  */

};

#endif // _C_COLOR_H_ 
