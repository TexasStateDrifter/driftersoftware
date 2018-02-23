#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

File dataFile;
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  while (!Serial) {
    ;
  }

  Serial.print("Initializing SD card....");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // Feburary 21, 2018 at 12pm you would call:
    rtc.adjust(DateTime(2018, 2, 21, 12, 0, 0));
  }
}

// the loop routine runs over and over again forever:
void loop() 
{
    dataFile = SD.open("volt.txt", FILE_WRITE);

    if(dataFile)
    {

      // read the input on analog pin 0:
      int sensorValue = analogRead(A0);
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = sensorValue * (5.0 / 1023.0);
      // print out the value you read:
      
      DateTime now = rtc.now();

      Serial.print(voltage);
      dataFile.print(voltage);
      
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
    delay(15000); //slow sample speed 15 seconds
    // if 5 minutes 300000
}
