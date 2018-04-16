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



#include <Adafruit_TLC59711.h>
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_ADS1015.h>
#include <SD.h>

//Wifi
#include <SoftwareSerial.h>
#define RX 3
#define TX 2
//


// How many boards do you have chained?
#define NUM_TLC59711 1

#define data   11
#define clock  13

Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x48
Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clock, data);

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power
File file;
int count=0;
int ledPin = 5;
boolean donevar=false;

//Wifi Stuff
//String AP = "DukeOpen";       // CHANGE ME
//String PASS = ""; // CHANGE ME
//String API = "Z29L3NWYLD7JILHY";   // CHANGE ME
//String HOST = "api.thingspeak.com";
//String PORT = "80";
//String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

SoftwareSerial esp8266(RX, TX); 

//Rather than powering the sensor through the 3.3V or 5V pins, 
//we'll use a digital pin to power the sensor. This will 
//prevent corrosion of the sensor as it sits in the soil. 

void setup() {
//BLUE WIRE FOR PD GOES TO NOTCH
//GREEN WIRE FOR LED GOES TO NOTCH
Serial.begin(9600);

// Connect
//esp8266.begin(9600);
//sendCommand("AT",5,"OK");
//sendCommand("AT+CWMODE=1",5,"OK");
//sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");


//Soil Moisture Stuff
 pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
 digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
 
//ADC Stuff
ads1115.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
ads1115.begin();

//Driver Stuff
//Serial.println("TLC59711 test");
pinMode(10, OUTPUT);
tlc.begin();
tlc.write();
colorWipe(0, 65535, 0, 100); // "Green" (depending on your LED wiring)
//sd stuff
SD.begin(10);
//Serial.println("SD creating test");
file = SD.open("WR_TEST1.TXT", O_CREAT | O_WRITE);
SD.remove("WR_TEST1.TXT");
file = SD.open("WR_TEST1.TXT", O_CREAT | O_WRITE);


//LED stuff
pinMode(ledPin, OUTPUT);
}

void loop() {

//Saltwater switch check
//unsigned long strt = micros();
//int soilVal=readSoil();

 int soilVal=readSoil();
 //Serial.println(soilVal);
//If in water, do all the driver/reading
//if(soilVal>300){
//For testing just run this branch regardless of the soil reading
if(soilVal>750&&count<520&&donevar==false){
//  if count==0{
//    file.println("NEW TEST");
//  }
 unsigned long strt = micros();
 //int soilVal=readSoil();
//Driver Stuff
//colorWipe(0, 65535, 0, 100); // "Green" (depending on your LED wiring)

//ADC Stuff
int16_t results3=ads1115.readADC_Differential_0_1();
float conversion3=results3*(.0078125E-3);
file.println(conversion3,6);
//unsigned long nd = micros() -strt;
if (conversion3>=.25&&digitalRead(ledPin == LOW)){
    digitalWrite(ledPin, HIGH);
}
else if (conversion3<.25&&digitalRead(ledPin == HIGH)){
  digitalWrite(ledPin, LOW);
}

//Serial.println(results3,6); //Serial.print("("); 
//Serial.println(conversion3,6); //Serial.print("("); 
//Serial.print("Time: ");Serial.println(nd);
count++;

}
else if(soilVal<750&&count==0){
  //Serial.println("Waiting for water");
}
else{
  donevar=true;
  
 file.flush();  
 file.close();
 //colorWipe(0, 0, 0, 100); // "Green" (depending on your LED wiring)
 digitalWrite(ledPin, LOW);
 //Serial.println("DONE");
 
 // Connect
esp8266.begin(9600);
sendCommand("AT",5,"OK");
sendCommand("AT+CWMODE=1",5,"OK");
sendCommand("AT+CWJAP=\""+ (String) "DukeOpen" +"\",\""+ (String) "" +"\"",20,"OK");


  File myfile = SD.open("WR_TEST1.TXT");   
  
  //Serial.println("open");
  
  // if the file is available, read the file
  if (myfile) 
  {
    myfile.seek(0);
      size_t n;      // Length of returned field with delimiter.
  char str[10];  // Must hold longest field with delimiter and zero byte.
  int count=0;
    while (myfile.available())
    {

  // Read the file and print fields.
  while (true) {
    n = readField(&myfile, str, sizeof(str), ",\n");

    // done if Error or at EOF.
    if (n == 0) break;

    // Print the type of delimiter.
    if (str[n-1] == ',' || str[n-1] == '\n') {
     // Serial.print(str[n-1] == ',' ? F("comma: ") : F("endl:  "));
      // Remove the delimiter.
      str[n-1] = 0;
      count++;
    } else {
      // At eof, too long, or read error.  Too long is error.
    //  Serial.print(myfile.available() ? F("error: ") : F("eof:   "));
        }

   // Print the field
//   Serial.println(str);

   String getData = "GET /update?api_key="+ (String) "Z29L3NWYLD7JILHY" +"&"+ (String) "field1" +"="+String(str);
   sendCommand("AT+CIPMUX=1",5,"OK");
   sendCommand("AT+CIPSTART=0,\"TCP\",\""+ (String) "api.thingspeak.com" +"\","+ (String) "80",15,"OK");
   sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
   esp8266.println(getData);delay(15000);countTrueCommand++;
   sendCommand("AT+CIPCLOSE=0",5,"OK");
              }
          }
    myfile.close();
    //Serial.write("Closed");
      }
}
//else{
//  Serial.println("Out of water");

//}
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
 //   delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}

void sendCommand(String command, int maxTime, char readReplay[]) {
 // Serial.print(countTrueCommand);
 // Serial.print(". at command => ");
 // Serial.print(command);
//  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

 
size_t readField(File* file, char* str, size_t size, char* delim) {
  char ch;
  size_t n = 0;
  while ((n + 1) < size && file->read(&ch, 1) == 1) {
    // Delete CR.
    if (ch == '\r') {
      continue;
    }
    str[n++] = ch;
    if (strchr(delim, ch)) {
        break;
    }
  }
  str[n] = '\0';
  return n;
}
