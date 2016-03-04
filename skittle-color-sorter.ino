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
#define C_ATIME TCS34725_INTEGRATIONTIME_50MS // Integration time 50ms, 20 cycles, Max Count: 20480
#define C_CYCLES (256-C_ATIME) // The cycles according to the integration time

const boolean topServoTimeoutEnable = true; // Enable the timeout for the top servo
const int topServoTimeout = 6000; // Timeout to determine if the top servo is stuck


// Include libraries
#include <LiquidCrystal.h>      // LCD display
#include <Servo.h>              // Servos
#include "Adafruit_TCS34725.h"  // Color Sensor

// Initialize the display with the numbers of the interface pins
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
// Create the object for the top continuous rotation servo
Servo servoTop;
// Create the object for the bottom standard servo
Servo servoBtm;
// Create the color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

// The number of skittles sorted
int skittleCount = 0;
// Time when the last skittle was being sorted
unsigned long lastSkittleTime = 0;
// The speed in which the top servo moves
int topServoSpeed = 0;
// Is the color sensor measuring the Skittle right now
boolean isColorBeingMeasured = false;

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
  servoTop.write(reverseDirection ? 180 : topServoSpeed);
  // servoTop.write(180);

  // If color is being measured right now, skip the stuck detection
  if (isColorBeingMeasured)
    return;

  // If the top servo gets stuck
  if (topServoTimeoutEnable && millis() - lastSkittleTime > topServoTimeout) {
    Serial.println("Top servo is stuck; Direction reversed.");
    // Reserve the top servo's direction
    reverseDirection = true;

    lastSkittleTime = millis();
  }

  if (topServoTimeoutEnable && reverseDirection && millis() - lastSkittleTime > 500) {
    // Set direction back to normal after .5s
    reverseDirection = false;
  }

}

void updateBtmServo() {

  // Calculate the angle of the bottom servo arm
  static int btmAngle = 0;
  static bool btmReversed = false;
  btmAngle += btmReversed ? -10 : 10;

  // The bottom arm angle range from 30 to 150 degrees
  if (btmAngle >= 150)
    btmReversed = true;
  if (btmAngle <= 30)
    btmReversed = false;

  // Set the angle of the shaft (in degrees), ranging from 0 to 180
  //servoBtm.write(90);
  servoBtm.write(btmAngle);
}

void updateColorSensor() {
  // Get color measurement values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  r /= C_CYCLES; g /= C_CYCLES; b /= C_CYCLES; c /= C_CYCLES;

  // If clear value is less than 300
  if (c < 150 && !isColorBeingMeasured) {
    // This is when the color sensor sees into the hole

    if (millis() - lastSkittleTime > 1000) {
      skittleCount ++;
      Serial.println(millis() - lastSkittleTime);
      lastSkittleTime = millis();

      // Slow down the top servo's speed for better accuracy
    /*
        topServoSpeed = 90;
        Serial.println("Start measuring this Skittle's color.");
      isColorBeingMeasured = true;
     */

    }

  }

  if (c < 150) {
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");

    if (millis() - lastSkittleTime > 1000) {
      topServoSpeed = 0;
      Serial.println("Finish measuring.");
      isColorBeingMeasured = false;
      lastSkittleTime = millis();

    }
  }

    if (c < 300) {
      
    if (millis() - lastSkittleTime > 1000) 
      lastSkittleTime = millis();
    
    }

  /*
//    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  //  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");
  
*/


}

void loop() {
  // static byte frames = 0;

  lcd.clear();
  lcd.print("Count: "); lcd.print(skittleCount, DEC);

  // Update the top servo
  updateTopServo();

  // Update the bottom servo (arm)
  //updateBtmServo();

  // Update the color sensor
  updateColorSensor();

  delay(100);
}
