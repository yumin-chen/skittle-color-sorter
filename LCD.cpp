/**************************************************************************/
/**
    Liquid-Crystal Display (LCD)

    @file     LCD.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>

    @section  Circuit
    - LCD
      + RS pin to digital pin 12
      + Enable pin to digital pin 11
      + LCD D4 pin to digital pin 5
      + LCD D5 pin to digital pin 4
      + LCD D6 pin to digital pin 3
      + LCD D7 pin to digital pin 2
      + LCD R/W pin to ground
*/
/**************************************************************************/
#define PIN_LCD_RS        12      /**< The RS pin      */
#define PIN_LCD_ENABLE    11      /**< The Enable pin  */
#define PIN_LCD_D4        5       /**< The data pin D4 */
#define PIN_LCD_D5        4       /**< The data pin D5 */
#define PIN_LCD_D6        3       /**< The data pin D6 */
#define PIN_LCD_D7        2       /**< The data pin D7 */

#include "LCD.h"

LCD::LCD() : LiquidCrystal(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7) {}

void LCD::setup()
{
  this->begin(16, 2);
}

void LCD::update()
{
  // If content is still valid, there's no need to refresh
  if (bValid) return;

  // Clear Screen
  this->clear();

  // Print out the first line
  this->setCursor(0, 0);
  this->print(sTopText.c_str());

  // Print out the the second line
  this->setCursor(0, 1);
  this->print(sBtmText.c_str());

}

void LCD::setTopText(String text)
{
  // Set the top text
  sTopText = text;
  // Invalidate the content to make it refresh
  bValid = false;
}

void LCD::setBottomText(String text)
{
  // Set the bottom text
  sBtmText = text;
  // Invalidate the content to make it refresh
  bValid = false;
}

void LCD::print(char const* text)
{
  LiquidCrystal::print(text);
}
