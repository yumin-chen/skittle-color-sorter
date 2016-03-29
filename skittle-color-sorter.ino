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
    - [Project Page]  http://CharmySoft.com/app/skittle-sorter
    - [Github Repo]   http://CharmySoft.com/app/skittle-sorter/github
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

int           Context::skittleCount         = 0;      // The number of skittles sorted

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
  Context::lcd.setTopText("Color Sorter");

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
