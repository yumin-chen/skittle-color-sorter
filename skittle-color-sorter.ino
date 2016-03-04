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
#define C_ATIME TCS34725_INTEGRATIONTIME_101MS //  Color sensor integration time 50ms
#define C_CYCLES (256-C_ATIME) // Color sensor cycles according to the integration time

#define C_HOLE_CLEAR 300 // The clear value used to determine the hole's arrival

#define F_CALIBRATING false // Is calibration in progress

const boolean topServoTimeoutEnable = true; // Enable the timeout for the top servo
const int topServoTimeout = 5000; // Timeout to determine if the top servo is stuck

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

const C_Color colorList [] = {C_COLOR_EMPTY, C_SKITTLE_RED, C_SKITTLE_GREEN, C_SKITTLE_YELLOW, C_SKITTLE_PURPLE, C_SKITTLE_ORANGE};

// Initialize the display with the numbers of the interface pins
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_ENABLE, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
// Create the object for the top continuous rotation servo
Servo servoTop;
// Create the object for the bottom standard servo
Servo servoBtm;
// Create the color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(C_ATIME, TCS34725_GAIN_1X);

// The number of skittles sorted
int skittleCount = 0;
// Time when the last skittle was being sorted
unsigned long lastSkittleTime = 0;

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
  // servoTop.write(180);

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

void __calibrating(C_Color new_color){
  /* This function is used to test and get the average color 
   *  of a certain Skittle
  */
  static C_Color avg_c = C_Color(new_color.r, new_color.g, new_color.b);
  static int count = 0;
  int diff = avg_c.compare(new_color).aggregate();
  Serial.print("Diff: "); Serial.println(diff);
  if(diff > 64){
    Serial.println("Difference too big. This data is discarded.");
  }else{
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
  static C_Color best_color = C_Color(0, 0 ,0);
  // Get color measurement values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  // Create a C_Color object from raw colors
  C_Color colors = C_Color::createFromRawColors(C_CYCLES, r, g, b, c);
  // Check if the hole arrives at the color sensor
  if (colors.c < C_HOLE_CLEAR && !isColorBeingMeasured) {
      Serial.println(millis() - lastSkittleTime);
      lastSkittleTime = millis();
      // Serial.println("Start measuring this Skittle's color.");
      isColorBeingMeasured = true;
      min_clear = C_HOLE_CLEAR;
  }
  if(isColorBeingMeasured){
    if(colors.c < min_clear){
      min_clear = colors.c;
      best_color = colors;
    }

     if (c > C_HOLE_CLEAR) {

              best_color.maximize();
              if(best_color.compare(colorList[0]).aggregate() < 16){
                
                        Serial.println("Empty hole");
                
              }else{
                        skittleCount ++;
                        Serial.println("A Skittle has been detected!");
                        Serial.println("Maximized colors:");
                        best_color.print();


                              if(F_CALIBRATING){
                                __calibrating(best_color);
                              }else{
                                    
                                    int min_diff = 64;
                                    int result = -1;
                                    for(int i = 0; i < sizeof(colorList)/8; i++){
                                      C_Color diff = best_color.compare(colorList[i]);
                                      int agg = diff.aggregate();
                                      if(agg < min_diff){
                                        min_diff = agg;
                                        result = i;
                                      }
                                    }
                                    switch(result){
                                      case 0:
                                        Serial.println("Empty hole");
                                        break;
                                      case 1:
                                        Serial.println("RED");
                                        break;
                                      case 2:
                                        Serial.println("GREEN");
                                        break;
                                      case 3:
                                        Serial.println("YELLOW");
                                        break;
                                      case 4:
                                        Serial.println("PURPLE");
                                        break;
                                      case 5:
                                        Serial.println("ORANGE");
                                        break;
                                      default:
                                        Serial.println("Unknown color");
                                        break;
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

void loop() {
  lcd.clear();
  lcd.print("Count: "); 
  lcd.print(skittleCount, DEC);

  // Update the top servo
  updateTopServo();

  // Update the bottom servo (arm)
  //updateBtmServo();

  // Update the color sensor
  updateColorSensor();

  delay(100);
}
