/*
  Skittle Color Sorter

  This sketch programs the Skittle Color Sorter that we
  assembled.
*/

// The circuit:
// * Top Servo
//    Digital pin 13
#define PIN_TOP_SERVO 13
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


// Constants
#define C_ATIME TCS34725_INTEGRATIONTIME_154MS //  Color sensor integration time 154ms
#define C_CYCLES (256-C_ATIME) // Color sensor cycles according to the integration time

#define C_HOLE_CLEAR 300 // The clear value used to determine the hole's arrival
#define C_IDEAL_CLEAR 96 // The ideal clear value for color detection
#define C_ALLOWED_COLOR_VARIANCE 64 // Allowed color variance for color detection

#define F_TOP_SERVO_TIMEOUT_ENABLED true // Enable the timeout for the top servo
#define C_TOP_SERVO_TIMEOUT 6000 // Timeout to determine if the top servo is stuck

#define C_COLOR_SIGNAL_TIME 1000 // Each Skittle gets 1s color view signal

#define F_CALIBRATING false // Is calibration in progress



// Include libraries
#include <LiquidCrystal.h>      // LCD display
#include <Servo.h>              // Servos
#include "Adafruit_TCS34725.h"  // Color Sensor
#include "C_Color.h"            // Color operations

// Calibrated Skittle's Colors
// These are the skittle's colors
#define C_COLOR_EMPTY C_Color(61, 88, 72)
#define C_SKITTLE_GREEN C_Color(64, 109, 51)
#define C_SKITTLE_RED C_Color(97, 62, 55)
#define C_SKITTLE_YELLOW C_Color(88, 91, 36)
#define C_SKITTLE_PURPLE C_Color(65, 74, 77)
#define C_SKITTLE_ORANGE C_Color(107, 66, 40)

const C_Color colorList [] = {C_SKITTLE_RED, C_SKITTLE_GREEN, C_SKITTLE_YELLOW, C_SKITTLE_PURPLE, C_SKITTLE_ORANGE};

// Initialize the display with the numbers of the interface pins
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
// Create the object for the top continuous rotation servo
Servo servoTop;
// Create the object for the bottom standard servo
Servo servoBtm;
// Create the color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(C_ATIME, TCS34725_GAIN_1X);


int skittleCount = 0; // The number of skittles sorted
unsigned long lastSkittleTime = 0; // Time when the last skittle was being sorted
int sortingResult = -1; // The sorting result, this number is the index for colorList

void setup() {
  // Set up the serial for debugging purposes
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Attach the servos on corresponding pins to the servo objects
  servoTop.attach(PIN_TOP_SERVO);
  servoBtm.attach(PIN_BTM_SERVO);

  if (!tcs.begin()) {
    // If the color sensor failed to initialize, print out an error
    lcd.print("ERR: Color Sensor Connection");
    while (1);
  }

  lcd.print("Color Sorter");

}

void updateTopServo() {
  static boolean reverseDirection = false;
  // Set the speed of the top servo
  // with 0 being full-speed in one direction, 180 being full speed in the other, and a value near 90 being no movement.
  servoTop.write(reverseDirection ? 180 : 0);

  // If the top servo gets stuck
  if (F_TOP_SERVO_TIMEOUT_ENABLED && millis() - lastSkittleTime > C_TOP_SERVO_TIMEOUT) {
    Serial.println("Top servo is stuck; Direction reversed.");
    // Reserve the top servo's direction
    reverseDirection = true;

    lastSkittleTime = millis();
  }

  if (F_TOP_SERVO_TIMEOUT_ENABLED && reverseDirection && millis() - lastSkittleTime > 1000) {
    // Set direction back to normal after 1s
    reverseDirection = false;
  }

}

void updateBtmServo() {
  // Calculate the angle of the bottom servo arm
#define MAX_BTM_ANGLE 150 // Maximum bottom servo angle
#define MIN_BTM_ANGLE 30 // Minimum bottom servo angle
#define NUM_OF_CUPS 5 // The number of cups at the bottom
#define EACH_CUP_ANGLE_INTERVAL ((MAX_BTM_ANGLE - MIN_BTM_ANGLE) / NUM_OF_CUPS)
  int btmAngle = MIN_BTM_ANGLE + EACH_CUP_ANGLE_INTERVAL * sortingResult;
  servoBtm.write(btmAngle);
}

void __calibrating(C_Color new_color) {
  /* This function is used to test and get the average color
      of a certain Skittle
  */
  static C_Color avg_c = C_Color(new_color.r, new_color.g, new_color.b);
  static int count = 0;
  int diff = avg_c.compare(new_color).aggregate();
  Serial.print("Diff: "); Serial.println(diff);
  if (diff > C_ALLOWED_COLOR_VARIANCE) {
    Serial.println("Difference too big. This data is discarded.");
  } else {
    avg_c.r = (avg_c.r * count + new_color.r) / (count + 1);
    avg_c.g = (avg_c.g * count + new_color.g) / (count + 1);
    avg_c.b = (avg_c.b * count + new_color.b) / (count + 1);
    count++;
    Serial.println("Average color:");
    avg_c.print();
  }


}

void updateColorSensor() {
  // Is the color sensor measuring the Skittle right now
  static boolean isColorBeingMeasured = false;
  // The best colors during a measuring cycle
  static uint16_t min_clear;
  static C_Color best_color = C_Color(0, 0 , 0);
  // Get color measurement values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  // Create a C_Color object from raw colors
  C_Color colors = C_Color::createFromRawColors(C_CYCLES, r, g, b, c);
  // Check if the hole arrives at the color sensor
  if (colors.c < C_HOLE_CLEAR && !isColorBeingMeasured) {
    //Serial.println(millis() - lastSkittleTime);
    lastSkittleTime = millis();
    // Serial.println("Start measuring this Skittle's color.");
    isColorBeingMeasured = true;
    min_clear = C_HOLE_CLEAR;
  }
  if (isColorBeingMeasured) {
    if (colors.c < min_clear && colors.c > C_IDEAL_CLEAR) {
      min_clear = colors.c;
      best_color = colors;
    }

    if (colors.c > C_HOLE_CLEAR) {

      best_color.maximize(); // Maximize color

      if (best_color.compare(C_COLOR_EMPTY).aggregate() < 16) {
        // If this is an empty hole.
        Serial.println("Empty hole");
      } else {
        skittleCount ++;
        Serial.println("A Skittle has been detected!");
        //Serial.println("Maximized colors:");
        //best_color.print();

        if (F_CALIBRATING) {
          // If we are calibrating colors
          __calibrating(best_color);
        } else {

          int min_diff = C_ALLOWED_COLOR_VARIANCE;
          sortingResult = -1;
          for (int i = 0; i < sizeof(colorList) / 8; i++) {
            C_Color diff = best_color.compare(colorList[i]);
            int agg = diff.aggregate();
            if (agg < min_diff) {
              min_diff = agg;
              sortingResult = i;
            }
          }

        }

      }

      // Serial.println("Finish measuring.");
      isColorBeingMeasured = false;
      lastSkittleTime = millis();


    }
  }

  // printColors(r, g, b, c);


}

void updateColorView() {
  static int oldSortingResult = -1;
  if (sortingResult != oldSortingResult) {
    oldSortingResult = sortingResult;
    switch (sortingResult) {
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
      default:
        Serial.println("Unknown color");
        analogWrite(PIN_COLOR_RED, 0);
        analogWrite(PIN_COLOR_GREEN, 0);
        analogWrite(PIN_COLOR_BLUE, 0);
        break;
    }
  } else {
    // Check if the color view signal has lasted enough time
    if (millis() - lastSkittleTime > C_COLOR_SIGNAL_TIME) {
      // Set it back to black
      analogWrite(PIN_COLOR_RED, 0);
      analogWrite(PIN_COLOR_GREEN, 0);
      analogWrite(PIN_COLOR_BLUE, 0);
    }
  }
}

void loop() {
  lcd.clear();
  lcd.print("Count: ");
  lcd.print(skittleCount, DEC);

  // Update the top servo
  updateTopServo();

  // Update the bottom servo (arm)
  updateBtmServo();

  // Update the color sensor
  updateColorSensor();

  // Update the color view LED output
  updateColorView();

}
