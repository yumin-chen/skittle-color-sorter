/**************************************************************************/
/**
    Context (Global Variables)

    This file is to provide access to some of the useful macros and global
    variables. This static class Context is the base class for all the
    components used in this project.

    @file     Context.h
    @author   Charlie Chen (CharmySoft)  <Charlie@CharmySoft.com>
*/
/**************************************************************************/
#ifndef  _CONTEXT_H_
#define  _CONTEXT_H_

/** Calibrated Skittle's Colors. All the colors are defined here. This
    macro will later be used to populate some arrays by changing the
    COLOR_DEF to retrive only needed data. */
#define COLORS_DEFINITION \
  /*       | Identifier   | Color Name      | Calibrated Color          | Color View        */\
  COLOR_DEF( RESULT_RED,		"Red Skittle",		C_Color(97, 73, 62, 117),	  C_Color(255, 0, 0) ),\
  COLOR_DEF( RESULT_GREEN,	"Green Skittle",	C_Color(58, 106, 54, 148),	C_Color(0, 255, 0) ),\
  COLOR_DEF( RESULT_YELLOW,	"Yellow Skittle",	C_Color(90, 90, 34, 210),	  C_Color(128, 255, 0) ),\
  COLOR_DEF( RESULT_PURPLE,	"Purple Skittle",	C_Color(65, 84, 81, 122),	  C_Color(128, 192, 32) ),\
  COLOR_DEF( RESULT_ORANGE,	"Orange Skittle",	C_Color(122, 64, 40, 154),	C_Color(255, 255, 0) ),\
  /* Don't forget to exclude these invalid colors in the HAS_COLOR macro function */\
  COLOR_DEF( RESULT_EMPTY,	"Empty hole",	    C_Color(64, 89, 71, 172),	  C_Color(0, 0, 0) )

/** This macro is retrive only the Identifier from the colors definition to
    generate the enum colorResult. */
#define COLOR_DEF( identifier, name, color, color_view )  identifier

/** This type represents a color's result. Note it not only can be a valid
    Skittle's color, but it also can be some invalid values. */
typedef enum : uint8_t
{
  COLORS_DEFINITION,
  RESULT_UNKNOWN			/* Add a special key for unknown colors */
} colorResult;

#undef COLOR_DEF

/** The size of the color list. This is also equal to the number of items
    in the enum colorResult. */
#define COLOR_LIST_SIZE RESULT_UNKNOWN

/** A macro function to check if the result is a valid color. */
#define HAS_RESULT(result) (result != RESULT_UNKNOWN && result != RESULT_EMPTY)

// Declare some of the components
class LCD;
class TopServo;
class BottomServo;
class ColorView;
class ColorSensor;

/** This static class is the base class for all the components used in this
    project. It works as a helper class that gives references to all the
    global variables. */
class Context {
  public:
    static LCD lcd; 						            /**< The liquid-crystal display                         */
    static TopServo servoTop; 			  	    /**< The top continuous rotation servo                  */
    static BottomServo servoBtm; 		  	    /**< The bottom standard servo                          */
    static ColorView colorView;			       	/**< The color LED                                      */
    static ColorSensor colorSensor; 	    	/**< The Adafruit color sensor                          */
    
    static int skittleCount; 				        /**< The number of skittles sorted                      */
    static colorResult colorResults [128]; 	/**< An array to store all the measured colors          */
  private:
};

#endif // _CONTEXT_H_ 
