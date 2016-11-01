/*
  TSL1402R.h - Library for reading TSL1402R linear photodiode array
  Created by Douglas Mayhew, September 31, 2016.
  Released into the public domain.
*/
#ifndef TSL1402R_h
#define TSL1402R_h

#include "Arduino.h"
#include "ADC.h" 	// Teensy ADC library

class TSL1402R
{
  public:
    TSL1402R(int CLKpin, int SIpin, int Apin1, int Apin2);
    uint16_t ExposureMicroseconds;
    void read(uint8_t * data, uint32_t len);
  private:
    int _CLKpin;
    int _SIpin;
    int _Apin1;
    int _Apin2;
    uint16_t _sample1;
    uint16_t _sample2;
    void initTSL1402R();
};


#endif