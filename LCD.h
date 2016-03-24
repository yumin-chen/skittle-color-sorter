/*
  Liquid-Crystal Display (LCD)

  This class defines the Liquid-Crystal Display (LCD)

*/
#ifndef  _LCD_H_
#define  _LCD_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Context.h"

class LCD : public LiquidCrystal, private Context
{
public:
	LCD();
  void setup();
	void update();

private:
};

#endif // _LCD_H_ 
