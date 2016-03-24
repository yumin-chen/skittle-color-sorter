/*
  Bottom Servo

  This class defines the standard servo at the top

*/
#ifndef  _COLOR_VIEW_H_
#define  _COLOR_VIEW_H_

#include <Arduino.h>
#include "Adafruit_TCS34725.h"
#include "Context.h"

class ColorView : private Context
{
public:
	ColorView();
	void update();
	void write(byte, byte, byte);

private:
};

#endif // _COLOR_VIEW_H_ 
