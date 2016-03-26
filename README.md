<img src="http://raw.github.com/CharmySoft/skittle-color-sorter/master/images/icon.png" width="48"/>&nbsp;&nbsp;**Skittle Color Sorter**
========================
Details of this project can be found on the [Skittle Color Sorter project page][2] under:  
[*http://www.CharmySoft.com/app/skittle-sorter.htm*][2]

Introduction
------------------------
[Skittle Color Sorter][2] is an Arduino UNO board sketch that programs a color sorting machine that can sort Skittles by their color.  
This is originally for our 3<sup>rd</sup> year course Project in Computer Engineering (BE). 

<div style="text-align:center"><img src="http://www.charmysoft.com/img/prod/pic_skittle.svg" align="center" height="400" /></div>  


Instructions
------------------------
This [Skittle Color Sorter][2] consists of a [Adafruit TCS34725 Color Sensor](http://learn.adafruit.com/adafruit-color-sensors) to measure color values, a [Continuous Rotation Servo](http://learn.adafruit.com/adafruit-motor-selection-guide/continuous-rotation-servos) at the top to keep the Skittles going down, a [Standard Servo](http://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors) at the bottom to move the arm to bring the Skittles to a desired place, and a [LCD Display](http://learn.adafruit.com/adafruit-arduino-lesson-11-lcd-displays-1/) to show information like the number of Skittles.  

The circuit:  

| Color Sensor  | Top Servo | Bottom Servo | LCD Display     |
|:-------------:|:---------:|:------------:|:---------------:|
| SDA - pin 4   | Pin 13    | Pin 10       | RS - pin 12     |
| SCL - pin 5   |           |              | Enable - pin 11 |
| Red - pin 7  	|           |              | D4 - pin 5      |
| Green - pin 9 |           |              | D5 - pin 4      |
| Blue - pin 6  |           |              | D6 - pin 3      |
|               |           |              | D7 - pin 2      |
|               |           |              | R/W - ground    |  

![Schematic](/images/schematic.jpg?raw=true "Schematic")  

Because the color sensor might report color values differently due to various factors, such as the lighting condition, the distance between the sensor and the object, etc, before this [Skittle Color Sorter][2] can work properly for your specific set of Skittles, each color needs to be calibrated.  

To use the sketch in calibration mode, set the following macro definitions to true on [ColorSensor.cpp](http://github.com/CharmySoft/skittle-color-sorter/blob/a9967638f4a1e9fce1a7ef8ceb0b503633c83953/ColorSensor.cpp#L15).
```c
// Change the following line to enter calibration mode
#define F_CALIBRATING             true         /**< Is calibration in progress */
#define F_CALI_EMPTY_HOLE         true         /**< Is calibrating the empty hole's color */
```
You need to firstly set both of them to true, this will make the sketch program try and get the average color of the empty hole. Upload the sketch, open the Serial Monitor and let it run for a while, and you will have the average color of when the hole is empty. Update the the color definition of the empty hole on [Context.h](http://github.com/CharmySoft/skittle-color-sorter/blob/a9967638f4a1e9fce1a7ef8ceb0b503633c83953/Context.h#L27) with your own data.
```c
/** Calibrated Skittle's Colors. All the colors are defined here. This
    macro will later be used to populate some arrays by changing the
    COLOR_DEF to retrive only needed data. */
#define COLORS_DEFINITION \
  /*       | Identifier   | Color Name      | Calibrated Color          | Color View        */\
  COLOR_DEF( RESULT_RED,	"Red Skittle",		C_Color(97, 73, 62, 117),	C_Color(255, 0, 0) ),\
  COLOR_DEF( RESULT_GREEN,	"Green Skittle",	C_Color(58, 106, 54, 148),	C_Color(0, 255, 0) ),\
  COLOR_DEF( RESULT_YELLOW,	"Yellow Skittle",	C_Color(90, 90, 34, 210),	C_Color(128, 255, 0) ),\
  COLOR_DEF( RESULT_PURPLE,	"Purple Skittle",	C_Color(65, 84, 81, 122),	C_Color(128, 192, 32) ),\
  COLOR_DEF( RESULT_ORANGE,	"Orange Skittle",	C_Color(122, 64, 40, 154),	C_Color(255, 255, 0) ),\
  /* Don't forget to exclude these invalid colors in the HAS_COLOR macro function */\
  COLOR_DEF( RESULT_EMPTY,	"Empty hole",		C_Color(66, 89, 71, 170),	C_Color(0, 0, 0) )
```
Now, you've had the empty hole's color defined. You can now start calibrating other colors. Go back to the calibration mode macro definition on [ColorSensor.cpp](http://github.com/CharmySoft/skittle-color-sorter/blob/a9967638f4a1e9fce1a7ef8ceb0b503633c83953/ColorSensor.cpp#L16) and change the F_CALI_EMPTY_HOLE to false. This will make the sketch try and get the average color of a specific kind of Skittles, while the empty hole's color will be correctly ignored.
```c
// Change the following line to enter calibration mode
#define F_CALIBRATING             true         /**< Is calibration in progress */
#define F_CALI_EMPTY_HOLE         false         /**< Is calibrating the empty hole's color */
```
When calibrating a specific color of Skittles, only put that one specific color of Skittles into the machine, and open the Serial Monitor to see the reported average color values. Test at least 10 different Skittles to ensure the calibration accurarcy. When you get the results, replace the rest of the definition with your own calibrated color values.  

When calibration is done, comment out the calibration mode flag (or set it to false). Upload the updated sketch, and the [Skittle Color Sorter][2] should be up and running.


Licensing
------------------------
Please see the file named [LICENSE.md](LICENSE.md).


Author
------------------------
* Charlie Chen  
	contributor of the Arduino sketch code for this project  

* Paul Boyle  
	contributor of the hardware frame for this project  

* Paul Lawlor  
	contributor of the hardware frame for this project  


Contact
------------------------
* CharmySoft: [*http://www.CharmySoft.com/*][1]  
* About: [*http://www.CharmySoft.com/about.htm*][3]  
* Email: [*Charlie@CharmySoft.com*](mailto:Charlie@CharmySoft.com)  

[1]: http://www.CharmySoft.com/ "CharmySoft"
[2]: http://www.CharmySoft.com/app/skittle-sorter.htm "Skittle Color Sorter"
[3]: http://www.CharmySoft.com/about.htm "About CharmySoft"