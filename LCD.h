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

    /** Set the text to show on the first line.  */
    void setTopText(String text);

    /** Set the text to show on the second line.  */
    void setBottomText(String text);

  protected:
    /** Print text to the LCD. This is to protect the `print` method so it
        cannot be publically accessed. You should use `setTopText` and
        `setBottomText` instead. */
    void      print(char const*);

    String    sTopText;   /**< The text to be shown on the first line.  */
    String    sBtmText;   /**< The text to be shown on the second line. */
    boolean   bValid;     /**< Is the screen content still valid. */
};

#endif // _LCD_H_ 
