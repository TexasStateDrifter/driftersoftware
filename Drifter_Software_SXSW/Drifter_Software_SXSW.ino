/***********************************************************
*            Drifter Software v1.0
*
* Programmers: Isaac Bondoc & John Marquez 
* 
* 
* Description: Drifter software is developed to 
* allow one to connect a Dissolved Oxygen, pH, 
* temperature and conductivity sensor to sample 
* water quality. A User interface was also developed
* for easy interaction with the drifter and its settings.
* 
* 
************************************************************/

/*
#include <avr/pgmspace.h>
#include <EEPROM.h>

#define DoSensorPin  A1
//dissolved oxygen sensor analog output pin to arduino mainboard
#define VREF 5000    //for arduino uno, the ADC reference is the AVCC, that is 5000mV(TYP)
float doValue;      //current dissolved oxygen value, unit; mg/L
float temperature = 25;    //default temperature is 25^C, you can use a temperature sensor to read it

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define ReceivedBufferLength 20
char receivedBuffer[ReceivedBufferLength+1];    // store the serial command
byte receivedBufferIndex = 0;

#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    //store the analog value in the array, readed from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;

#define SaturationDoVoltageAddress 12          //the address of the Saturation Oxygen voltage stored in the EEPROM
#define SaturationDoTemperatureAddress 16      //the address of the Saturation Oxygen temperature stored in the EEPROM
float SaturationDoVoltage,SaturationDoTemperature;
float averageVoltage;

const float SaturationValueTab[41] PROGMEM = {      //saturation dissolved oxygen concentrations at various temperatures
14.46, 14.22, 13.82, 13.44, 13.09,
12.74, 12.42, 12.11, 11.81, 11.53,
11.26, 11.01, 10.77, 10.53, 10.30,
10.08, 9.86,  9.66,  9.46,  9.27,
9.08,  8.90,  8.73,  8.57,  8.41,
8.25,  8.11,  7.96,  7.82,  7.69,
7.56,  7.43,  7.30,  7.18,  7.07,
6.95,  6.84,  6.73,  6.63,  6.53,
6.41,
};
*/







#include <Wire.h>
//#include <LowPower.h>
#include <avr/pgmspace.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

//#define DoSensorPin  A1

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #define make it easy to set the backlight color.
// Note: Can delete this whole section if you know you want
//       to set it to particular color. Backlight is color
//       is initialized in "void setup()".
#define NONE    0x0
#define GREEN   0x2

#define MENU_COUNT 3 // Number of total MENU options -- Modify the number of rows in the array
#define SUBMENU_COUNT 3 // Max # of SUBMENU options for any menu option
bool menuMode = 0; // bool var to determine if it's in main menu (0) or sub menu(1). 

// 2-dim Array of type String. If submenu option has less then max, fill in with (" ").
String const menuOptions[MENU_COUNT+1][SUBMENU_COUNT+1] PROGMEM = {{"Sample Now","subOpt00  ","subOpt01  ","          "},
                                                                   {"Sensors   ","Temp      ","pH        ","D.O.      "},
                                                                   {"Backlight ","ON        ","OFF       ","          "},
                                                                   {"          ","          ","          ","          "}};
                                                                // {"Option 4  ","subOpt40  ","subOpt41  "}
                                                                // {"Backlight ","ON        ","OFF      "}};

void setup() {
  // setup code sets up the 1st page of the main menu
//  RTCsetup();
//  funcSetup();
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("*");
  lcd.print(menuOptions[0][0]);
  lcd.setCursor(6,1);
  lcd.print(menuOptions[1][0]);
  Serial.begin(9600);
  menuMode = 0;
  lcd.setBacklight(GREEN);
  
  pinMode(DoSensorPin,INPUT);
  readDoCharacteristicValues();
}

// Menu Counter, Z, Gives you the position in the main menu
// you are on. subMenu Counter, K, gives you the position in
// the subMenu you are on.
int Z = 0, K = 0; 

// Main Program 
void loop() {
 
  lcd.setCursor(0,0);             // move to the begining of the second line

  uint8_t lcd_key = lcd.readButtons();  // Read the buttons 
  
  switch (lcd_key){               // depending on which button was pushed, we perform an action
    
       case BUTTON_RIGHT:{             //  push button "RIGHT" and show the word on the screen
          //  lcd.print(F("RIGHT"));
            delay(300);
            break;
       }
       case BUTTON_LEFT:{
           //  lcd.print(F("LEFT ")); //  push button "LEFT" and show the word on the screen
             Z = menuLeft(Z);
//             lcd.setCursor(0,0);
//             lcd.print(Z);
//             lcd.setCursor(0,1);
//             lcd.print(K);
             delay(300);
             break;
       }    
       case BUTTON_UP:{
          //   lcd.print(F("UP   "));  //  push button "UP" and show the word on the screen
             Z = menuUp(Z);
//             lcd.setCursor(0,0);
//             lcd.print(Z);
//             lcd.setCursor(0,1);
//             lcd.print(K);
             delay(300);
             break;
       }
       case BUTTON_DOWN:{
           //  lcd.print(F("DOWN "));  //  push button "DOWN" and show the word on the screen
             Z = menuDown(Z);
//             lcd.setCursor(0,0);
//             lcd.print(Z);
//             lcd.setCursor(0,1);
//             lcd.print(K);
             delay(300);
             break;      
       }
       case BUTTON_SELECT:{
           //  lcd.print(F("SEL  "));  //  push button "SELECT" and show the word on the screen
             Z = menuSel(Z);
//             lcd.setCursor(0,0);
//             lcd.print(Z);
//             lcd.setCursor(0,1);
//             lcd.print(K);
             delay(300);
             break;
       }
       default :{
          // lcd.print(F("NONE "));  //  No action  will show "None" on the screen
//          lcd.setCursor(0,0);
//          lcd.print(Z);
//          lcd.setCursor(0,1);
//          lcd.print(K);
           break;
       }
  }
}

//////////////////////////////////////////////////////////////////
// Function: menuCursor takes in a parameter "d" from one of the 
//           up, down, left, or select functions. "d" indicates 
//           the main menu position so that the UI can be on 
//           the right menu page. 
//////////////////////////////////////////////////////////////////
void menuCursor(int d){

  // Clear the astick area. Note: I could have just used lcd.clear()
  // but using it makes the screen perform a noticable flicker.
  lcd.setCursor(5,0);
  lcd.print(F(" "));
  lcd.setCursor(5,1);
  lcd.print(F(" "));
  lcd.setCursor(6,0);
  
  if(menuMode == 0){  // If in main menu mode
    lcd.setCursor(5, d % 2);     // Set cursor to place astrick
    lcd.print(F("*"));
    lcd.setCursor(6,0);

    if(d <= MENU_COUNT and d % 2 == 1){  // If menu option and odd position number
     lcd.print(menuOptions[d-1][0]);
     lcd.setCursor(6,1);
     lcd.print(menuOptions[d][0]);
    }
    else if(d <=  MENU_COUNT and d % 2 == 0){  // If menu option and even position number
     lcd.print(menuOptions[d][0]);
     lcd.setCursor(6,1);
     lcd.print(menuOptions[d+1][0]);
    } 
  }
  
  else if(menuMode == 1) { // If in sub menu mode
     lcd.setCursor(5, (K-1) % 2);
     lcd.print(F("*"));
     
    if(K % 2 == 1){  // If sub option and even position number
      lcd.setCursor(6,0);
      lcd.print(menuOptions[d][K]);
      if(menuOptions[d][K] == "D.O.      "){     ////     started fucking with code here!
        lcd.setCursor(6,1);
        lcd.print(F("          "));
      }
      else{
        lcd.setCursor(6,1);
        lcd.print(menuOptions[d][K+1]);
      }
    }
    else if(K % 2 == 0){  // If sub option and odd position number
      lcd.setCursor(6,0);
      lcd.print(menuOptions[d][K-1]);
      if(menuOptions[d][K] == "D.O.      "){    // also fucked with this code here!
        lcd.setCursor(6,1);
        lcd.print(F("          "));
      }
      else{
        lcd.setCursor(6,1);
        lcd.print(menuOptions[d][K]);
      }
    }
  }
 
}

// Pressing up button will decrement menu counter. 
int menuUp(int i) {
    
    if(menuMode == 0){  // If in main menu 
        i -= 1;         
      if(i + 1 <= 1){
        i = 0 ;
      }
    }
    else if(menuMode == 1){ // Else if in sub menu
      K -= 1 ;
      if(K == 0){
        K = 1;    
      }
    }
    
//    lcd.setCursor(0,0);
//    lcd.print(i);
//    lcd.setCursor(0,1);
//    lcd.print(K);
    menuCursor(i);
    return i;
}

// Pressing down button increase counter by 1. 
int menuDown(int i) {
    
    if(menuMode == 0){  // If in Main menu 
      i += 1;
      if (i == MENU_COUNT) {
        i = MENU_COUNT - 1; 
      }
    }
    else if(menuMode == 1){ // If in Sub menu and K is equal to last Submenu option
      if(K == SUBMENU_COUNT){
        K = SUBMENU_COUNT ;  // Set equal to Submenu count (Doesn't allow K to be higher than SubMenu_Count
      }
    else if(menuOptions[Z][K+1] == "          "){
        K = K; 
    }
    else{  
        K += 1;
      }
    }
//    lcd.setCursor(0,0);
//    lcd.print(i);
//    lcd.setCursor(0,1);
//    lcd.print(K);
    menuCursor(i);
    return i;
}

int menuSel(int i){
  Serial.print("Z = ");
  Serial.print(i);
  Serial.print(" K = ");
  Serial.print(K);
  
  // If in mainMenu, put in SubMenu mode (mainMenu == 1) 
  // Increment K (position in sub menu)
  if (menuMode == 0 and Z >= 1) {
    Serial.print(F("Entering submenu mode"));
    menuMode = 1;
    K += 1 ;
//    lcd.setCursor(0,0);
//    lcd.print(i);
//    lcd.setCursor(0,1);
//    lcd.print(K);
    menuCursor(i);  
    return i;
  }
  // Else you are already in subMenu and want to trigger 
  // an action
  else if(menuMode == 0 and Z == 0){      
      lcd.clear();
      lcd.print("Temp:");
      lcd.print(getTemp());
      delay(3000);
      lcd.clear();
      lcd.print("D.O.:");
      lcd.print(getDO());
      delay(3000);
      lcd.clear();
      lcd.print("pH:");
      lcd.print(getPH());
      delay(3000);
      lcd.setCursor(0,0);
      lcd.print(F("        "));
      lcd.setCursor(5, 0);
      lcd.print("*");
      lcd.print(menuOptions[0][0]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[1][0]);
  }
  else{
    if(Z == 1 and K == 1){ // If selected 1st suboption of the 1st main menu option run this.
      Serial.print(F("This is right before the dataLog code for the temp sensor"));  // Debug code
      lcd.clear();
      lcd.print("Temp:");
      lcd.print(getTemp());
      delay(3000);
      lcd.setCursor(0,0);
      lcd.print(F("        "));
      lcd.setCursor(5, 0);
      lcd.print("*");
      lcd.print(menuOptions[0][0]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[1][0]);
    }
    else if(Z == 1 && K == 2){
      // Do something
      lcd.clear();
      lcd.print("pH:");
      lcd.print(getPH());
      delay(3000);
      lcd.setCursor(0,0);
      lcd.print(F("        "));
      lcd.setCursor(5, 0);
      lcd.print("*");
      lcd.print(menuOptions[0][0]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[1][0]);
    }
    else if(Z == 1 and K == 3){
      lcd.clear();
      lcd.print("D.O:");
      lcd.print(getDO());
      delay(3000);
      lcd.setCursor(0,0);
      lcd.print(F("        "));
      lcd.setCursor(5, 0);
      lcd.print("*");
      lcd.print(menuOptions[0][0]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[1][0]);
    }
    else if(Z == 2 && K == 1){
      // Do something
      lcd.setBacklight(GREEN); // Turn backlight ON
    }
    else if(Z == 2 && K == 2){
      // Do something
      lcd.setBacklight(NONE); // Turn backlight OFF      
    }
/*
    else if(Z == 3 && K == 1){
      // Do something
      set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
      sleep_enable();
      sleep_cpu ();  
    }
    else if(Z == 3 && K == 2){
      // Do something
    }    
 /*
    else if(Z == 4 && K == 1){
      lcd.display();
    }
    else if(Z == 4 && K ==2){
      lcd.noDisplay();
    }
*/
  }
  return i;
}

int menuLeft(int i){
  menuMode = 0;
  K = 0 ;
  i = 0 ;
//  lcd.setCursor(0,0);
//  lcd.print(i);
//  lcd.setCursor(0,1);
//  lcd.print(K);
  menuCursor(i);
  return(i);
}









/*
float getDO()
{
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 30U)     //every 30 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(DoSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }
   
   static unsigned long tempSampleTimepoint = millis();
   if(millis()-tempSampleTimepoint > 500U)  // every 500 milliseconds, read the temperature
   {
      tempSampleTimepoint = millis();
      //temperature = readTemperature();  // add your temperature codes here to read the temperature, unit:^C
   }
   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 1000U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
      {
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      }
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the value more stable by the median filtering algorithm
      //Serial.print(F("Temperature:"));
      //Serial.print(temperature,1);
      //Serial.print(F("^C"));
      doValue = pgm_read_float_near( &SaturationValueTab[0] + (int)(SaturationDoTemperature+0.5) ) * averageVoltage / SaturationDoVoltage;  //calculate the do value, doValue = Voltage / SaturationDoVoltage * SaturationDoValue(with temperature compensation)
      //Serial.print(F("DO Value:"));
      //Serial.print(doValue,2);
      //Serial.println(F("mg/L"));
      return doValue;
   }
 
}

boolean serialDataAvailable(void)
{
  char receivedChar;
  static unsigned long receivedTimeOut = millis();
  while ( Serial.available() > 0 ) 
  {   
    if (millis() - receivedTimeOut > 500U) 
    {
      receivedBufferIndex = 0;
      memset(receivedBuffer,0,(ReceivedBufferLength+1));
    }
    receivedTimeOut = millis();
    receivedChar = Serial.read();
    if (receivedChar == '\n' || receivedBufferIndex == ReceivedBufferLength)
    {
  receivedBufferIndex = 0;
  strupr(receivedBuffer);
  return true;
    }else{
        receivedBuffer[receivedBufferIndex] = receivedChar;
        receivedBufferIndex++;
    }
  }
  return false;
}

int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      {
    bTab[i] = bArray[i];
      }
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
    for (i = 0; i < iFilterLen - j - 1; i++) 
          {
      if (bTab[i] > bTab[i + 1]) 
            {
    bTemp = bTab[i];
          bTab[i] = bTab[i + 1];
    bTab[i + 1] = bTemp;
       }
    }
      }
      if ((iFilterLen & 1) > 0)
  bTemp = bTab[(iFilterLen - 1) / 2];
      else
  bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

void readDoCharacteristicValues(void)
{
    EEPROM_read(SaturationDoVoltageAddress, SaturationDoVoltage);  
    EEPROM_read(SaturationDoTemperatureAddress, SaturationDoTemperature);
    if(EEPROM.read(SaturationDoVoltageAddress)==0xFF && EEPROM.read(SaturationDoVoltageAddress+1)==0xFF && EEPROM.read(SaturationDoVoltageAddress+2)==0xFF && EEPROM.read(SaturationDoVoltageAddress+3)==0xFF)
    {
      SaturationDoVoltage = 1127.6;   //default voltage:1127.6mv
      EEPROM_write(SaturationDoVoltageAddress, SaturationDoVoltage);
    }
    if(EEPROM.read(SaturationDoTemperatureAddress)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+1)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+2)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+3)==0xFF)
    {
      SaturationDoTemperature = 25.0;   //default temperature is 25^C
      EEPROM_write(SaturationDoTemperatureAddress, SaturationDoTemperature);
    }    
}
*/

