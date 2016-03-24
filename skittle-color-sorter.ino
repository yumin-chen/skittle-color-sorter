/*
  Skittle Color Sorter

  This sketch programs the Skittle Color Sorter that we
  assembled.
*/

// * Bottom Servo (Arm)
//    Digital pin 10
#define PIN_BTM_SERVO 10
// * Color Sensor
//    SDA pin to analog pin 4
//    SCL pin to analog pin 5
//    Color View Red pin to diginal pin 7
//    Color View Green pin to diginal pin 9
//    Color View Blue pin to diginal pin 6
#define PIN_COLOR_RED 7
#define PIN_COLOR_GREEN 9
#define PIN_COLOR_BLUE 6
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

// Uncomment the following lines to enter calibration mode
// #define F_CALIBRATING true // Is calibration in progress
#define F_CALI_EMPTY_HOLE false // Is calibrating the empty hole's color

// Constants
#define C_ATIME TCS34725_INTEGRATIONTIME_154MS //  Color sensor integration time 154ms
#define C_CYCLES (256-C_ATIME) // Color sensor cycles according to the integration time

#define C_HOLE_CLEAR 300 // The clear value used to determine the hole's arrival
#define C_IDEAL_CLEAR 96 // The ideal clear value for color detection
#define C_ALLOWED_COLOR_VARIANCE 64 // Allowed color variance for color detection
#define C_ALLOWED_CLEAR_VARIANCE 128 // Allowed clear variance for color detection

#define C_COLOR_SIGNAL_TIME 3000 // Each Skittle gets 1s color view signal

// Include libraries
#include <LiquidCrystal.h>      // LCD display
#include <Servo.h>              // Servos
#include "Adafruit_TCS34725.h"  // Color Sensor
#include "C_Color.h"            // Color operations
#include "TopServo.h"           // Top Servo

// Calibrated Skittle's Colors
// These are the skittle's colors
#define C_COLOR_EMPTY C_Color(66, 89, 71, 170)
#define C_SKITTLE_RED C_Color(97, 73, 62, 117)
#define C_SKITTLE_GREEN C_Color(58, 106, 54, 148)
#define C_SKITTLE_YELLOW C_Color(90, 90, 34, 210)
#define C_SKITTLE_PURPLE C_Color(65, 84, 81, 122)
#define C_SKITTLE_ORANGE C_Color(122, 64, 40, 154)

const C_Color colorList [] = {C_SKITTLE_RED, C_SKITTLE_GREEN, C_SKITTLE_YELLOW, C_SKITTLE_PURPLE, C_SKITTLE_ORANGE};

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4,
                  PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7); // Initialize the display with the numbers of the interface pins
TopServo servoTop; // Declare the object for the top continuous rotation servo
Servo servoBtm; // Declare the object for the bottom standard servo
Adafruit_TCS34725 tcs = Adafruit_TCS34725(C_ATIME, TCS34725_GAIN_1X); // Initialize the color sensor object

int skittleCount = 0; // The number of skittles sorted
unsigned long lastSkittleTime = 0; // Time when the last skittle was being sorted
static boolean isColorBeingMeasured = false; // Is the color sensor measuring a Skittle right now
byte colorResults [256]; // An array to store all the detected colors;
#define RESULT_UNKNOWN 255
#define HAS_RESULT(color_index) (color_index >= 0 && color_index < sizeof(colorList))

void setup() {
  // Set up the serial for debugging purposes
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Attach the servos on corresponding pins to the servo objects
  servoTop.setup();
  servoBtm.attach(PIN_BTM_SERVO);

  if (!tcs.begin()) {
    // If the color sensor failed to initialize, print out an error
    lcd.print("ERR: Color Sensor Connection");
    while (1);
  }

  lcd.print("Color Sorter");

  // Set every element in colorResults to RESULT_NO_DATA_YET
  for (int i = 0; i < sizeof(colorResults); i++) {
    colorResults[i] = RESULT_UNKNOWN;
  }

}

void updateBtmServo() {
  // Calculate the angle of the bottom servo arm
#define MAX_BTM_ANGLE 150 // Maximum bottom servo angle
#define MIN_BTM_ANGLE 30 // Minimum bottom servo angle
#define NUM_OF_CUPS 5 // The number of cups at the bottom
#define EACH_CUP_ANGLE_INTERVAL ((MAX_BTM_ANGLE - MIN_BTM_ANGLE) / NUM_OF_CUPS)
  int result;
  if (skittleCount > 1 && HAS_RESULT(colorResults[skittleCount - 1])) {
    result = colorResults[skittleCount - 1];
  } else if (skittleCount > 2) {
    result = colorResults[skittleCount - 2];
  } else {
    result = 2;
  }
  int btmAngle = MIN_BTM_ANGLE + EACH_CUP_ANGLE_INTERVAL * result;
  servoBtm.write(btmAngle);
}

void __calibrating(C_Color new_color) {
  /* This function is used to test and get the average color
      of a certain Skittle
  */
  static C_Color avg_c = C_Color(new_color.r, new_color.g, new_color.b, new_color.c);
  static int count = 0;
  int diff = avg_c.compare(new_color).aggregate();
  Serial.print("Diff: "); Serial.println(diff);
  if (diff > C_ALLOWED_COLOR_VARIANCE) {
    Serial.println("Difference too big. This sample data is discarded.");
  } else {
    avg_c.r = (avg_c.r * count + new_color.r) / (count + 1);
    avg_c.g = (avg_c.g * count + new_color.g) / (count + 1);
    avg_c.b = (avg_c.b * count + new_color.b) / (count + 1);
    avg_c.c = (avg_c.c * count + new_color.c) / (count + 1);
    count++;
    Serial.println("Average color:");
    avg_c.print();
  }
}

void analyzeColor(C_Color best_color) {
  if (!F_CALI_EMPTY_HOLE && best_color.compare(C_COLOR_EMPTY).aggregate() < 16
      && abs(int(best_color.c) - int(C_COLOR_EMPTY.c)) < 32) {
    // If this is an empty hole.
    Serial.println("Empty hole");
    if (servoTop.isRemeasuring()) {
      servoTop.stopRemeasuring();
    }
  } else {
    Serial.println("A Skittle has been detected!");
    //Serial.println("Maximized colors:");
    //best_color.print();

#if F_CALIBRATING
    // If we are calibrating colors
    __calibrating(best_color);
#else
    // Compare the reported color with the pre-defined colors in the colorList
    // and set the result to the color with the least color variance, if the
    // variance is still under the allowed variance range (C_ALLOWED_COLOR_VARIANCE)

    int min_diff = C_ALLOWED_COLOR_VARIANCE; // Set the mininum difference to the allowed color variance
    int temp_result = -1;
    for (int i = 0; i < sizeof(colorList) / 8; i++) {
      // Compare the reported color with the color defined in the colorList
      C_Color diff = best_color.compare(colorList[i]);

      // Add the color difference's primary colors (R + G + B) together
      int agg = diff.aggregate();
      // Check if the aggregated color difference value is less than the minimum color difference
      if (agg < min_diff) {
        // If this is less than the minimun
        min_diff = agg; // Set the minimun difference to this aggregated color difference
        temp_result = i; // Set the result to this color's index
      }
    }

    // Check if the clear value is out of allowed range
    if (temp_result > -1 && abs(int(best_color.c) - int(colorList[temp_result].c)) > C_ALLOWED_CLEAR_VARIANCE) {
      // If the clear value is out of range, then set this result back to unknown
      temp_result = -1;
    }

    // If we've got a temp result
    if (temp_result != -1) {
      colorResults[skittleCount] = temp_result;
    }

    if (!HAS_RESULT(colorResults[skittleCount])) {
      // If color cannot be detected, try again
      servoTop.remeasureColor();
    }else{
      if(servoTop.isRemeasuring()){
        Serial.println("We've got the color on re-measurement.");
        servoTop.stopRemeasuring();
      }
    }
#endif
  }
  // Serial.println("Finish measuring.");
  isColorBeingMeasured = false;
  if (!servoTop.isRemeasuring() && HAS_RESULT(colorResults[skittleCount])) {
    skittleCount++;
  }
}

void updateColorSensor() {
  // The best colors during a measuring cycle
  static uint16_t min_clear;
  static C_Color best_color = C_Color(0, 0 , 0);
  // Get color measurement values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  // Create a C_Color object from raw colors
  C_Color colors = C_Color::createFromRawColors(C_CYCLES, r, g, b, c);
  // Check if the hole arrives at the color sensor
  if (colors.c < C_HOLE_CLEAR && !isColorBeingMeasured && millis() - lastSkittleTime > 1000) {
    //Serial.println(millis() - lastSkittleTime);
    lastSkittleTime = millis();
    // Serial.println("Start measuring this Skittle's color.");
    isColorBeingMeasured = true;
    min_clear = C_HOLE_CLEAR;
  }
  // Check if a Skittle's color is being measured right now
  if (isColorBeingMeasured) {
    // Try and reach the ideal clear value
    if (colors.c < min_clear && colors.c > C_IDEAL_CLEAR) {
      // The best color sample is picked when the clear is minimum while still larger than C_IDEAL_CLEAR
      min_clear = colors.c;
      best_color = colors;
    }
    // If the skittle moves away and there's nothing left for the color sensor there
    if (colors.c > C_HOLE_CLEAR || (colors.c - min_clear) > C_ALLOWED_COLOR_VARIANCE) {
      best_color.maximize(); // Maximize color
      best_color.print();

      // Analyze the color
      analyzeColor(best_color);
    }
  }

  // printColors(r, g, b, c);
}

void updateColorView() {
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

void loop() {
  lcd.clear();
  lcd.print("Count: ");
  lcd.print(skittleCount, DEC);

  // Update the top servo
  servoTop.update(lastSkittleTime);

  // Update the bottom servo (arm)
  updateBtmServo();

  // Update the color sensor
  updateColorSensor();

  // Update the color view LED output
  updateColorView();

}
