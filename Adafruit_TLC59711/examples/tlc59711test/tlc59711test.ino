/*************************************************** 
  This is an example for our Adafruit 12-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/

  These drivers uses SPI to communicate, 2 pins are required to  
  interface: Data and Clock. The boards are chainable

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/



#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_ADS1015.h>




// How many boards do you have chained?
#define NUM_TLC59711 1

#define data   11
#define clock  13

Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x48
Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clock, data);

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power

//Rather than powering the sensor through the 3.3V or 5V pins, 
//we'll use a digital pin to power the sensor. This will 
//prevent corrosion of the sensor as it sits in the soil. 

void setup() {
//BLUE WIRE FOR PD GOES TO NOTCH
//GREEN WIRE FOR LED GOES TO NOTCH
Serial.begin(9600);
//Soil Moisture Stuff
 pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
 
//ADC Stuff
Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
Serial.println("ADC Range: +/- 6.144V (1 bit = 188uV)");
Serial.println(" 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV");
ads1115.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
ads1115.begin();

//Driver Stuff
Serial.println("TLC59711 test");
pinMode(10, OUTPUT);
tlc.begin();
tlc.write();
}

void loop() {

//Saltwater switch check
int soilVal=readSoil();
Serial.println(soilVal);

//If in water, do all the driver/reading
//if(soilVal>300){
//For testing just run this branch regardless of the soil reading
if(1){
//Driver Stuff
colorWipe(0, 65535, 0, 100); // "Green" (depending on your LED wiring)

//ADC Stuff
int16_t results;
results = ads1115.readADC_Differential_0_1();
//results2 = ads1115.readADC_Differential_2_3();
//float conversion=results*(188E-6);
float conversion=results*(.0078125E-3);
//float conversion2=results2*(.0078125E-3);
//Serial.print("Differential: "); 
//Serial.print(results); Serial.print("("); 
Serial.print(conversion,6); 
//Serial.println(" ");  Serial.println(conversion2,6);
//delay(200); 
}
else{
  Serial.println("Out of water");
}
}


// Fill the dots one after the other with a color
void colorWipe(uint16_t r, uint16_t g, uint16_t b, uint8_t wait) {
  for(uint16_t i=0; i<8*NUM_TLC59711; i++) {
      tlc.setLED(i, r, g, b);
      tlc.write();
      delay(wait);
  }
}

//Read the soil moisture content
int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}


