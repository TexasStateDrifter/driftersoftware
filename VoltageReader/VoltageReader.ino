#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"

File dataFile;
RTC_PCF8523 rtc;
const float voltageDivider = 4.9;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
double temp_F;
int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
      while (1);
  }

  while (!Serial) {
    ;
  }

  Serial.print("Initializing SD card....");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (rtc.initialized()) {   //remove ! if need to readjust
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 21, 2018 at 12pm you would call:
    rtc.adjust(DateTime(2018, 3, 2, 13, 19, 0));
  }
}

// the loop routine runs over and over again forever:
void loop() 
{
    dataFile = SD.open("volt.txt", FILE_WRITE);

    if(dataFile)
    {

      // read the input on analog pin 0:
      int sensorValue = analogRead(A0); // Battery output
      int sensorValue2 = analogRead(A1); // Solar output
      
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltageRead = sensorValue * (5.0 / 1023.0);
      float voltageRead2 = sensorValue2 * (5.0 / 1023.0);

      float voltage = (voltageRead * voltageDivider);
      float voltage2 = (voltageRead2 * voltageDivider);

      float temperature = getTemp();
      // print out the value you read:
      
      DateTime now = rtc.now();

      Serial.print("Battery: ");
      Serial.print(voltage);
      dataFile.print("Battery: ");
      dataFile.print(voltage);
      
      Serial.print("   ");
      dataFile.print("   ");

      Serial.print("Solar: ");
      Serial.print(voltage2);
      dataFile.print("Solar: ");
      dataFile.print(voltage2);

      Serial.print("   ");
      dataFile.print("   ");

      Serial.print("Temp: ");
      dataFile.print("Temp: ");
      Serial.print(temperature);
      dataFile.print(temperature);
      Serial.print("°C");
      dataFile.print("°C");
      Serial.print(" ");
      dataFile.print(" ");
      temp_F = ((temperature * 1.8) + 32);
      Serial.print(temp_F);
      dataFile.print(temp_F);
      Serial.print("°F");
      dataFile.print("°F");

      Serial.print("   ");
      dataFile.print("   ");

      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
  
      dataFile.print(now.year(), DEC);
      dataFile.print('/');
      dataFile.print(now.month(), DEC);
      dataFile.print('/');
      dataFile.print(now.day(), DEC);
      dataFile.print(" (");
      dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFile.print(") ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.println(); 

      dataFile.close();
    }
    else
    {
      Serial.println("error opening volt.txt");
    }
    delay(2000); //Ran in milliseconds, 1 minute = 60000
    // 30 min = 180000
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
