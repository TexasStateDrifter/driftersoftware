#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"

File dataFileTemp; // Text file for Temperature
File dataFilePH; // Text file for PH
File dataFileDO; // Text file for DO
File dataFileCond; // Text file for Conductivity
RTC_PCF8523 rtc; // Using the RTC as an object for the 1st time in the software



double temp_F;


/******************************************************
* RTC setup
*
* Description - This function setups the RTC as well 
* as an initialization of the SD card to the system
*
******************************************************/
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

  if (!rtc.initialized()) { // Remove the ! in order to adjust the DateTime via code
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 9, 2018 at 2pm you would call:
    rtc.adjust(DateTime(2018, 4, 9, 14, 51, 0));
  }
}

/******************************************************
* Datalog functions
*
* Description - These functions are in charge of opening 
* a repective text file and calling the measurment functions 
* in order to be stored on to the SD card
*
******************************************************/


/*
 * Datalog Temperature
 * 
 * This function is in charge of storing both a fahrenheit and
 * celcius temperature on to the SD card
 *
 */
 
void DatalogTemp() 
{
    dataFileTemp = SD.open("temp.txt", FILE_WRITE); // Opens the temperture text file

    if(dataFileTemp)
    {
      //Display data and time on both Serial and SD for testing
      
      Serial.println(F("   "));
      dataFileTemp.println(F("   "));
      
      float temperature = getTemp(); // function to measure temperature
      DateTime now = rtc.now();

      
      
      Serial.print(temperature);
      dataFileTemp.print(temperature);
      Serial.print(",");
      dataFileTemp.print(",");
      Serial.print(F("°C"));
      dataFileTemp.print(F("°C"));
      Serial.print(F(","));
      dataFileTemp.print(F(","));
      temp_F = ((temperature * 1.8) + 32); // function to change celcius to fahrenheit
      Serial.print(temp_F);
      dataFileTemp.print(temp_F);
      Serial.print(",");
      dataFileTemp.print(",");
      Serial.print(F("°F"));
      dataFileTemp.print(F("°F"));

      Serial.print(F(","));
      dataFileTemp.print(F(","));

      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      
      Serial.print(F(","));
      
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

      dataFileTemp.print(F(","));

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
      Serial.println(F("error opening temp.txt")); // Alert the user if file could not be open
    }
}

/*
 * Datalog PH
 * 
 * This function is in charge of storing PH
 * on to the SD card
 *
 */
 
void DatalogPH() 
{
    dataFilePH = SD.open("ph.txt", FILE_WRITE); // Opens the PH text file

    if(dataFilePH)
    {
      //Display data and time on both Serial and SD for testing
      
      Serial.println(F("   "));
      dataFilePH.println(F("   "));
      
      float ph = getPH();
      DateTime now = rtc.now();
      
      Serial.print(ph);
      dataFilePH.print(ph);
      Serial.print(F(","));
      dataFilePH.print(F(","));
      Serial.print("PH");
      dataFilePH.print("PH");
      Serial.print(F(","));
      dataFilePH.print(F(","));

      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);

      Serial.print(F(","));

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

      dataFilePH.print(F(","));

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
      Serial.println(F("error opening ph.txt")); // Alert the user if file could not be open
    }
}

/*
 * Datalog DO
 * 
 * This function is in charge of storing Dissolved
 * Oxygen on to the SD card
 *
 */

void DatalogDO() 
{
  
    dataFileDO = SD.open("do.txt", FILE_WRITE); // Opens the DO text file

    if(dataFileDO)
    {
      //Display data and time on both Serial and SD for testing
      
      Serial.println(F("   "));
      dataFileDO.println(F("   "));
      
      float DO = getDO();
      DateTime now = rtc.now();
      
      Serial.print(DO);
      dataFileDO.print(DO);
      Serial.println(F(","));
      dataFileDO.println(F(","));
      Serial.print(F("mg/L"));
      dataFileDO.print(F("mg/L"));
      Serial.println(F(","));
      dataFileDO.println(F(","));

      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);

      Serial.println(F(","));

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
            
      dataFileDO.println(F(","));

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
      Serial.println(F("error opening do.txt")); // Alert the user if file could not be open
    }
}

/*
 * Datalog Conductivity
 * 
 * This function is in charge of storing Conductivity
 * on to the SD card
 *
 */
 
void DatalogCond() 
{
    dataFileCond = SD.open("cond.txt", FILE_WRITE); // Opens the Conductivity text file
    
    if(dataFileCond)
    {
      //Display data and time on both Serial and SD for testing
      
      Serial.println(F("   "));
      dataFileCond.println(F("   "));

      DateTime now = rtc.now();
      runEC(); // Fucntion called to run a conductivity measuring loop
      
      float ec1 = getEC(); // Function called to grab Conductivity
      float tds1 = getTDS(); // Function called to grab Total Dissolved Solids
      float sal1 = getSAL(); // Function called to grab Salinity
      float grav1 = getGRAV(); // Function called to grab Gravity
      
      
      Serial.print("EC");       //we now print each value we parsed separately
      Serial.print(","); 
      Serial.print(ec1);        //this is the EC value
      Serial.print(","); 
      dataFileCond.print("EC");
      dataFileCond.print(","); 
      dataFileCond.print(ec1);
      dataFileCond.print(","); 
    
      Serial.print("TDS");       //we now print each value we parsed separately
      Serial.print(","); 
      Serial.print(tds1);        //this is the TDS value
      Serial.print(","); 
      dataFileCond.print("TDS");
      dataFileCond.print(",");
      dataFileCond.print(tds1);
      dataFileCond.print(",");
    
      Serial.print("SAL");        //we now print each value we parsed separately
      Serial.print(",");
      Serial.print(sal1);         //this is the salinity value
      Serial.print(",");
      dataFileCond.print("SAL");
      dataFileCond.print(",");
      dataFileCond.print(sal1);
      dataFileCond.print(",");
    
      Serial.print("GRAV");       //we now print each value we parsed separately
      Serial.print(",");
      Serial.print(grav1);        //this is the specific gravity
      Serial.print(",");
      dataFileCond.print("GRAV");
      dataFileCond.print(",");
      dataFileCond.print(grav1);
      dataFileCond.print(",");
      
      
      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);

      Serial.print(",");

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

      dataFileCond.print(",");

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
      Serial.println(F("error opening cond.txt")); // Alert the user if file could not be open
    }
}



