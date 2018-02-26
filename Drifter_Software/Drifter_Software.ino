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
#include <avr/pgmspace.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #define make it easy to set the backlight color.
// Note: Can delete this whole section if you know you want
//       to set it to particular color. Backlight is color
//       is initialized in "void setup()".
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define MENU_COUNT 6 // Number of total MENU options -- Modify the number of rows in the array
#define SUBMENU_COUNT 2 // Max # of SUBMENU options for any menu option
bool menuMode = 0; // bool var to determine if it's in main menu (0) or sub menu(1). 


// 2-dim Array of type String. If submenu option has less then max, fill in with (" ").
String const menuOptions[MENU_COUNT][SUBMENU_COUNT+1] PROGMEM = {{"Option 0","subOpt00","subOpt01"},
                                                   {"Sensors ","Temp    ","subOpt11"},
                                                   {"Option 2","subOpt20","subOpt21"}, 
                                                   {"Option 3","subOpt30","subOpt31"}, 
                                                   {"Option 4","subOpt40","subOpt41"},
                                                   {"Option 5","subOpt50","subOpt51"}};

void setup() {
  // setup code sets up the 1st page of the main menu
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("*");
  lcd.print(menuOptions[0][0]);
  lcd.setCursor(6,1);
  lcd.print(menuOptions[1][0]);
  Serial.begin(9600);
  menuMode = 0;
  lcd.setBacklight(GREEN);
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
            lcd.print(F("RIGHT"));
            delay(300);
            break;
       }
       case BUTTON_LEFT:{
             lcd.print(F("LEFT ")); //  push button "LEFT" and show the word on the screen
             Z = menuLeft(Z);
             delay(300);
             break;
       }    
       case BUTTON_UP:{
             lcd.print(F("UP   "));  //  push button "UP" and show the word on the screen
             Z = menuUp(Z);
             delay(300);
             break;
       }
       case BUTTON_DOWN:{
             lcd.print(F("DOWN "));  //  push button "DOWN" and show the word on the screen
             Z = menuDown(Z);
             delay(300);
             break;      
       }
       case BUTTON_SELECT:{
             lcd.print(F("SEL  "));  //  push button "SELECT" and show the word on the screen
             Z = menuSel(Z);
             delay(300);
             break;
       }
       default :{
           lcd.print(F("NONE "));  //  No action  will show "None" on the screen
           lcd.setCursor(0,1);
           lcd.print(Z);
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
     
    if(K % 2 == 1){  // If menu option and even position number
      lcd.setCursor(6,0);
      lcd.print(menuOptions[d][K]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[d][K+1]);
    }
    else if(K % 2 == 0){  // If sub option and odd position number
      lcd.setCursor(6,0);
      lcd.print(menuOptions[d][K-1]);
      lcd.setCursor(6,1);
      lcd.print(menuOptions[d][K]);
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
    
    lcd.setCursor(0,1);
    lcd.print(i);
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
    else if(menuMode == 1){ // If in Sub menu
      K += 1;
      if(K == SUBMENU_COUNT){
        K = SUBMENU_COUNT ;
      }
      else{  
        K -= 1;
      }
    }
    lcd.setCursor(0,1);
    lcd.print(i);
    menuCursor(i);
    return i;
}

int menuSel(int i){
  // If in mainMenu, put in SubMenu mode (mainMenu == 1) 
  // Increment K (position in sub menu)
  if (menuMode == 0) {
    menuMode = 1;
    K += 1 ;
    lcd.setCursor(0,1);
    lcd.print(i);
    menuCursor(i);  
    return i;
  }
  // Else you are already in subMenu and want to trigger 
  // an action
  else{
    if(Z == 1 && K == 1){
      Serial.print(F("This is right before the dataLog code for the temp sensor"));  // Debug code
      RTCdatalog();
    }
  }
}

int menuLeft(int i){
  menuMode = 0;
  K = 0 ;
  i = 0 ;
  lcd.setCursor(0,1);
  lcd.print(i);
  menuCursor(i);
  return(i);
}


