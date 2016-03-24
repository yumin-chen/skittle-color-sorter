/*
  Color Sensor

  This class defines the color sensor

*/
#ifndef  _COLOR_SENSOR_H_
#define  _COLOR_SENSOR_H_

#include <Arduino.h>
#include "Adafruit_TCS34725.h"
#include "Context.h"

class ColorSensor : public Adafruit_TCS34725, private Context
{
public:
	ColorSensor();
  void setup();
	void update();
private:
	void calibrating(C_Color);
	void analyzeColor();
	C_Color best_color = C_Color(0, 0, 0);
};

#endif // _COLOR_SENSOR_H_ 
