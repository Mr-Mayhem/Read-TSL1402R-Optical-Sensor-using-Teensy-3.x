/*
   - Demo Sketch for reading TSL1402R linear photodiode array
  Uses TSL1402R.h class library for Teensy 3.x by Douglas Mayhew

  Use with PC running matching Processing app to xy plot the pixel values
  
  Created by Douglas Mayhew, September 31, 2016.
  Released into the public domain.
*/
#include <TSL1402R.h>
#include <ADC.h> // https://github.com/pedvide/ADC
//========================================================================================
// Pins for the TSL1402R sensor.
#define CLKpin 24 // <-- Teensy 3.6 pin delivering the clock pulses to pin 3(CLK) of the TSL1402R
#define SIpin  25 // <-- Teensy 3.6 pin delivering the SI (serial-input) pulse to pin 2 of the TSL1402R
#define Apin1  14 // <-- Teensy 3.6 pin connected to pin 4 (analog output 1) of the TSL1402R
#define Apin2  39 // <-- Teensy 3.6 pin connected to pin 8 (analog output 2) (parallel mode only)
//========================================================================================
ADC *adc = new ADC(); // adc object;
ADC::Sync_result ADCresult; // makes Teensy ADC library read 2 pins at the same time using 2 seperate ADCs
//========================================================================================
#define PREFIX 0xff
uint8_t sensorByteArray[512];

void initADC()
{
    // This code is used to init the Teensy ADC library. We use this to gain more control, faster read, 
    // and ability to read two pixel values at the same moment, cutting read time in half.
    ///// ADC0 ////
    // reference can be ADC_REF_3V3, ADC_REF_1V2 (not for Teensy LC) or ADC_REF_EXT.
    //adc->setReference(ADC_REF_1V2, ADC_0); // change all 3.3 to 1.2 if you change the reference to 1V2

    adc->setAveraging(1); // set number of averages
    adc->setResolution(12); // set bits of resolution

    // it can be ADC_VERY_LOW_SPEED, ADC_LOW_SPEED, ADC_MED_SPEED, ADC_HIGH_SPEED_16BITS, ADC_HIGH_SPEED or ADC_VERY_HIGH_SPEED
    // see the documentation for more information
    adc->setConversionSpeed(ADC_HIGH_SPEED); // change the conversion speed
    // it can be ADC_VERY_LOW_SPEED, ADC_LOW_SPEED, ADC_MED_SPEED, ADC_HIGH_SPEED or ADC_VERY_HIGH_SPEED
    adc->setSamplingSpeed(ADC_HIGH_SPEED); // change the sampling speed

    //adc->enableInterrupts(ADC_0);

    // always call the compare functions after changing the resolution!
    //adc->enableCompare(1.0/3.3*adc->getMaxValue(ADC_0), 0, ADC_0); // measurement will be ready if value < 1.0V
    //adc->enableCompareRange(1.0*adc->getMaxValue(ADC_0)/3.3, 2.0*adc->getMaxValue(ADC_0)/3.3, 0, 1, ADC_0); // ready if value lies out of [1.0,2.0] V

    ////// ADC1 /////
    #if ADC_NUM_ADCS>1
    adc->setAveraging(1, ADC_1); // set number of averages
    adc->setResolution(12, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_HIGH_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_HIGH_SPEED, ADC_1); // change the sampling speed

    // always call the compare functions after changing the resolution!
    //adc->enableCompare(1.0/3.3*adc->getMaxValue(ADC_1), 0, ADC_1); // measurement will be ready if value < 1.0V
    //adc->enableCompareRange(1.0*adc->getMaxValue(ADC_1)/3.3, 2.0*adc->getMaxValue(ADC_1)/3.3, 0, 1, ADC_1); // ready if value lies out of [1.0,2.0] V
    #endif
}
// arguments are CLKpin, SIpin, Apin1, Apin2
TSL1402R Sensor(CLKpin, SIpin, Apin1, Apin2);  //sensor object

void setup() 
{
  Serial.begin(115200);
  initADC();
  Sensor.ExposureMicroseconds = 500;
}

void loop() 
{
  delay(5);
  Sensor.read(sensorByteArray, 512);
  Serial.write(PREFIX); // PREFIX
  Serial.write(sensorByteArray, 512);
}
