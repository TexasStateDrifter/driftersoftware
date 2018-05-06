#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"

File dataFileTemp;
File dataFilePH;
File dataFileDO;
File dataFileCond;
File dataFileVolt;
RTC_PCF8523 rtc;


const float voltageDivider = 4.9;

char const daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
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
  Serial.println("initialization done.");

  if (!rtc.initialized()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 9, 2018 at 2pm you would call:
    rtc.adjust(DateTime(2018, 4, 9, 14, 51, 0));
  }
}


void DatalogTemp() 
{
    dataFileTemp = SD.open("temp.txt", FILE_WRITE);

    if(dataFileTemp)
    {
      Serial.println(F("   "));
      dataFileTemp.println(F("   "));
      
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
}


void DatalogPH() 
{
    dataFilePH = SD.open("ph.txt", FILE_WRITE);

    if(dataFilePH)
    {
      Serial.println(F("   "));
      dataFilePH.println(F("   "));
      
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
      Serial.println(F("   "));
      dataFileDO.println(F("   "));
      
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


void DatalogCond() 
{
    dataFileCond = SD.open("cond.txt", FILE_WRITE);
    
    if(dataFileCond)
    {

      Serial.println(F("   "));
      dataFileCond.println(F("   "));

      DateTime now = rtc.now();
      runEC();
      
      float ec1 = getEC();
      float tds1 = getTDS();
      float sal1 = getSAL();
      float grav1 = getGRAV();
      
      Serial.print("EC:");                                //we now print each value we parsed separately
      Serial.println(ec1);                                 //this is the EC value
      dataFileCond.print("EC:");
      dataFileCond.println(ec1);
    
      Serial.print("TDS:");                               //we now print each value we parsed separately
      Serial.println(tds1);                                //this is the TDS value
      dataFileCond.print("TDS:");
      dataFileCond.println(tds1);
    
      Serial.print("SAL:");                               //we now print each value we parsed separately
      Serial.println(sal1);                                //this is the salinity value
      dataFileCond.print("SAL:");
      dataFileCond.println(sal1);
    
      Serial.print("GRAV:");                              //we now print each value we parsed separately
      Serial.print(grav1);                               //this is the specific gravity
      dataFileCond.print("GRAV:");
      dataFileCond.print(grav1);
      Serial.print("   ");                                   //this just makes the output easer to read
      dataFileCond.print("   ");
      
      
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


/*
void DatalogVolt()
{
    dataFileVolt = SD.open("volt.txt", FILE_WRITE);

    if(dataFileVolt)
    {
      Serial.println(F("   "));
      dataFileVolt.println(F("   "));

      DateTime now = rtc.now();
      
      // read the input on analog pin 0:
      int sensorValue = analogRead(A2); // Battery output
      //int sensorValue2 = analogRead(A3); // Solar output
      
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltageRead = sensorValue * (5.0 / 1023.0);
      //float voltageRead2 = sensorValue2 * (5.0 / 1023.0);

      float voltage = (voltageRead * voltageDivider);
      //float voltage2 = (voltageRead2 * voltageDivider);

      Serial.print("Battery: ");
      Serial.print(voltage);
      dataFileVolt.print("Battery: ");
      dataFileVolt.print(voltage);
      
      Serial.print("   ");
      dataFileVolt.print("   ");
      

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
  
      dataFileVolt.print(now.year(), DEC);
      dataFileVolt.print('/');
      dataFileVolt.print(now.month(), DEC);
      dataFileVolt.print('/');
      dataFileVolt.print(now.day(), DEC);
      dataFileVolt.print(" (");
      dataFileVolt.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFileVolt.print(") ");
      dataFileVolt.print(now.hour(), DEC);
      dataFileVolt.print(':');
      dataFileVolt.print(now.minute(), DEC);
      dataFileVolt.print(':');
      dataFileVolt.print(now.second(), DEC);
      dataFileVolt.println(); 

      dataFileVolt.close();
    }
    else
    {
      Serial.println(F("error opening volt.txt"));
    }
}
*/

