<img src="http://raw.github.com/CharmySoft/skittle-color-sorter/master/images/icon.png" width="48"/>&nbsp;&nbsp;**Skittle Color Sorter**
========================
Details of this project can be found on the [Skittle Color Sorter project page][2] under:  
[*http://www.CharmySoft.com/app/skittle-sorter.htm*][2]

Introduction
------------------------
[Skittle Color Sorter][2] is an Arduino UNO board sketch that programs a color sorting machine that can sort Skittles by their color.  
This is originally for our 3<sup>rd</sup> year course Project in Computer Engineering (BE). 


Instruction
------------------------
This [Skittle Color Sorter][2] consists of a [Adafruit TCS34725 Color Sensor](http://learn.adafruit.com/adafruit-color-sensors) to measure color values, a [Continuous Rotation Servo](http://learn.adafruit.com/adafruit-motor-selection-guide/continuous-rotation-servos) at the top to keep the Skittles going down, a [Standard Servo](http://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors) at the bottom to move the arm to bring the Skittles to a desired place, and a [LCD Display](http://learn.adafruit.com/adafruit-arduino-lesson-11-lcd-displays-1/) to show information like the number of Skittles.  

The circuit:  

| Color Sensor          | Top Servo      | Bottom Servo   | LCD Display             |
|:---------------------:|:--------------:|:--------------:|:-----------------------:|
| SDA - analog pin 4    | Digital pin 13 | Digital pin 10 | RS - digital pin 12     |
| SCL - analog pin 5    |                |                | Enable - digital pin 11 |
| Red - diginal pin 7   |                |                | D4 - digital pin 5      |
| Green - diginal pin 9 |                |                | D5 - digital pin 4      |
| Blue - diginal pin 6  |                |                | D6 - digital pin 3      |
|                       |                |                | D7 - digital pin 2      |
|                       |                |                | R/W - ground            |  

![Schematic](/images/schematic.jpg?raw=true "Schematic")  

Because the color sensor might report color values differently due to various factors, such as the lighting condition, the distance between the sensor and the object, etc, before this [Skittle Color Sorter][2] can work properly for your specific set of Skittles, each color needs to be calibrated.  

To use the sketch in calibration mode, uncomment this line at the head of [skittle-color-sorter.ino](skittle-color-sorter.ino).
```c
// Uncomment the following line to enter calibration mode
#define F_CALIBRATING true // Is calibration in progress
```

When calibrating a specific color of Skittles, only put that one specific color of Skittles into the machine, and open the Serial Monitor to see the reported average color values. Test at least 10 different Skittles to ensure the calibration accurarcy. When you get the results, replace the Calibrated Skittle's Colors at the top of [skittle-color-sorter.ino](skittle-color-sorter.ino) with your own calibrated color values.
```c
// Calibrated Skittle's Colors
// These are the skittle's colors
#define C_COLOR_EMPTY C_Color(61, 88, 72)
#define C_SKITTLE_GREEN C_Color(64, 109, 51)
#define C_SKITTLE_RED C_Color(97, 62, 55)
#define C_SKITTLE_YELLOW C_Color(88, 91, 36)
#define C_SKITTLE_PURPLE C_Color(65, 74, 77)
#define C_SKITTLE_ORANGE C_Color(107, 66, 40)

const C_Color colorList [] = {C_SKITTLE_RED, C_SKITTLE_GREEN, C_SKITTLE_YELLOW, C_SKITTLE_PURPLE, C_SKITTLE_ORANGE};
```

When calibration is done, comment out the calibration mode flag.
```c
// Uncomment the following line to enter calibration mode
// #define F_CALIBRATING true // Is calibration in progress
```

Upload the updated sketch, and the [Skittle Color Sorter][2] should be up and running.


Licensing
------------------------
Please see the file named [LICENSE.md](LICENSE.md).


Author
------------------------
* Charlie Chen  
	founder of [CharmySoft][1]  
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