#include <Wire.h> 
#include <Adafruit_ADS1015.h>


Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x48


void setup(void)
{
Serial.begin(9600);
Serial.println("Hello!");

Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
Serial.println("ADC Range: +/- 6.144V (1 bit = 188uV)");
ads1115.setGain(GAIN_SIXTEEN);
ads1115.begin();
}

void loop(void)
{
int16_t results;

results = ads1115.readADC_Differential_0_1();
//float conversion=results*(188E-6);
float conversion=results*(.0078125E-3);
//Serial.print("Differential: "); 
//Serial.print(results); Serial.print("("); 
Serial.print(conversion,4); Serial.println("V");

delay(1000);
}
