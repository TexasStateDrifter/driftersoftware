#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

//datalogger A4 A5
//DO sensor A1
//EC sensor rx 2 tx 3
//pH sensor A0

#define DoSensorPin  A1
File dataFile;
File dataFile2;
//RTC_DS1307 rtc;
RTC_PCF8523 rtc;

double temp_F;
//int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
//OneWire ds(DS18S20_Pin);  // on digital pin 2

//void RTCsetup()
void RTCsetup() 
{
  //The serial monitor output will only be read in 9600
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
      while (1);
  }
  
  // DO_Read
  pinMode(DoSensorPin,INPUT);
  readDoCharacteristicValues();
  //
  
  while (!Serial) {
    ;
  }

  Serial.print(F("Initializing SD card...."));
  if (!SD.begin(10)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done."));

  if (!rtc.initialized()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 9, 2018 at 2pm you would call:
    rtc.adjust(DateTime(2018, 3, 9, 17, 59, 0));
  }
}

//void RTCdatalog() 
void sampleNow()
{
    dataFile = SD.open("temp.txt", FILE_WRITE);
    dataFile2 = SD.open("do.txt", FILE_WRITE);
    if(dataFile && dataFile2)
    {
      float temperature = getTemp();
      float doRead = getDO();
      DateTime now = rtc.now();
      
      Serial.print(temperature);
      dataFile.print(temperature);
      Serial.print(F("°C"));
      dataFile.print(F("°C"));
      Serial.print(F(" "));
      dataFile.print(F(" "));
      temp_F = ((temperature * 1.8) + 32);
      Serial.print(temp_F);
      dataFile.print(temp_F);
      Serial.print(F("°F"));
      dataFile.print(F("°F"));

      Serial.print(F("   "));
      dataFile.print(F("   "));

      Serial.print(doRead);
      dataFile2.print(doRead);
      Serial.print(F(" "));
      dataFile2.print(F(" "));
      Serial.print(F("mg/L"));
      dataFile2.print(F("mg/L"));
      Serial.print(F("   "));
      dataFile2.print(F("   "));
      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      Serial.print(F(" ("));
      Serial.print(F(") "));
      Serial.print(now.hour(), DEC);
      Serial.print(F(":"));
      Serial.print(now.minute(), DEC);
      Serial.print(F(":"));
      Serial.print(now.second(), DEC);
      Serial.println();
      
      dataFile.print(now.year(), DEC);
      dataFile.print('/');
      dataFile.print(now.month(), DEC);
      dataFile.print('/');
      dataFile.print(now.day(), DEC);
      dataFile.print(" (");
      dataFile.print(") ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);
      dataFile.println();

      dataFile2.print(now.year(), DEC);
      dataFile2.print('/');
      dataFile2.print(now.month(), DEC);
      dataFile2.print('/');
      dataFile2.print(now.day(), DEC);
      dataFile2.print(" (");
      dataFile2.print(") ");
      dataFile2.print(now.hour(), DEC);
      dataFile2.print(':');
      dataFile2.print(now.minute(), DEC);
      dataFile2.print(':');
      dataFile2.print(now.second(), DEC);
      dataFile2.println(); 

      dataFile.close();
      dataFile2.close();
    }
    else
    {
      Serial.println(F("error opening temp.txt"));
    }
    delay(5000); //slow sample speed 15 seconds
}


