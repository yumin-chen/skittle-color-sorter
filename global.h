/*
  Global variables
*/
#ifndef  _GLOBAL_H_
#define  _GLOBAL_H_

struct global {
  int skittleCount = 0; // The number of skittles sorted
  unsigned long lastSkittleTime = 0; // Time when the last skittle was being sorted
  boolean isColorBeingMeasured = false; // Is the color sensor measuring a Skittle right now
  byte colorResults [256]; // An array to store all the detected colors;
};

#endif // _GLOBAL_H_ 
