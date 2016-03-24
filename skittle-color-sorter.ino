/*
  Skittle Color Sorter

  This sketch programs the Skittle Color Sorter that we
  assembled.
*/


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

// Include libraries
#include <LiquidCrystal.h>      // LCD display
#include "Context.h"            // Context (global variables)
#include "TopServo.h"           // Top Servo
#include "BottomServo.h"        // Bottom Servo
#include "ColorView.h"          // Color View
#include "ColorSensor.h"        // Color Sensor

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4,
                  PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7); // Initialize the display with the numbers of the interface pins

// Initialize global variables
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

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Set up the servos
  Context::servoTop.setup();
  Context::servoBtm.setup();

  if (!Context::colorSensor.begin()) {
    // If the color sensor failed to initialize, print out an error
    lcd.print("ERR: Color Sensor Connection");
    while (1);
  }

  lcd.print("Color Sorter");

  
    // Set every element in Context::colorResults to RESULT_UNKNOWN
    for (int i = 0; i < sizeof(Context::colorResults); i++) {
      Context::colorResults[i] = RESULT_UNKNOWN;
    }
}

void loop() {
  lcd.clear();
  lcd.print("Count: ");
  lcd.print(Context::skittleCount, DEC);

  // Update the top servo
  Context::servoTop.update();

  // Update the bottom servo (arm)
  Context::servoBtm.update();

  // Update the color sensor
  Context::colorSensor.update();

  // Update the color view LED output
  Context::colorView.update();

}
