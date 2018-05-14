
#include <Wire.h>
#include "RTClib.h"
#include <avr/sleep.h>
 

int wakeupPin = 2;
RTC_PCF8523 rtc2; // Using the RTC as an object for the 3rd time in the software

/*
 * Wake Up Now 
 * 
 * This function gets called for the sole reason to
 * wake up the arduino. This must be called in order for the
 * arduino to read "a" function
 * 
 */

void wakeUpNow()
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
  
}

/*
 * Sleep Now
 * 
 * This function gets called in order to shut down the arduino
 * expect for pin 2 used as an interrupt reader.
 * 
 */
 
void sleepNow()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // This is set at the most power saving mode on the avr/sleep library
  sleep_enable(); // sleep mode is enabled
  attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, LOW); // interrupt will wake up the system when SQ is LOW
  sleep_mode(); // sleep mode beings
  sleep_disable(); // sleep mode disabled after interrupt read
  detachInterrupt(digitalPinToInterrupt(2));
}

/*
 * Sleep Setup
 * 
 * This function is called everytime before the system
 * goes to sleep in order to set the RTC SQ wave pin High
 * and return Low after 1 minute
 * 
 */
 
void sleepSetup()
{
  Wire.begin();                  // join i2c bus (address optional for master)
  Wire.beginTransmission(0x68);  // address to rtc pcf8523, as per datasheet
  Wire.write(byte(0x10));        // Tmr_A_freq_ctrl
  Wire.write(B00000010);         // 00000 unused --- 010 --- set to 1 Hz
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(byte(0x11));        // Tmr_A_reg 00 to FF to set timer value
  Wire.write(byte(0x3C));        // 0x3C to set it to 60 Seconds = Tmr_A value%Tmr_A_Freq_ctrl value
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
  pinMode(wakeupPin,INPUT_PULLUP); // the input_pullup is used when working with open drain devices
  attachInterrupt(digitalPinToInterrupt(2), wakeUpNow, LOW); // interrupt will wake up the system when SQ is LOW
}

