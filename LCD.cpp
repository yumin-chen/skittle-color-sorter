/*
  Liquid-Crystal Display (LCD)

  This class defines the Liquid-Crystal Display (LCD)
  
*/

// The circuit:
// * LCD
//    RS pin to digital pin 12
#define PIN_LCD_RS 12
//    Enable pin to digital pin 11
#define PIN_LCD_ENABLE 11
//    LCD D4 pin to digital pin 5
#define PIN_LCD_D4 5
//    LCD D5 pin to digital pin 4
#define PIN_LCD_D5 4
//    LCD D6 pin to digital pin 3
#define PIN_LCD_D6 3
//    LCD D7 pin to digital pin 2
#define PIN_LCD_D7 2
//    LCD R/W pin to ground

#include "LCD.h"

LCD::LCD() : LiquidCrystal(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, 
  PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7) // Initialize the LCD object
{
}

void LCD::setup(){
  /* Sets up the LCD object
  */
  Context::lcd.begin(16, 2); // Set up the LCD's number of columns and rows
}

void LCD::update(){
  /* This function should be called on the main loop. 
  */
  if(skittleCount > 0){
    this->clear();
    this->print("Count: ");
    this->print(skittleCount, DEC);
  }
}
