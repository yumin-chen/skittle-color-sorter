/*
  Bottom Servo

  This class defines the standard servo at the bottom

*/

// The circuit:
// * Color View
//    Red pin to diginal pin 7
#define PIN_COLOR_RED 7
//    Green pin to diginal pin 9
#define PIN_COLOR_GREEN 9
//    Blue pin to diginal pin 6
#define PIN_COLOR_BLUE 6

#define C_COLOR_SIGNAL_TIME 3000 // Each Skittle gets 1s color view signal

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
        analogWrite(PIN_COLOR_RED, 255);
        analogWrite(PIN_COLOR_GREEN, 0);
        analogWrite(PIN_COLOR_BLUE, 0);
        break;
      case 1:
        Serial.println("GREEN");
        analogWrite(PIN_COLOR_RED, 0);
        analogWrite(PIN_COLOR_GREEN, 255);
        analogWrite(PIN_COLOR_BLUE, 0);
        break;
      case 2:
        Serial.println("YELLOW");
        analogWrite(PIN_COLOR_RED, 255);
        analogWrite(PIN_COLOR_GREEN, 255);
        analogWrite(PIN_COLOR_BLUE, 0);
        break;
      case 3:
        Serial.println("PURPLE");
        analogWrite(PIN_COLOR_RED, 128);
        analogWrite(PIN_COLOR_GREEN, 32);
        analogWrite(PIN_COLOR_BLUE, 128);
        break;
      case 4:
        Serial.println("ORANGE");
        analogWrite(PIN_COLOR_RED, 255);
        analogWrite(PIN_COLOR_GREEN, 128);
        analogWrite(PIN_COLOR_BLUE, 32);
        break;
    }
    index++;
  }

  // Check if the color view signal has lasted enough time
  if (millis() - lastColorSignalTime > C_COLOR_SIGNAL_TIME) {
    // Set it back to black
    analogWrite(PIN_COLOR_RED, 0);
    analogWrite(PIN_COLOR_GREEN, 0);
    analogWrite(PIN_COLOR_BLUE, 0);
  }
}


