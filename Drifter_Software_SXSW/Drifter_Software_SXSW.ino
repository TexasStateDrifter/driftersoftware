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
//#include <LowPower.h>
#include <avr/pgmspace.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

#define DoSensorPin  A1

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #define make it easy to set the backlight color.
// Note: Can delete this whole section if you know you want
//       to set it to particular color. Backlight is color
//       is initialized in "void setup()".
#define NONE    0x0
#define GREEN   0x2

#define MENU_COUNT 7 // Number of total MENU options -- Modify the number of rows in the array
#define SUBMENU_COUNT 4 // Max # of SUBMENU options for any menu option
bool menuMode = 0; // bool var to determine if it's in main menu (0) or sub menu(1). 

// 2-dim Array of type String. If submenu option has less then max, fill in with (" ").
String const menuOptions[MENU_COUNT+1][SUBMENU_COUNT+1] PROGMEM = {{"Run       ","          ","          ","          ","          "},
                                                                   {"Sample Now","          ","          ","          ","          "},
                                                                   {"Sensors   ","Temp      ","pH        ","Cond.     ","D.O       "},
                                                                   {"SampleFreq","Mod. Rate ","          ","          ","          "},
                                                                   {"Time/Date ","Mod. Time ","Mod. Date ","          ","          "},
                                                                   {"SleepMode ","ON        ","OFF       ","          ","          "},
                                                                   {"Backlight ","ON        ","OFF       ","          ","          "},
                                                                   {"          ","          ","          ","          ","          "}};

// String const subOptions[][] PROGMEM = {{"",""},
//                                       {"",""},
//                                       {"",""}};

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
  lcd.setBacklight(0x7);
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
   // The code below will put arduino into sleep mode
   // in which it will be waken up by an external inturrupt
   
  }
  else{
    if(Z == 1 and K == 1){ // If selected 1st suboption of the 1st main menu option run this.
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

