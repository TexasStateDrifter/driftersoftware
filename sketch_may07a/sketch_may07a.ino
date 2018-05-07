#include <Wire.h>
#include "RTClib.h"
#include <avr/sleep.h>
// rtc inturrupt 
// attempting to set it to 1min. 
int wakeupPin = 2;
RTC_PCF8523 rtc;
  
void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
  
}
void setup() {
  Serial.begin(9600);
  Wire.begin();                  // join i2c bus (address optional for master)
  Wire.beginTransmission(0x68);  // address to rtc pcf8523, as per datasheet
  Wire.write(byte(0x10));        // Tmr_A_freq_ctrl
  Wire.write(B00000010);         // 00000 unused --- 010 --- set to 1 Hz
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x11));        // Tmr_A_reg 00 to FF to set timer value
  Wire.write(byte(0x0A));        // 0x0A to set it to 10 Seconds = Tmr_A value%Tmr_A_Freq_ctrl value
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x0F));        // Tmr_CLKOUT_ctrl reg
  Wire.write(B00111010);         // instruction 00111010, sets Timer_A inturrupt permanent active high, disables Timer_B, Activates Timer_A
  Wire.endTransmission();
  Wire.beginTransmission(0x68); 
  Wire.write(byte(0x01));        // address to Control_2 and status register 2
  Wire.write(B00000010);         // instruction 00000010 enables coundown timerA interrupt
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x0F));        // Tmr_CLKOUT_ctrl reg
  Wire.write(B00111010);         // instruction 00111010, sets Timer_A inturrupt permanent active high, disables Timer_B, Activates Timer_A
  Wire.endTransmission();
  pinMode(wakeupPin,INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, LOW);
}
int pinRead = 0; 
void loop() 
{
  //delay(1000);
  Serial.print("Entering Sleep Mode   ");
  
  DateTime now = rtc.now();
  
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print("  ");
  Serial.print("\n");
  delay(100);
  sleepNow();
  Serial.println("Out of Sleep Mode");
  Serial.print("\n");
  reSetup();
  
}
void sleepNow()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, LOW);
  sleep_mode();
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(2));
}
void reSetup()
{
  Wire.begin();                  // join i2c bus (address optional for master)
  Wire.beginTransmission(0x68);  // address to rtc pcf8523, as per datasheet
  Wire.write(byte(0x10));        // Tmr_A_freq_ctrl
  Wire.write(B00000010);         // 00000 unused --- 010 --- set to 1 Hz
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x11));        // Tmr_A_reg 00 to FF to set timer value
  Wire.write(byte(0x0A));        // 0x0A to set it to 10 Seconds = Tmr_A value%Tmr_A_Freq_ctrl value
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x0F));        // Tmr_CLKOUT_ctrl reg
  Wire.write(B00111010);         // instruction 00111010, sets Timer_A inturrupt permanent active high, disables Timer_B, Activates Timer_A
  Wire.endTransmission();
  Wire.beginTransmission(0x68); 
  Wire.write(byte(0x01));        // address to Control_2 and status register 2
  Wire.write(B00000010);         // instruction 00000010 enables coundown timerA interrupt
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x0F));        // Tmr_CLKOUT_ctrl reg
  Wire.write(B00111010);         // instruction 00111010, sets Timer_A inturrupt permanent active high, disables Timer_B, Activates Timer_A
  Wire.endTransmission();
  pinMode(wakeupPin,INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, LOW);
}
