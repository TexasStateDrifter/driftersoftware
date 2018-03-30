#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"


File dataFile;
RTC_DS1307 rtc;

char const daysOfTheWeek[7][12] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
double temp_F;

void RTCsetup() 
{
  //The serial monitor output will only be read in 9600
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
      while (1);
  }
  
  while (!Serial) {
    ;
  }

  Serial.print(F("Initializing SD card...."));
  if (!SD.begin(10)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done."));

  if (!rtc.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 9, 2018 at 2pm you would call:
    rtc.adjust(DateTime(2018, 2, 12, 14, 41, 0));
  }
}

void RTCdatalog() 
{
    dataFile = SD.open("temp.txt", FILE_WRITE);

    if(dataFile)
    {
      float temperature = getTemp();
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

      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      Serial.print(F(" ("));
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
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
      Serial.println(F("error opening temp.txt"));
    }
    // delay(5000); //slow sample speed 15 seconds
}
