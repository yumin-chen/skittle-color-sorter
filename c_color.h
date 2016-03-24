/*
  C_Color

  This class defines some of the color operations for the Skittle Color Sorter

*/
#ifndef  _C_COLOR_H_
#define  _C_COLOR_H_

#include <Arduino.h>

#define COLOR_UNIT uint8_t

class C_Color 
{
public:
  C_Color(COLOR_UNIT red, COLOR_UNIT green, COLOR_UNIT blue, COLOR_UNIT clear = 0);
  static C_Color createFromRawColors(int cycles, uint16_t red, uint16_t green, uint16_t blue, uint16_t clear = 0);
  C_Color compare(C_Color);
  void maximize();
  int aggregate() const;
  void print() const;
  COLOR_UNIT r;
  COLOR_UNIT g;
  COLOR_UNIT b;
  COLOR_UNIT c;

private:
};

#endif // _C_COLOR_H_ 