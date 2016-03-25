/**************************************************************************/
/**
    Liquid-Crystal Display (LCD)

    The Liquid-Crystal Display (LCD) that can display textual information.

    @file     LCD.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/
#ifndef  _LCD_H_
#define  _LCD_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Context.h"

/** A subclass of LiquidCrystal that can display textual information. */
class LCD : public LiquidCrystal, private Context
{
  public:
    /** Constructor. Initialize the liquid-crystal display with corresponding
        pins */
    LCD();

    /** Set up the LCD's number of columns and rows. This function should be
        called before anything else. */
    void setup();

    /** Refresh the LCD's content. This clears everything already written to
        the LCD screen, and write the updated content. This function should
        be called in the main loop. */
    void update();

    /** Set the label text. The label will be shown on the second line of the
        LCD screen. */
    void setLabelText(String text);

  private:
    String sLabelText; /**< The label text to be shown on the second line. */
};

#endif // _LCD_H_ 
