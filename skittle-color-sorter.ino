/**************************************************************************/
/**
    Skittle Color Sorter

    An Arduino UNO board sketch that programs a Skittle color sorting
    machine that can sort Skittles by their color.

    @file     skittle-color-sorter.ino
    @mainpage Skittle Color Sorter
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>

    @section  Introduction
    This Skittle Color Sorter consists of a Adafruit TCS34725 Color Sensor to
    measure color values, a Continuous Rotation Servo at the top to keep the
    Skittles going down, a Standard Servo at the bottom to move the arm to
    bring the Skittles to a desired place, and a LCD Display to show
    information like the number of Skittles.

    @section  More
    - [CharmySoft]    http://CharmySoft.com/
    - [Project Page]  http://CharmySoft.com/app/skittle-sorter.htm
    - [Github Repo]   http://github.com/CharmySoft/skittle-color-sorter
*/
/**************************************************************************/

// Include libraries
#include "Context.h"                      // Context (global variables) 
#include "LCD.h"                          // LCD display
#include "TopServo.h"                     // Top Servo
#include "BottomServo.h"                  // Bottom Servo
#include "ColorView.h"                    // Color View
#include "ColorSensor.h"                  // Color Sensor

// Initialize global variables
LCD           Context::lcd;               // The liquid-crystal display
TopServo      Context::servoTop;          // The top continuous rotation servo
BottomServo   Context::servoBtm;          // The bottom standard servo
ColorView     Context::colorView;         // The color LED
ColorSensor   Context::colorSensor;       // The Adafruit color sensor

// Use a macro to generate a constant array colorList 
// whose indexes correspond to the colorResult enum
#define COLOR_DEF( identifier, name, color, color_view )  color
const C_Color Context::colorList [] = { COLORS_DEFINITION };
#undef COLOR_DEF

int           Context::skittleCount         = 0;      // The number of skittles sorted
unsigned long Context::lastSkittleTime      = 0;      // Time when the last skittle was being sorted
boolean       Context::isColorBeingMeasured = false;  // Is the color sensor measuring a Skittle right now
colorResult   Context::colorResults [128];            // An array to store all the measured colors

void setup() {
  // Set up the serial for debugging purposes
  Serial.begin(9600);

  // Set up the LCD screen
  Context::lcd.setup();

  // Set up the servos
  Context::servoTop.setup();
  Context::servoBtm.setup();

  // Set up the Adafruit color sensor
  Context::colorSensor.setup();

  // Print some text on the LCD screen as a welcome message
  Context::lcd.print("Color Sorter");

  // Set every element in Context::colorResults to RESULT_UNKNOWN
  for (int i = 0; i < sizeof(Context::colorResults); i++)
    Context::colorResults[i] = RESULT_UNKNOWN;

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
