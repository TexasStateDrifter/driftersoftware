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
#include <Wire.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <string.h>


#include <SD.h>
#include "RTClib.h"
RTC_PCF8523 rtc1; // Using the RTC as an object for the 2nd time in the software
 
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define DoSensorPin  A8 // Clarify DO connection to A8

// These #define make it easy to set the backlight color.
// Note: Can delete this whole section if you know you want
//       to set it to particular color. Backlight is color
//       is initialized in "void setup()".

#define NONE    0x0
#define GREEN   0x2
#define WHITE   0x7

// Number of total MENU options -- Modify the number of rows in the array
#define MENU_COUNT 6

// Sub menu Options and Sizes
#define SUBSENSOR_COUNT 4
#define SUBFREQ_COUNT 2
#define SUBDATE_COUNT 2
#define SUBSLEEP_COUNT 2
#define SUBLIGHT_COUNT 2


int pos = 0; //position of the array
int menuVar = 0; //list of menus
int arrSize; //size of each menu/submenu
long int freq;

//bool variables determine probe toggle, 1 = ON, 0 = OFF
bool temp = 1;
bool pH = 1;
bool cond = 1;
bool dO = 1;

int yearSet; // setting the date based off user function 
int monthSet;
int daySet;
int hourSet;
int minuteSet;
int secondSet;

int counterLoop;

// all menu Arrays                                                                      
String const menu[MENU_COUNT+1] PROGMEM = {"Run        ","Sample Now ","Sensors    ","Sample Freq","Time/Date  ","Backlight  ","           "};
String const subSensor[5] PROGMEM = {"Temp        ","pH          ","Cond.       ","D.O.       "};
String const subSampleFreq[3] PROGMEM = {"    Mod Rate    ","HR:MIN     00:0"};
String const subDate[3] PROGMEM = {"Date","Time"};
String const subLight[3] PROGMEM = {"ON","OFF"};


// setup code sets up the 1st page of the main menu as well as minor sensor adjustments
void setup() 
{
  Serial.begin(9600);
  RTCsetup();
  ECsetup();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("*");
  lcd.print(menu[0]);
  lcd.setCursor(1,1);
  lcd.print(menu[1]);
  lcd.setBacklight(0x2);
  pinMode(DoSensorPin,INPUT);
  readDoCharacteristicValues();
  pinMode(A13, OUTPUT);
  digitalWrite(A13, LOW);
  pinMode(A14, OUTPUT);
  digitalWrite(A14, LOW);
  pinMode(A15, OUTPUT);
  digitalWrite(A15, LOW);
}


// Main Program 
void loop() 
{
  
  lcd.setCursor(0,0);             // move to the begining of the second line

  uint8_t lcd_key = lcd.readButtons();  // Read the buttons
  
  switch (lcd_key){               // depending on which button was pushed, we perform an action
       case BUTTON_RIGHT:{
            
            delay(100);
            break;
       }
       case BUTTON_LEFT:{
          
            menuLeft(); // back button to revert to main menu    
            delay(100);
            break;
       }    
       case BUTTON_UP:{
            
            String* arr = menuNumb(getmenuVar()); // function to set array/menu selected
            arrSize = menuSize(getmenuVar()); // function to grab menu/sub menu size
            menuUp(getPos(), arrSize, arr); // function to place cursor on screen
            
            delay(100);
            break; 
       }
       case BUTTON_DOWN:{
            
            String* arr = menuNumb(getmenuVar()); // function to set array/menu selected
            arrSize = menuSize(getmenuVar()); // function to grab menu/sub menu size
            menuDown(getPos(), arrSize, arr); // function to place cursor on screen
            
            delay(100);
            break;       
       }
       case BUTTON_SELECT:{
            
            setmenuVar(menuSel(getmenuVar(), getPos())); // function to set menu selected
            delay(100);
            break;
       }
       default :{

           break;
       }
  }
}

/******************************************************
* Size function
*
* Description - This function grabs which menu array the
* LCD is currently on and returns the size
*
*
******************************************************/

int menuSize(int menuVar)
{
  switch(menuVar)
  {
    case 0:
    {
      return MENU_COUNT;   //menu (count 7)
      break;
    }
    
    case 1:
    {
      return SUBSENSOR_COUNT;   //submenu 1 (count 4)
      break;
    }
    
    case 2:
    {
      return SUBFREQ_COUNT;   //submenu 2 (count 2)
      break;
    }

    case 3:
    {
      return SUBDATE_COUNT;   //submenu 2 (count 2)
      break;
    }

    case 4:
    {
      return SUBLIGHT_COUNT;   //submenu 2 (count 2)
      break;
    }

  }
}



/******************************************************
* Set Menu
*
* Description - This function grabs the postion of the
* current menu array and returns the sub menu array
*
******************************************************/

String* menuNumb (int menuVar)
{
  switch(menuVar)
  {
    case 0:
    {
      String* point = menu;
      return point;
      delete[] point;   
      break;
    }

    case 1:
    {
      String* point = subSensor;
      return point;
      delete[] point;    
      break;
    }

    case 2:
    {
      String* point = subSampleFreq;
      return point;
      delete[] point;    
      break;
    }

    case 3:
    {
      String* point = subDate;
      return point;
      delete[] point;    
      break;
    }

    case 4:
    {
      String* point = subLight;
      return point;
      delete[] point;    
      break;
    }
  }

  
}

/******************************************************
* Button UP/Down
*
* Description - These buttons when selected will move
* the cursor up or down on each menu/sub menu and will
* display a proper position based off the current menu
*
******************************************************/


void menuUp(int pos, int arrSize, String *arr) // Cursor will move upward until top of menu array
{
  lcd.setCursor(0,0);
  lcd.print((" "));
  lcd.setCursor(0,1);
  lcd.print((" "));
  lcd.setCursor(1,0);

  if(pos == 0)
  {
    setPos(pos);
    
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(1,0);
    lcd.print(arr[pos]);
    lcd.setCursor(1,1);
    lcd.print(arr[pos+1]);

    displayToggle();
  }
  else
  {
    pos = pos-1;
    if(pos % 2 == 0)
    {
      setPos(pos);

      lcd.setCursor(0,0);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos+1]); 

      displayToggle();
    }
    else if(pos % 2 == 1)
    {
      setPos(pos);
      
      lcd.setCursor(0,1);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos-1]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos]);

      displayToggle();
    }
  }
}

///////////////////////////////////////////////////////////////////

void menuDown(int pos, int arrSize, String *arr) // Cursor will move downard until end of menu array
{
  lcd.setCursor(0,0);
  lcd.print((" "));
  lcd.setCursor(0,1);
  lcd.print((" "));
  lcd.setCursor(1,0);
  
  if(pos == arrSize-1)
  {
    if(pos % 2 == 0)
    {
      setPos(pos);
      
      lcd.setCursor(0,0);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos+1]);

      displayToggle();
    }

    else if(pos % 2 == 1)
    {
      setPos(pos);
      
      lcd.setCursor(0,1);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos-1]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos]);

      displayToggle();
    }
  }
  else
  {
    pos = pos+1;
    if(pos % 2 == 0)
    {
      setPos(pos);
      
      lcd.setCursor(0,0);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos+1]);

      displayToggle();
    }
    else if(pos % 2 == 1)
    {
      setPos(pos);
      
      lcd.setCursor(0,1);
      lcd.print("*");
      lcd.setCursor(1,0);
      lcd.print(arr[pos-1]);
      lcd.setCursor(1,1);
      lcd.print(arr[pos]);

      displayToggle();
    }
  }
}


/******************************************************
* Button Select
*
* Description - This button when selected will assure
* a confirmation by the user and will either move the 
* LCD to print a particular sub menu and/or allow the user
* to store user adjusted variables. The system will then
* return the postion of the array
*
******************************************************/

int menuSel (int menuVar, int pos)
{
  if (menuVar == 0 && pos == 0) // Run function will run the system to continuously measure and record
                                // data based off user sample rate
  {
    int sleepLoop;
    lcd.clear();
    lcd.setCursor(0,0);
    if(getFreq() == 0)
    {
      sleepLoop = 5; // if user does not set sample rate, 5 minutes is standard
    }
    else
    {
      sleepLoop = getFreq(); // sample frequency is grabbed by the user input
    }

    lcd.clear();
    lcd.setBacklight(NONE); // Turn backlight OFF   
    lcd.noDisplay();
    
    while(true) // continuous loop for system to measure and store data
    {
      DateTime now = rtc1.now();
      /*
      Serial.print("Enter Sleep Mode   ");

      Serial.print(now.year(), DEC);
      Serial.print(F("/"));
      Serial.print(now.month(), DEC);
      Serial.print(F("/"));
      Serial.print(now.day(), DEC);
      Serial.print(F(" "));
      Serial.print(now.hour(), DEC);
      Serial.print(F(":"));
      Serial.print(now.minute(), DEC);
      Serial.print(F(":"));
      Serial.print(now.second(), DEC);
      Serial.println();
      */
      
      delay(30);
      counterLoop = 0; // counter set at zero
      
      while(counterLoop != sleepLoop) // loop will run and wake the system up every minute until the counterLoop
                                      // matches the set sample rate
      {
        sleepSetup();
        delay(30);
        sleepNow(); // system will sleep
        delay(100);
        counterLoop++;
      }

      

    //Each if statement checks to see if the user has toggled each probe ON or OFF. If off, that probe will not be measure and recorded

    if(getToggleTemp() == 1)
    {
      DatalogTemp(); // Temperature will record and store value on SD
      delay(10);
    }

    if(getToggleDO() == 1)
    {
      pinMode(A13, OUTPUT);
      digitalWrite(A13, HIGH);
      DatalogDO(); // DO will record and store value on SD
      delay(10);
      pinMode(A13, OUTPUT);
      digitalWrite(A13, LOW);
      delay(10);
    }

    if(getTogglePH() == 1)
    {
      pinMode(A14, OUTPUT);
      digitalWrite(A14, HIGH);
      DatalogPH(); // PH will record and store value on SD
      delay(10);
      pinMode(A14, OUTPUT);
      digitalWrite(A14, LOW);
      delay(10);
    }

    if(getToggleCond() == 1)
    {
      pinMode(A15, OUTPUT);
      digitalWrite(A15, HIGH);
      for(int i = 0; i<4; i++)
      {
        runEC();
      }
      DatalogCond(); //  Conductivity will record and store value on SD
      delay(10);
      pinMode(A15, OUTPUT);
      digitalWrite(A15, LOW);
    }

      delay(100);
    }
    
  }
  
  if (menuVar == 0 && pos == 1)  // Sample now will measure and display every measurement on the LCD
  {
    lcd.clear();
    
    //Each if statement checks to see if the user has toggled each probe ON or OFF. If off, that probe will not be measure and recorded
    
    if(getToggleTemp() == 1)
    {
      lcd.print("Temp:");
      lcd.print(getTemp());
      delay(3000);
    }
    lcd.clear();
    if(getToggleDO() == 1)
    {
      pinMode(A13, OUTPUT);
      digitalWrite(A13, HIGH);
      lcd.print("DO:");
      lcd.print(getDO());
      delay(3000);
      pinMode(A13, OUTPUT);
      digitalWrite(A13, LOW);
      delay(1000);
    }
    lcd.clear();
    if(getTogglePH() == 1)
    {
      pinMode(A14, OUTPUT);
      digitalWrite(A14, HIGH);
      lcd.print("pH:");
      lcd.print(getPH());
      delay(3000);
      pinMode(A14, OUTPUT);
      digitalWrite(A14, LOW);
      delay(1000);
    }
    lcd.clear();
    if(getToggleCond() == 1)
    {
      pinMode(A15, OUTPUT);
      digitalWrite(A15, HIGH);
      for(int i = 0; i<5; i++)
      {
        runEC();
      }
      lcd.setCursor(0,0);
      lcd.print("EC:");
      lcd.print(getEC());

      lcd.setCursor(0,1);
      lcd.print("TDS:");
      lcd.print(getTDS());
      delay(3000);
      
      lcd.clear();
      
      lcd.setCursor(0,0);
      lcd.print("SAL:");
      lcd.print(getSAL());

      lcd.setCursor(0,1);
      lcd.print("GRAV:");
      lcd.print(getGRAV());
      delay(3000);

      pinMode(A15, OUTPUT);
      digitalWrite(A15, LOW);
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.print(menu[0]);
    lcd.setCursor(1,1);
    lcd.print(menu[1]);
    setPos(0);
    return 0;
  }
  
  if (menuVar == 0 && pos == 2) // toggle submenu, displays each probe with an ON/OFF text as an indicator on the current position
  {
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(1,0);
    lcd.print(subSensor[0]);
    
    if (getToggleTemp() == 1)
    {
      lcd.setCursor(13,0);
      lcd.print("  ");
      lcd.setCursor(13,0);
      lcd.print("ON");
    }
    else if (getToggleTemp() == 0)
    {
      lcd.setCursor(13,0);
      lcd.print("  ");
      lcd.setCursor(13,0);
      lcd.print("OFF");
    }
    
    lcd.setCursor(1,1);
    lcd.print(subSensor[1]);
    
    if (getTogglePH() == 1)
    {
      lcd.setCursor(13,1);
      lcd.print("  ");
      lcd.setCursor(13,1);
      lcd.print("ON");
    }
    else if (getTogglePH() == 0)
    {
      lcd.setCursor(13,1);
      lcd.print("  ");
      lcd.setCursor(13,1);
      lcd.print("OFF");
    }
    setPos(0);
    return 1;
  }

  if (menuVar == 0 && pos == 3)   // sample frequency, stand alone function on its own
  {
    return lcdSampleRate();
  }

  if (menuVar == 0 && pos == 4)   // displays both date and time when called
  {
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(1,0);
    lcd.print(subDate[0]);
    lcd.setCursor(1,1);
    lcd.print(subDate[1]);
    displayDate();
    setPos(0);
    return 3;
  }

  if (menuVar == 0 && pos == 5)   // Allows the user to toggle the light on and off
  {
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(1,0);
    lcd.print(subLight[0]);
    lcd.setCursor(1,1);
    lcd.print(subLight[1]);
    setPos(0);
    return 4;
  }

  if (menuVar == 0 && pos == 6) // Allows the user to toggle the light on and off
  {
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(1,0);
    lcd.print(subLight[0]);
    lcd.setCursor(1,1);
    lcd.print(subLight[1]);
    setPos(0);
    return 5;
  }

  if (menuVar == 1 && pos == 0)  // submenu 1 temperature toggle
  {
    
    if (getToggleTemp() == 1)
    {
      setToggleTemp(0);
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print("OFF");
    }
    else if (getToggleTemp() == 0)
    {
      setToggleTemp(1);
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print("ON");
    }
    setPos(0);
    return 1;
  }

  if (menuVar == 1 && pos == 1)  // submenu 1 pH toggle
  {
    if (getTogglePH() == 1)
    {
      setTogglePH(0);
      lcd.setCursor(13,1);
      lcd.print("   ");
      lcd.setCursor(13,1);
      lcd.print("OFF");
    }
    else if (getTogglePH() == 0)
    {
      setTogglePH(1);
      lcd.setCursor(13,1);
      lcd.print("   ");
      lcd.setCursor(13,1);
      lcd.print("ON");
    }
    setPos(1);
    return 1;
  }

  if (menuVar == 1 && pos == 2)  // submenu 1 conductivity toggle
  {
    if (getToggleCond() == 1)
    {
      setToggleCond(0);
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print("OFF");
    }
    else if (getToggleCond() == 0)
    {
      setToggleCond(1);
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print("ON");
    }
    setPos(2);
    return 1;
  }

  if (menuVar == 1 && pos == 3)  // submenu 1 DO toggle
  {
    if (getToggleDO() == 1)
    {
      setToggleDO(0);
      lcd.setCursor(13,1);
      lcd.print("   ");
      lcd.setCursor(13,1);
      lcd.print("OFF");
    }
    else if (getToggleDO() == 0)
    {
      setToggleDO(1);
      lcd.setCursor(13,1);
      lcd.print("   ");
      lcd.setCursor(13,1);
      lcd.print("ON");
    }
    setPos(3);
    return 1;
  }

  if (menuVar == 3 && pos == 0)  //Date change function
  {
    dateMod();
    menuLeft();
    return 0;
  }

  if (menuVar == 3 && pos == 1)  //Time Change function
  {
    timeMod();
    menuLeft();
    return 0;
  }

  
  if (menuVar == 4 && pos == 0)  //Backlight ON
  {
    lcd.setBacklight(GREEN);
    setPos(0);
    return 4;
  }

  if (menuVar == 4 && pos == 1)  //Backlight OFF
  {
    lcd.setBacklight(NONE);
    setPos(1);
    return 4;
  }

   
}

/******************************************************
*Left Button
*
*Description - This button will essentially return the
*GUI back to the original startup display without any
*changes (more simply) a back button
*
******************************************************/

void menuLeft()
{
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("*");
  lcd.setCursor(1,0);
  lcd.print(menu[0]);
  lcd.setCursor(1,1);
  lcd.print(menu[1]);
  
  setmenuVar(0);
  setPos(0);
}


/******************************************************
*Setters and Getters
*
*Description - This potion of the code is in charge of
*returning and storing particular variables based of the
*UI
*
******************************************************/

int getmenuVar() //  particular menu or submenu selected by user  
{
     return menuVar;
}

void setmenuVar(int Menu)
{
     menuVar = Menu;
}


////////////////////////////////////////////////////////////

int getPos() //  position of the lcd based off user interface at the moment
{
     return pos;
}

void setPos(int Position)
{
     pos = Position;
}


////////////////////////////////////////////////////////////

int getFreq() //  the sample frequency set by the user
{
     return freq;
}

void setFreq(int tot)
{
     freq = tot;
}


////////////////////////////////////////////////////////////

bool getToggleTemp() //  temperature toggle
{
     return temp;
}

void setToggleTemp(bool tog1)
{
     temp = tog1;
}


////////////////////////////////////////////////////////////

bool getTogglePH() //  pH  toggle
{
     return pH;
}

void setTogglePH(bool tog2)
{
     pH = tog2;
}


////////////////////////////////////////////////////////////

bool getToggleCond() //  conductivity  toggle
{
     return cond;
}

void setToggleCond(bool tog3)
{
     cond = tog3;
}


////////////////////////////////////////////////////////////

bool getToggleDO() //  DO  toggle
{
     return dO;
}

void setToggleDO(bool tog4)
{
     dO = tog4;
}


////////////////////////////////////////////////////////////

int getYear() // new year is set based off user
{
    return yearSet;
}

void setYear(int yearNew)
{
    yearSet = yearNew;
}


////////////////////////////////////////////////////////////

int getMonth() // new month is set based off user
{
    return monthSet;
}

void setMonth(int monthNew)
{
    monthSet = monthNew;
}


////////////////////////////////////////////////////////////

int getDay() // new day is set based off user
{
    return daySet;
}

void setDay(int dayNew)
{
    daySet = dayNew;
}

////////////////////////////////////////////////////////////

int getHour() // new hour is set based off user
{
    return hourSet;
}

void setHour(int hourNew)
{
    hourSet = hourNew;
}


////////////////////////////////////////////////////////////

int getMinute() // new minute is set based off user
{
    return minuteSet;
}

void setMinute(int minuteNew)
{
    minuteSet = minuteNew;
}


////////////////////////////////////////////////////////////

int getSecond() // new second is set based off user
{
    return secondSet;
}

void setSecond(int secondNew)
{
    secondSet = secondNew;
}


/******************************************************
*Miscellaneous Functions
*
*Description - Most of these fucntions are called upon
*once a section is button selected for user change.
*These fucntions can range from user set sample rate to 
*modding time and date
*
******************************************************/


/*
 * Display Toggle 
 * 
 * This function allows the user to set each probe on
 * or off before measuring and collecting data
 */
 
void displayToggle()
{
  delay(10);
  if (getmenuVar() == 1)
  {
    if(getPos() == 0 || getPos() == 1)
    {
      if (getToggleTemp() == 0 && getTogglePH() == 0)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("OFF");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("OFF");
      }
      
      else if (getToggleTemp() == 1 && getTogglePH() == 1)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("ON");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("ON");
      }
      
      else if (getToggleTemp() == 0 && getTogglePH() == 1)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("OFF");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("ON");
      }

      else if (getToggleTemp() == 1 && getTogglePH() == 0)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("ON");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("OFF");
      }
    }
    else if(getPos() == 2 || getPos() == 3)
    {
      if (getToggleCond() == 0 && getToggleDO() == 0)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("OFF");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("OFF");
      }
      
      else if (getToggleCond() == 1 && getToggleDO() == 1)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("ON");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("ON");
      }
      
      else if (getToggleCond() == 0 && getToggleDO() == 1)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("OFF");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("ON");
      }

      else if (getToggleCond() == 1 && getToggleDO() == 0)
      {
        lcd.setCursor(13,0);
        lcd.print("   ");
        lcd.setCursor(13,0);
        lcd.print("ON");

        lcd.setCursor(13,1);
        lcd.print("   ");
        lcd.setCursor(13,1);
        lcd.print("OFF");
      }
    }
  }
  else
  {
    return;
  }
}


/*
 * Display Date
 * 
 * This function grabs the set data on the RTC 
 * at the momment and displays it on the screen
 * 
 */
 
void displayDate()
{
  DateTime now = rtc1.now();
  
  Serial.print(now.year(), DEC);
  lcd.setCursor(6,0);
  lcd.print(now.year());
  Serial.print(F("/"));
  lcd.setCursor(10,0);
  lcd.print("/");
  Serial.print(now.month(), DEC);
  if (now.month() < 10)
  {
    lcd.setCursor(11,0);
    lcd.print("0");
    lcd.setCursor(12,0);
    lcd.print(now.month());
  }
  else
  {
    lcd.setCursor(11,0);
    lcd.print(now.month());
  } 
  Serial.print(F("/"));
  lcd.setCursor(13,0);
  lcd.print("/");
  Serial.print(now.day(), DEC);
  if (now.day() < 10)
  {
    lcd.setCursor(14,0);
    lcd.print("0");
    lcd.setCursor(15,0);
    lcd.print(now.day());
  }
  else
  {
    lcd.setCursor(14,0);
    lcd.print(now.day());
  }

  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  if (now.hour() < 10)
  {
    lcd.setCursor(8,1);
    lcd.print("0");
    lcd.setCursor(9,1);
    lcd.print(now.hour());
  }
  else
  {
    lcd.setCursor(8,1);
    lcd.print(now.hour());
  }
  Serial.print(F(":"));
  lcd.setCursor(10,1);
  lcd.print(":");
  Serial.print(now.minute(), DEC);
  if (now.minute() < 10)
  {
    lcd.setCursor(11,1);
    lcd.print("0");
    lcd.setCursor(12,1);
    lcd.print(now.minute());
  }
  else
  {
    lcd.setCursor(11,1);
    lcd.print(now.minute());
  }
  Serial.print(F(":"));
  lcd.setCursor(13,1);
  lcd.print(":");
  Serial.print(now.second(), DEC);
  if (now.second() < 10)
  {
    lcd.setCursor(14,1);
    lcd.print("0");
    lcd.setCursor(15,1);
    lcd.print(now.second());
  }
  else
  {
    lcd.setCursor(14,1);
    lcd.print(now.second());
  }
  Serial.println();
}


/*
 * LCD Sample Rate
 * 
 * This function allows the user to set the sample
 * rate at which the device will record data. 
 * Using the up and down buttons, the LCD will
 * display and increase/decrease on a rate of 5.
 * The select button will store the user sample
 * rate
 * 
 */
 
int lcdSampleRate()
{
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(subSampleFreq[0]);
  lcd.setCursor(0,1);
  lcd.print(subSampleFreq[1]);
  
  int freqMin = 5;
  int freqHr = 0;
  lcd.setCursor(14,1);
  lcd.print(0);
  lcd.setCursor(15,1);
  lcd.print(freqMin);
  bool b = 1;
  while(b) // runs loop until b = 0
  {
    
    uint8_t lcd_key = lcd.readButtons();
    if (lcd_key == BUTTON_UP) // The up button will display a 5 minute increase up until 24 hours
    {
      if (freqHr == 24)
      {
        lcd.setCursor(11,1);
        lcd.print(freqHr);
        lcd.setCursor(15,1);
        lcd.print(0);
        lcd.setCursor(14,1);
        lcd.print(0);
      }
      else
      {
        freqMin = freqMin+5;
        if (freqMin == 60)
        {
          freqHr = freqHr+1;
          freqMin = 0;
          if (freqHr >= 10)
          {
            lcd.setCursor(11,1);
            lcd.print(freqHr);
          }
          else
          {
            lcd.setCursor(12,1);
            lcd.print(freqHr);
          }
        }
        
        if (freqMin < 10)
        {
          lcd.setCursor(15,1);
          lcd.print(0);
          lcd.setCursor(14,1);
          lcd.print(0);
          lcd.setCursor(15,1);  
          lcd.print(freqMin);
        }
        else
        {
          lcd.setCursor(14,1);  
          lcd.print(freqMin);
        }
       }
    }

    
    else if (lcd_key == BUTTON_DOWN) // The down button will display a 5 minute decrease to 5 minutes minimum
    {
      if (freqMin == 5 && freqHr == 0)
      {
      }
      
      else
      {
         freqMin = freqMin-5;
         
         if (freqMin == -5)
          {
            freqHr = freqHr-1;
            freqMin = 55;
            if (freqHr < 10)
            {
              lcd.setCursor(11,1);
              lcd.print(0);
              lcd.setCursor(12,1);
              lcd.print(freqHr);
              lcd.setCursor(14,1);
              lcd.print(freqMin);
            }
            else
            {
              lcd.setCursor(11,1);
              lcd.print(freqHr);
              lcd.setCursor(14,1);
              lcd.print(freqMin);
            }
          }
          else
          {
            if (freqMin < 10)
            {
              lcd.setCursor(14,1);
              lcd.print(0);
              lcd.setCursor(15,1);  
              lcd.print(freqMin);

              if (freqHr < 10)
              {
                lcd.setCursor(11,1);
                lcd.print(0);
                lcd.setCursor(12,1);
                lcd.print(freqHr);
              }
              else
              {
                lcd.setCursor(11,1);
                lcd.print(freqHr);
              }
            }
            else
            {
              lcd.setCursor(14,1); 
              lcd.print(freqMin);

              if (freqHr < 10)
              {
                lcd.setCursor(11,1);
                lcd.print(0);
                lcd.setCursor(12,1);
                lcd.print(freqHr);
              }
              else
              {
                lcd.setCursor(11,1);
                lcd.print(freqHr);
              }
            }
          }
      }
    }

    else if (lcd_key == BUTTON_LEFT) // Left button returns to main menu without any changes
    {
      lcd.clear();
      menuLeft();

      
      b = 0; // end loop
    }

    else if (lcd_key == BUTTON_SELECT) // select button stores the user sample rate and returns home
    {
      Serial.println(freqMin);
      Serial.println(freqHr);
      int Hr123 = (freqHr * (60));
      Serial.println(Hr123);
      freq = (freqMin + Hr123);
      Serial.println(freq);
      setFreq(freq);
      lcd.clear();
      menuLeft();

      
      b = 0; // end loop
    }
    delay(50);
  }
  return 0;
}

/*
 * Mod Date
 * 
 * This function will be called when the Date is being changed 
 * by the user. This fucntion will call the Mod year, month, 
 * and day functions in order for the user to set accordingly.
 * The function will then call the RTC and store the new
 * set values
 * 
 */

void dateMod()
{
  DateTime now = rtc1.now();

  setHour(now.hour());
  setMinute(now.minute());
  setSecond(now.second());
  
  delay(200);
  modYear();
  delay(200);
  modMonth();
  delay(200);
  modDay();
  delay(200);
  
  yearSet = getYear();
  monthSet = getMonth();
  daySet = getDay();
  hourSet = getHour();
  minuteSet = getMinute();
  secondSet = getSecond();
  
  rtc1.adjust(DateTime(yearSet, monthSet, daySet, hourSet , minuteSet, secondSet));
  return; 
}

/*
 * Mod Time
 * 
 * This function will be called when the Time is being changed 
 * by the user. This fucntion will call the Mod Hour, Minute, 
 * and Second functions in order for the user to set accordingly.
 * The function will then call the RTC and store the new
 * set values
 * 
 */
 
void timeMod()
{
  DateTime now = rtc1.now();

  setYear(now.year());
  setMonth(now.month());
  setDay(now.day());
  
  delay(200);
  modHour();
  delay(200);
  modMinute();
  delay(200);
  modSecond();
  delay(200);
  
  yearSet = getYear();
  monthSet = getMonth();
  daySet = getDay();
  hourSet = getHour();
  minuteSet = getMinute();
  secondSet = getSecond();
  
  rtc1.adjust(DateTime(yearSet, monthSet, daySet, hourSet , minuteSet, secondSet));
  return;
}

/*
 * Mod Year/Month/Day/Hour/Minute/Second
 * 
 * These function are called during either the mod date
 * or mod time. Each function will display a text to "please
 * set year, month, day, ect." and the user will be able to 
 * change the variable using the Up and Down buttons.
 * Select will store the variable.
 * 
 */

void modYear() // set year by user 2018 - 2050
{
  lcd.clear();
  int yearNew = 18;
  lcd.setCursor(0,0);
  lcd.print("Please set Year");
  lcd.setCursor(0,1);
  lcd.print("20");
  lcd.setCursor(2,1);
  lcd.print(yearNew);
  int b = 1;
  while(b)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(yearNew == 50)
      {
        lcd.setCursor(2,1);
        lcd.print(yearNew);
        delay(250);
      }
      else
      {
        yearNew++;
        lcd.setCursor(2,1);
        lcd.print(yearNew);
        delay(250);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(yearNew == 18)
      {
        lcd.setCursor(2,1);
        lcd.print(yearNew);
        delay(250);
      }
      else
      {
        yearNew--;
        lcd.setCursor(2,1);
        lcd.print(yearNew);
        delay(250);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      b = 0;
      setYear(yearNew);
    }
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////

void modMonth() // set month by user 1 - 12
{
  lcd.clear();
  int monthNew = 1;
  lcd.setCursor(0,0);
  lcd.print("Please set Month");
  lcd.setCursor(0,1);
  lcd.print(monthNew);
  int c = 1;
  while(c)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(monthNew == 12)
      {
        lcd.setCursor(0,1);
        lcd.print(monthNew);
        delay(250);
      }
      else
      {
        monthNew++;
        lcd.setCursor(0,1);
        lcd.print(monthNew);
        delay(250);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(monthNew == 10)
      {
        monthNew--;
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,1);
        lcd.print(monthNew);
        delay(250);
      }
      else if(monthNew == 1)
      {
        lcd.setCursor(0,1);
        lcd.print(monthNew);
        delay(250);
      }
      else
      {
        monthNew--;
        lcd.setCursor(0,1);
        lcd.print(monthNew);
        delay(250);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      c = 0;
      setMonth(monthNew);
    }
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////

void modDay() // set day by user 1 - 31
{
  lcd.clear();
  int dayNew = 1;
  lcd.setCursor(0,0);
  lcd.print("Please set Day");
  lcd.setCursor(0,1);
  lcd.print(dayNew);
  int d = 1;
  while(d)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(dayNew == 31)
      {
        lcd.setCursor(0,1);
        lcd.print(dayNew);
        delay(250);
      }
      else
      {
        dayNew++;
        lcd.setCursor(0,1);
        lcd.print(dayNew);
        delay(250);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(dayNew == 10)
      {
        dayNew--;
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,1);
        lcd.print(dayNew);
        delay(250);
      }
      else if(dayNew == 1)
      {
        lcd.setCursor(0,1);
        lcd.print(dayNew);
        delay(250);
      }
      else
      {
        dayNew--;
        lcd.setCursor(0,1);
        lcd.print(dayNew);
        delay(250);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      d = 0;
      setDay(dayNew);
    }
    delay(50);
  }
}


////////////////////////////////////////////////////////////////////////////////////

void modHour() // set hour by user 1 - 24
{
  lcd.clear();
  int hourNew = 1;
  lcd.setCursor(0,0);
  lcd.print("Please set Hour");
  lcd.setCursor(0,1);
  lcd.print(hourNew);
  lcd.setCursor(11,1);
  lcd.print("24-Hr");
  int b = 1;
  while(b)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(hourNew == 60)
      {
        lcd.setCursor(0,1);
        lcd.print(hourNew);
        delay(250);
      }
      else
      {
        hourNew++;
        lcd.setCursor(0,1);
        lcd.print(hourNew);
        delay(250);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(hourNew == 10)
      {
        hourNew--;
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,1);
        lcd.print(hourNew);
        delay(250);
      }
      else if(hourNew == 1)
      {
        lcd.setCursor(0,1);
        lcd.print(hourNew);
        delay(250);
      }
      else
      {
        hourNew--;
        lcd.setCursor(0,1);
        lcd.print(hourNew);
        delay(250);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      b = 0;
      setHour(hourNew);
    }
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////

void modMinute() // set minute by user 0 - 59
{
  lcd.clear();
  int minuteNew = 0;
  lcd.setCursor(0,0);
  lcd.print("Please set Min");
  lcd.setCursor(0,1);
  lcd.print(minuteNew);
  int c = 1;
  while(c)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(minuteNew == 59)
      {
        lcd.setCursor(0,1);
        lcd.print(minuteNew);
        delay(150);
      }
      else
      {
        minuteNew++;
        lcd.setCursor(0,1);
        lcd.print(minuteNew);
        delay(150);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(minuteNew == 10)
      {
        minuteNew--;
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,1);
        lcd.print(minuteNew);
        delay(150);
      }
      else if(minuteNew == 0)
      {
        lcd.setCursor(0,1);
        lcd.print(minuteNew);
        delay(150);
      }
      else
      {
        minuteNew--;
        lcd.setCursor(0,1);
        lcd.print(minuteNew);
        delay(150);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      c = 0;
      setMinute(minuteNew);
    }
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////

void modSecond() // set second by user 0 - 59
{
  lcd.clear();
  int secondNew = 0;
  lcd.setCursor(0,0);
  lcd.print("Please set Sec");
  lcd.setCursor(0,1);
  lcd.print(secondNew);
  int d = 1;
  while(d)
  {
    uint8_t lcd_key = lcd.readButtons();
    if(lcd_key == BUTTON_UP)
    {
      if(secondNew == 59)
      {
        lcd.setCursor(0,1);
        lcd.print(secondNew);
        delay(150);
      }
      else
      {
        secondNew++;
        lcd.setCursor(0,1);
        lcd.print(secondNew);
        delay(150);
      }
    }
    
    else if(lcd_key == BUTTON_DOWN)
    {
      if(secondNew == 10)
      {
        secondNew--;
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,1);
        lcd.print(secondNew);
        delay(150);
      }
      else if(secondNew == 0)
      {
        lcd.setCursor(0,1);
        lcd.print(secondNew);
        delay(150);
      }
      else
      {
        secondNew--;
        lcd.setCursor(0,1);
        lcd.print(secondNew);
        delay(150);
      }
    }

    else if(lcd_key == BUTTON_SELECT)
    {
      d = 0;
      setSecond(secondNew);
    }
    delay(50);
  }
}


