/*
  Skittle Color Sorter

  This sketch programs the Skittle Color Sorter that we
  assembled.
*/

// Include libraries
#include "Context.h"            // Context (global variables)
#include "LCD.h"                // LCD display
#include "TopServo.h"           // Top Servo
#include "BottomServo.h"        // Bottom Servo
#include "ColorView.h"          // Color View
#include "ColorSensor.h"        // Color Sensor

// Initialize global variables
LCD Context::lcd; 
TopServo Context::servoTop; 
BottomServo Context::servoBtm; 
ColorView Context::colorView;
ColorSensor Context::colorSensor; 
const C_Color Context::colorList [SKITTLE_COLORS] = {C_SKITTLE_RED, C_SKITTLE_GREEN, C_SKITTLE_YELLOW, C_SKITTLE_PURPLE, C_SKITTLE_ORANGE};
int Context::skittleCount = 0;
unsigned long Context::lastSkittleTime = 0;
boolean Context::isColorBeingMeasured = false;
byte Context::colorResults [256];

void setup() {
  
  // Set up the serial for debugging purposes
  Serial.begin(9600);

  // Set up the servos
  Context::servoTop.setup();
  Context::servoBtm.setup();

  if (!Context::colorSensor.begin()) {
    // If the color sensor failed to initialize, print out an error
    Context::lcd.print("ERR: Color Sensor Connection");
    while (1);
  }

  Context::lcd.print("Color Sorter");

  
    // Set every element in Context::colorResults to RESULT_UNKNOWN
    for (int i = 0; i < sizeof(Context::colorResults); i++) {
      Context::colorResults[i] = RESULT_UNKNOWN;
    }
}

void loop() {
  // Update the LCD
  Context::lcd.update();

  // Update the top servo
  Context::servoTop.update();

  // Update the bottom servo (arm)
  Context::servoBtm.update();

  // Update the color sensor
  Context::colorSensor.update();

  // Update the color view LED output
  Context::colorView.update();

}
