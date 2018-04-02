#ifndef UserTypes_h
#define UserTypes_h
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h> 


// User data types.  Modify for your data items.
#define FILE_BASE_NAME "adcreading"
#define NUM_TLC59711 1
#define dataTLC   11
#define clock  13


const uint8_t ADC_DIM = 1;
struct data_t {
  uint32_t time;
  uint16_t adc[ADC_DIM];
};
void startADC();
void acquireData(data_t* data);
void printData(Print* pr, data_t* data);
void printHeader(Print* pr);
void userSetup();
#endif  // UserTypes_h
