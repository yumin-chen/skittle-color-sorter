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

#define C_HOLE_CLEAR 256 // The clear value used to determine the hole's arrival
#define C_SKITTLE_CLEAR 160 // The clear value used to determine the Skittle's arrival

#define F_CALIBRATING false // Is calibration in progress

const boolean topServoTimeoutEnable = true; // Enable the timeout for the top servo
const int topServoTimeout = 5000; // Timeout to determine if the top servo is stuck

// Colors
typedef struct{
  uint16_t r;
  uint16_t g;
  uint16_t b;
} c_color;

c_color rgb(uint16_t r, uint16_t g, uint16_t b){
  c_color c;
  c.r = r; c.g = g; c.b = b;
  return c;
}
// Calibrated Skittle's Colors
// These are the skittle's colors 
#define C_COLOR_GREEN rgb(64, 109, 51) 

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

void standardizeColors(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c){
  /* Standardize colors to the unit 8-bits color standard where the 
   *  maximum color value is 256. 
  */
  *r /= C_CYCLES; *g /= C_CYCLES; *b /= C_CYCLES; *c /= C_CYCLES;
}

void maximizeColors(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c){
  /* Maximize colors based on the clear value.
  */
  float scale = 256.0 / *c;
  Serial.print("scale");Serial.print(scale);
  *r *= scale; *g *= scale; *b *= scale;
}

void __calibrating(uint16_t r, uint16_t g, uint16_t b){
  /* This function is used to test and get the average color 
   *  of a certain Skittle
  */
  static uint16_t a_r = r, a_g = g, a_b = b, count = 0;
  //if(abs(r - a_r) > 64 || abs(g - a_g) > 64 || abs(b - a_b) > 64 ){
  //  Serial.println("Difference too big. This data is discarded.");
  //}else{
    a_r = (a_r * count + r) / (count + 1);
    a_g = (a_g * count + g) / (count + 1);
    a_b = (a_b * count + b) / (count + 1);
    count++;
    Serial.println("Average color:");
    printColors(a_r, a_g, a_b, 0);
  //}
  
  
}

int checkColor(c_color c){
  c_color c2 = C_COLOR_GREEN;
  uint16_t color_diff_r = abs(int(c.r) - int(c2.r));
  uint16_t color_diff_g = abs(int(c.g) - int(c2.g));
  uint16_t color_diff_b = abs(int(c.b) - int(c2.b));
  Serial.println("Color diff:");
  printColors(color_diff_r, color_diff_g, color_diff_b, 0);
  return 0;
}

void printColors(uint16_t r, uint16_t g, uint16_t b, uint16_t c){
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}

void updateColorSensor() {
  // Is the color sensor measuring the Skittle right now
  static boolean isColorBeingMeasured = false;
  // The best colors during a measuring cycle
  static uint16_t min_clear, best_r, best_g, best_b;
  // Get color measurement values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  // Convert colors to the 8-bits color standard values
  standardizeColors(&r, &g, &b, &c);
  
  // Check if the hole arrives at the color sensor
  if (c < C_HOLE_CLEAR && !isColorBeingMeasured) {
      Serial.println(millis() - lastSkittleTime);
      lastSkittleTime = millis();
      // Serial.println("Start measuring this Skittle's color.");
      isColorBeingMeasured = true;
      min_clear = C_HOLE_CLEAR;// C_SKITTLE_CLEAR;
  }
  if(isColorBeingMeasured){
    if(c < min_clear){
      min_clear = c;
      best_r = r;
      best_g = g;
      best_b = b;
    }

     if (c > C_HOLE_CLEAR) {
          // Check if there was enough data to tell the skittle's color
          if(min_clear < C_SKITTLE_CLEAR){
              skittleCount ++;
              Serial.println("A Skittle has been detected!");
              printColors(best_r, best_g, best_b, min_clear);
              uint16_t m_r = best_r, m_g = best_g, m_b = best_b;
              maximizeColors(&m_r, &m_g, &m_b, &min_clear);
              Serial.println("Maximized colors:");
              printColors(m_r, m_g, m_b, min_clear);
              if(F_CALIBRATING){
                __calibrating(m_r, m_g, m_b);
              }
              int result = checkColor(rgb(m_r, m_g, m_b));

          }
          else{
              // Serial.println("The hole is empty!!");
              // printColors(best_r, best_g, best_b, min_clear);
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
