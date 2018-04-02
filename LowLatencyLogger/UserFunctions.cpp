#include "UserTypes.h"

#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_ADS1015.h>


Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x48

// User data functions.  Modify these functions for your data items.

// Start time for data
static uint32_t startMicros;
void startADC(){
ads1115.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
ads1115.begin();
}

// Acquire a data record.
void acquireData(data_t* data) {
  data->time = micros();
  for (int i = 0; i < ADC_DIM; i++) {
    data->adc[i] = ads1115.readADC_Differential_0_1();
  //  print(data->adc[i]);
  }
}

// Print a data record.
void printData(Print* pr, data_t* data) {
  if (startMicros == 0) {
    startMicros = data->time;
  }
  pr->print(data->time - startMicros);
  for (int i = 0; i < ADC_DIM; i++) {
    pr->write(',');
    pr->print(data->adc[i]);
  }
  pr->println();
}

// Print data header.
void printHeader(Print* pr) {
  startMicros = 0;
  pr->print(F("micros"));
  for (int i = 0; i < ADC_DIM; i++) {
    pr->print(F(",adc"));
    pr->print(i);
  }
  pr->println();
}

// Sensor setup
void userSetup() {
}
