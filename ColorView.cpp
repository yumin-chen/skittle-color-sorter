/*
  Color View

  This class defines the color LED output

*/

// The circuit:
// * Color View
//    Red pin to diginal pin 7
#define PIN_COLOR_RED 7
//    Green pin to diginal pin 9
#define PIN_COLOR_GREEN 9
//    Blue pin to diginal pin 6
#define PIN_COLOR_BLUE 6

#define C_COLOR_SIGNAL_TIME 3000 // Each Skittle gets 3s color view signal

#include "ColorView.h"

ColorView::ColorView(){
}

void ColorView::update(){
	/* This function should be called on the main loop. 
	*/
  static int index = 0;
  unsigned long lastColorSignalTime = 0;
  if (HAS_RESULT(colorResults[index])) {
    lastColorSignalTime = millis();
    switch (colorResults[index]) {
      case 0:
        Serial.println("RED");
    	this->write(255, 0, 0);
        break;
      case 1:
        Serial.println("GREEN");
    	this->write(0, 255, 0);
        break;
      case 2:
        Serial.println("YELLOW");
    	this->write(255, 255, 0);
        break;
      case 3:
        Serial.println("PURPLE");
    	this->write(128, 32, 128);
        break;
      case 4:
        Serial.println("ORANGE");
    	this->write(255, 128, 32);
        break;
    }
    index++;
  }

  // Check if the color view signal has lasted enough time
  if (millis() - lastColorSignalTime > C_COLOR_SIGNAL_TIME) {
    // Set it back to black
    this->write(0, 0, 0);
  }
}

void ColorView::write(byte r, byte g, byte b){
	/* Writes the analog color values. 
	*/
    analogWrite(PIN_COLOR_RED, r);
    analogWrite(PIN_COLOR_GREEN, g);
    analogWrite(PIN_COLOR_BLUE, b);
}


