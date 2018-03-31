#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"


File dataFileTemp;
File dataFilePH;
File dataFileDO;
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

void DatalogTemp() 
{
    dataFileTemp = SD.open("temp.txt", FILE_WRITE);

    if(dataFileTemp)
    {
      float temperature = getTemp();
      DateTime now = rtc.now();
      
      Serial.print(temperature);
      dataFileTemp.print(temperature);
      Serial.print(F("°C"));
      dataFileTemp.print(F("°C"));
      Serial.print(F(" "));
      dataFileTemp.print(F(" "));
      temp_F = ((temperature * 1.8) + 32);
      Serial.print(temp_F);
      dataFileTemp.print(temp_F);
      Serial.print(F("°F"));
      dataFileTemp.print(F("°F"));

      Serial.print(F("   "));
      dataFileTemp.print(F("   "));

      
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
      
      dataFileTemp.print(now.year(), DEC);
      dataFileTemp.print('/');
      dataFileTemp.print(now.month(), DEC);
      dataFileTemp.print('/');
      dataFileTemp.print(now.day(), DEC);
      dataFileTemp.print(" (");
      dataFileTemp.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFileTemp.print(") ");
      dataFileTemp.print(now.hour(), DEC);
      dataFileTemp.print(':');
      dataFileTemp.print(now.minute(), DEC);
      dataFileTemp.print(':');
      dataFileTemp.print(now.second(), DEC);
      dataFileTemp.println(); 

      dataFileTemp.close();
    }
    else
    {
      Serial.println(F("error opening temp.txt"));
    }
    // delay(5000); //slow sample speed 15 seconds
}


void DatalogPH() 
{
    dataFilePH = SD.open("ph.txt", FILE_WRITE);

    if(dataFilePH)
    {
      float ph = getPH();
      DateTime now = rtc.now();
      
      Serial.print(ph);
      dataFilePH.print(ph);
      Serial.print("PH");
      dataFilePH.print("PH");
      Serial.print(F("   "));
      dataFilePH.print(F("   "));

      
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
      
      dataFilePH.print(now.year(), DEC);
      dataFilePH.print('/');
      dataFilePH.print(now.month(), DEC);
      dataFilePH.print('/');
      dataFilePH.print(now.day(), DEC);
      dataFilePH.print(" (");
      dataFilePH.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFilePH.print(") ");
      dataFilePH.print(now.hour(), DEC);
      dataFilePH.print(':');
      dataFilePH.print(now.minute(), DEC);
      dataFilePH.print(':');
      dataFilePH.print(now.second(), DEC);
      dataFilePH.println(); 

      dataFilePH.close();
    }
    else
    {
      Serial.println(F("error opening ph.txt"));
    }
}

void DatalogDO() 
{
    dataFileDO = SD.open("do.txt", FILE_WRITE);

    if(dataFileDO)
    {
      float DO = getDO();
      DateTime now = rtc.now();
      
      Serial.print(DO);
      dataFileDO.print(DO);
      Serial.print(F("mg/L"));
      dataFileDO.print(F("mg/L"));
      Serial.print(F("   "));
      dataFileDO.print(F("   "));

      
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
      
      dataFileDO.print(now.year(), DEC);
      dataFileDO.print('/');
      dataFileDO.print(now.month(), DEC);
      dataFileDO.print('/');
      dataFileDO.print(now.day(), DEC);
      dataFileDO.print(" (");
      dataFileDO.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFileDO.print(") ");
      dataFileDO.print(now.hour(), DEC);
      dataFileDO.print(':');
      dataFileDO.print(now.minute(), DEC);
      dataFileDO.print(':');
      dataFileDO.print(now.second(), DEC);
      dataFileDO.println(); 

      dataFileDO.close();
    }
    else
    {
      Serial.println(F("error opening do.txt"));
    }
}

/*
void DatalogCond() 
{
    dataFileCond = SD.open("cond.txt", FILE_WRITE);

    if(dataFileCond)
    {
      float Cond = getCond();
      DateTime now = rtc.now();
      
      Serial.print(temperature);
      dataFileCond.print(temperature);
      Serial.print(F("°C"));
      dataFileCond.print(F("°C"));
      Serial.print(F(" "));
      dataFileCond.print(F(" "));
      temp_F = ((temperature * 1.8) + 32);
      Serial.print(temp_F);
      dataFileCond.print(temp_F);
      Serial.print(F("°F"));
      dataFileCond.print(F("°F"));

      Serial.print(F("   "));
      dataFileCond.print(F("   "));

      
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
      
      dataFileCond.print(now.year(), DEC);
      dataFileCond.print('/');
      dataFileCond.print(now.month(), DEC);
      dataFileCond.print('/');
      dataFileCond.print(now.day(), DEC);
      dataFileCond.print(" (");
      dataFileCond.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFileCond.print(") ");
      dataFileCond.print(now.hour(), DEC);
      dataFileCond.print(':');
      dataFileCond.print(now.minute(), DEC);
      dataFileCond.print(':');
      dataFileCond.print(now.second(), DEC);
      dataFileCond.println(); 

      dataFileCond.close();
    }
    else
    {
      Serial.println(F("error opening cond.txt"));
    }
}
*/
