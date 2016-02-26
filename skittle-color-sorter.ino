/*
  Skittle Color Sorter

  This sketch programs the Skittle Color Sorter that we
  assembled.
*/

// The circuit:
// * Top Servo
//    Digital pin 13
const int pinTopServo = 13;
// * Bottom Servo (Arm)
//    Digital pin 10
const int pinBtmServo = 10;
// * Color Sensor
//    SDA pin to analog pin 4
//    SCL pin to analog pin 5
//    Color View Red pin to diginal pin 7
//    Color View Green pin to diginal pin 9
//    Color View Blue pin to diginal pin 6
// * LCD
//    RS pin to digital pin 12
const int pinLcdRS = 12;
//    Enable pin to digital pin 11
const int pinLcdEnable = 11;
//    LCD D4 pin to digital pin 5
const int pinLcdD4 = 5;
//    LCD D5 pin to digital pin 4
const int pinLcdD5 = 4;
//    LCD D6 pin to digital pin 3
const int pinLcdD6 = 3;
//    LCD D7 pin to digital pin 2
const int pinLcdD7 = 2;
//    LCD R/W pin to ground


// Constants
const boolean topServoTimeoutEnable = true; // Enable the timeout for the top servo
const int topServoTimeout = 6000; // Timeout to determine if the top servo is stuck
const int rewindingTime = 500; // Time in milliseconds when the top servo rewinds to solve jam

// Include libraries
#include <LiquidCrystal.h>      // LCD display
#include <Servo.h>              // Servos
#include "Adafruit_TCS34725.h"  // Color Sensor

// Initialize the display with the numbers of the interface pins
LiquidCrystal lcd(pinLcdRS, pinLcdEnable, pinLcdD4, pinLcdD5, pinLcdD6, pinLcdD7);
// Create the object for the top continuous rotation servo
Servo servoTop;
// Create the object for the bottom standard servo
Servo servoBtm;
// Create the color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Variables
int skittleCount = 0; // The number of skittles sorted
unsigned long lastSkittleTime = 0; // Time when the last skittle was being sorted
int topServoSpeed = 0; // The speed in which the top servo moves
boolean isColorBeingMeasured = false; // Is the color sensor measuring the Skittle right now


void setup() {
  // Set up the serial for debugging purposes
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Attach the servos on corresponding pins to the servo objects
  servoTop.attach(pinTopServo);
  servoBtm.attach(pinBtmServo);

  if(!tcs.begin()){
    // If the color sensor failed to initialize, print out an error
    lcd.print("ERR: Color Sensor Connection Error");
    Serial.println("ERR: Color Sensor connection can't be established.");
    while (1);
  }
  
  lcd.print("Color Sorter");

}

void updateTopServo(){
  // Is the servo going the opposite direction
  static boolean rewinding = false;

  // Set the speed of the top servo
  // 0 being full-speed clockwise
  // 90 being no movement
  // 180 being full-speed counterclockwise
  servoTop.write(rewinding? 180: topServoSpeed);

  // If color is being measured right now, skip the jam detection
  if(isColorBeingMeasured)
    return;

  // If timeout is NOT enabled, skip the jam detection
  if(!topServoTimeoutEnable)
    return;

  // If the top servo gets stuck, start rewinding
  if(millis() - lastSkittleTime > topServoTimeout){
    Serial.println("Top servo is stuck; Direction reversed.");
    // Reserve the top servo's direction
    rewinding = true;
    
    lastSkittleTime = millis();
  }

  // If rewinding more than the set time, set direction back to normal
  if(rewinding && millis() - lastSkittleTime > rewindingTime){
    rewinding = false;
  }
  
}

void updateBtmServo(){
  
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

void updateColorSensor(){
  // Get color measurement values
  uint16_t r, g, b, c;
  delay(100);  // takes 50ms to read
  tcs.getRawData(&r, &g, &b, &c);

  // If clear value is less than 300
  if(c < 300 && !isColorBeingMeasured){
    // This is when the color sensor sees into the hole
    
    if (millis() - lastSkittleTime > 1000){
      skittleCount ++;
      Serial.println(millis() - lastSkittleTime);
      lastSkittleTime = millis();

      // Slow down the top servo's speed for better accuracy
      topServoSpeed = 90;
      Serial.println("Start measuring this Skittle's color.");
      isColorBeingMeasured = true;
    }
    
  }

  if(c < 300){
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");
    
    if (millis() - lastSkittleTime > 1000){
      topServoSpeed = 0;
      Serial.println("Finish measuring.");
      isColorBeingMeasured = false;
      lastSkittleTime = millis();
    
    }
  }

/*
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
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
  lcd.print("Count: ");lcd.print(skittleCount, DEC);

  // Update the top servo
  //if(frames == 0)
  updateTopServo();
  
  // Update the bottom servo (arm)
  //if(frames == 0)
  //updateBtmServo();

  // Update the color sensor
  updateColorSensor();

  // Because the color sensor needs to measure the color more frequently, 
  // instead of delaying the main loop, which will affect the color sensor's
  // refresh rate, a new variable was introduced to control and manually 
  // delay certain components while keep the color sensor up and running.
  //frames ++;
}
