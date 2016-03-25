/**************************************************************************/
/**
    Color Sensor

    @file     ColorSensor.cpp
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>

    @section  Circuit
    - Color Sensor
      + SDA pin to analog pin 4
      + SCL pin to analog pin 5
*/
/**************************************************************************/
// Change the following line to enter calibration mode
#define F_CALIBRATING             false         /**< Is calibration in progress */
#define F_CALI_EMPTY_HOLE         false         /**< Is calibrating the empty hole's color */

// Constants
#define C_HOLE_CLEAR              300           /**< The clear value used to determine the hole's arrival */
#define C_IDEAL_CLEAR             96            /**< The ideal clear value for color detection */
#define C_ALLOWED_COLOR_VARIANCE  64            /**< Allowed color variance for color detection */
#define C_ALLOWED_CLEAR_VARIANCE  128           /**< Allowed clear variance for color detection */

#define C_ATIME TCS34725_INTEGRATIONTIME_154MS  /**< Color sensor integration time 154ms */
#define C_CYCLES (256 - C_ATIME)                /**< Color sensor cycles according to the integration time */

#include "ColorSensor.h"
#include "TopServo.h"
#include "LCD.h"

ColorSensor::ColorSensor() : Adafruit_TCS34725(C_ATIME, TCS34725_GAIN_1X)
{
  lastSkittleTime = 0;
  isMeasuring = false;
}

void ColorSensor::setup() {
  if (!this->begin()) {
    // If the color sensor failed to initialize, print out an error
    Serial.println("Error: Color sensor not found.");
    lcd.setTopText("ERR: Color Sensor Connection");

    // Infinite loop so the program won't continue
    while (1);
  }
}

void ColorSensor::update()
{
  static C_Color bestColor(0, 0, 0);  // The best colors during a measuring cycle
  static uint16_t minClear;           // The minimum clear value during a measuring cycle
  uint16_t r, g, b, c;                // Red, green, blue and clear channel values

  // Reads the raw color values
  getRawData(&r, &g, &b, &c);

  // Create a C_Color object from raw colors
  C_Color colors = C_Color::createFromRawColors(C_CYCLES, r, g, b, c);
  //colors.print();

  // Check if the hole arrives at the color sensor
  if (colors.c < C_HOLE_CLEAR && !isMeasuring && millis() - lastSkittleTime > 1000) {
    //Serial.println(millis() - lastSkittleTime);
    lastSkittleTime = millis();
    // Serial.println("Start measuring this Skittle's color.");
    isMeasuring = true;
    minClear = C_HOLE_CLEAR;
    bestColor = colors;
  }

  // Check if a Skittle's color is being measured right now
  if (isMeasuring) {
    // Try and reach the ideal clear value
    if (colors.c < minClear && colors.c > C_IDEAL_CLEAR) {
      // The best color sample is picked when the clear is minimum while still larger than C_IDEAL_CLEAR
      minClear = colors.c;
      bestColor = colors;
    }
    // If the skittle moves away and there's nothing left for the color sensor there
    if (colors.c > C_HOLE_CLEAR || (colors.c - minClear) > C_ALLOWED_COLOR_VARIANCE) {
      bestColor.maximize(); // Maximize color
      bestColor.print();

      // Analyze the color
      _analyzeColor(bestColor);

      if (!servoTop.isRemeasuring() && HAS_RESULT(colorResults[skittleCount])) {
        // Add 1 to the skittleCount if the color is not being re-measured and we've got the result
        skittleCount++;
        // Set the new Skittle count to the LCD screen
        lcd.setTopText(String("Count:") + skittleCount);
      }

      // Serial.println("Finish measuring.");
      isMeasuring = false;

    }
  }

  // printColors(r, g, b, c);
}

void ColorSensor::_calibrating(const C_Color& new_color) {
  static C_Color avg_c(new_color);  // The average color (result)
  static int count = 0;             // The amount of valid data samples

  // Get the color variance between the new color and the avarage color
  int diff = avg_c.compare(new_color).aggregate();
  Serial.print("Diff: "); Serial.println(diff);

  // Check if the color variance is out of a valid range
  if (diff > C_ALLOWED_COLOR_VARIANCE) {
    Serial.println("Difference too big. This sample data is discarded.");
  } else {
    // If the new valid is valid, set the average color to include the new data
    avg_c.r = (avg_c.r * count + new_color.r) / (count + 1);
    avg_c.g = (avg_c.g * count + new_color.g) / (count + 1);
    avg_c.b = (avg_c.b * count + new_color.b) / (count + 1);
    avg_c.c = (avg_c.c * count + new_color.c) / (count + 1);
    count++;
    Serial.println("Average color:");
    avg_c.print();
  }
}

void ColorSensor::_analyzeColor(const C_Color& bestColor)
{
  bestColor.print();

  // Use a macro to generate a constant array colorList
  // whose indexes correspond to the colorResult enum
#define COLOR_DEF( identifier, name, color, color_view )  color
  const C_Color colorList [] = { COLORS_DEFINITION };
#undef COLOR_DEF
  // Compare the best color with the colorList and get the closest result
  colorResult tempResult = bestColor.compareWithColorList(colorList, C_ALLOWED_COLOR_VARIANCE);


  if (!F_CALI_EMPTY_HOLE && tempResult == RESULT_EMPTY) {
    // If this is an empty hole.
    Serial.println("Empty hole");
    if (servoTop.isRemeasuring()) {
      servoTop.stopRemeasuring();
    }
  } else {
    Serial.println("A Skittle has been detected!");
    //Serial.println("Maximized colors:");
#if F_CALIBRATING
    // If we are calibrating colors
    _calibrating(bestColor);
#else

    // Check if the clear value is out of allowed range
    if (HAS_RESULT(tempResult) && abs(int(bestColor.c) - int(colorList[tempResult].c)) > C_ALLOWED_CLEAR_VARIANCE) {
      // If the clear value is out of range, then set this result back to unknown
      tempResult = RESULT_UNKNOWN;
    }

    // If we've got a temp result
    if (HAS_RESULT(tempResult)) {
      colorResults[skittleCount] = tempResult;
    }

    if (!HAS_RESULT(colorResults[skittleCount])) {
      // If color cannot be detected, try again
      servoTop.remeasureColor();
    } else {
      if (servoTop.isRemeasuring()) {
        Serial.println("We've got the color on re-measurement.");
        servoTop.stopRemeasuring();
      }
    }
#endif
  }
}

unsigned long ColorSensor::getLastSkittleTime() {
  return lastSkittleTime;
}

