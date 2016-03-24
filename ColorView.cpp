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
    // Use a macro to generate the colorResult enum-indexed nameList constant array
#define COLOR_DEF( identifier, name, color )  name
    const String nameList [] = { COLORS_DEFINITION };
#undef COLOR_DEF
    Serial.println(nameList[colorResults[index]]);
    this->write(colorList[colorResults[index]]);
    index++;
    lastColorSignalTime = millis();
  }

  // Check if the color view signal has lasted enough time
  if (millis() - lastColorSignalTime > C_COLOR_SIGNAL_TIME) {
    // Set it back to black
    this->write(C_Color(0, 0, 0));
  }
}

void ColorView::write(const C_Color& c){
  /* Writes the analog color values. 
  */
  C_Color maxmized_color = c;
  maxmized_color.maximize();
  analogWrite(PIN_COLOR_RED, colorCorrect(maxmized_color.r));
  analogWrite(PIN_COLOR_GREEN, colorCorrect(maxmized_color.g));
  analogWrite(PIN_COLOR_BLUE, colorCorrect(maxmized_color.b));
}

uint8_t ColorView::colorCorrect(uint8_t i){
  /*  RGB -> gamma color correction
  */
  float x = i;
  x /= 255;
  x = pow(x, 2.5);
  x *= 255;
  return 256 - x;
}
