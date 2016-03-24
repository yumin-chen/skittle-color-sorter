/*
  Color Sensor

  This class defines the color sensor

*/

// The circuit:
// * Color Sensor
//    SDA pin to analog pin 4
//    SCL pin to analog pin 5

#define F_CALIBRATING false // Is calibration in progress
#define F_CALI_EMPTY_HOLE false // Is calibrating the empty hole's color

// Constants
#define C_ATIME TCS34725_INTEGRATIONTIME_154MS //  Color sensor integration time 154ms
#define C_CYCLES (256-C_ATIME) // Color sensor cycles according to the integration time

#define C_HOLE_CLEAR 300 // The clear value used to determine the hole's arrival
#define C_IDEAL_CLEAR 96 // The ideal clear value for color detection
#define C_ALLOWED_COLOR_VARIANCE 64 // Allowed color variance for color detection
#define C_ALLOWED_CLEAR_VARIANCE 128 // Allowed clear variance for color detection

#include "ColorSensor.h"
#include "TopServo.h"
#include "LCD.h"

ColorSensor::ColorSensor() : Adafruit_TCS34725(C_ATIME, TCS34725_GAIN_1X) // Initialize the color sensor object
{
}

void ColorSensor::setup(){
  /* Sets up the color sensor object
  */
  if (!this->begin()) {
    // If the color sensor failed to initialize, print out an error
    Serial.println("Error: Color sensor not found.");
    lcd.print("ERR: Color Sensor Connection");
    // Infinite loop so the program won't continue
    while (1);
  }
}

void ColorSensor::update(){
	/* This function should be called on the main loop. 
	*/

  // The best colors during a measuring cycle
  static uint16_t min_clear;
  // Get color measurement values
  uint16_t r, g, b, c;
  getRawData(&r, &g, &b, &c);
  // Create a C_Color object from raw colors
  C_Color colors = C_Color::createFromRawColors(C_CYCLES, r, g, b, c);
  //colors.print();
  // Check if the hole arrives at the color sensor
  if (colors.c < C_HOLE_CLEAR && !isColorBeingMeasured && millis() - lastSkittleTime > 1000) {
    //Serial.println(millis() - lastSkittleTime);
    lastSkittleTime = millis();
    // Serial.println("Start measuring this Skittle's color.");
    isColorBeingMeasured = true;
    min_clear = C_HOLE_CLEAR;
    best_color = colors;
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
      analyzeColor(); 
      // Serial.println("Finish measuring.");
      isColorBeingMeasured = false;
      if (!servoTop.isRemeasuring() && HAS_RESULT(colorResults[skittleCount])) {
        skittleCount++;
      }
    }
  }

  // printColors(r, g, b, c);
}

void ColorSensor::calibrating(C_Color new_color) {
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

void ColorSensor::analyzeColor() {
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
    calibrating(best_color);
#else
    // Compare the reported color with the pre-defined colors in the colorList
    // and set the result to the color with the least color variance, if the
    // variance is still under the allowed variance range (C_ALLOWED_COLOR_VARIANCE)

    int min_diff = C_ALLOWED_COLOR_VARIANCE; // Set the mininum difference to the allowed color variance
    colorResult temp_result = RESULT_UNKNOWN;
    for (int i = 0; i < sizeof(colorList) / 8; i++) {
      // Compare the reported color with the color defined in the colorList
      C_Color diff = best_color.compare(colorList[i]);

      // Add the color difference's primary colors (R + G + B) together
      int agg = diff.aggregate();
      // Check if the aggregated color difference value is less than the minimum color difference
      if (agg < min_diff) {
        // If this is less than the minimun
        min_diff = agg; // Set the minimun difference to this aggregated color difference
        temp_result = static_cast<colorResult>(i);; // Set the result to this color's index
      }
    }

    // Check if the clear value is out of allowed range
    if (HAS_RESULT(temp_result) && abs(int(best_color.c) - int(colorList[temp_result].c)) > C_ALLOWED_CLEAR_VARIANCE) {
      // If the clear value is out of range, then set this result back to unknown
      temp_result = RESULT_UNKNOWN;
    }

    // If we've got a temp result
    if (HAS_RESULT(temp_result)) {
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
}
