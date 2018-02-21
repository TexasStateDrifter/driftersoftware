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
#include <LiquidCrystal.h> 


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define MENU_COUNT 6 // Number of total MENU options -- Modify the number of rows in the array
#define SUBMENU_COUNT 2 // Max # of SUBMENU options for any menu option
bool menuMode = 0; // bool var to determine if it's in main menu (0) or sub menu(1). 


// 2x2 Array of type String. If submenu option has less then max, fill in with (" ").
String menuOptions[MENU_COUNT][SUBMENU_COUNT+1] = {{"Option 0","subOpt00","subOpt01"},
                                                  {"Option 1","subOpt10","subOpt11"},
                                                  {"Option 2","subOpt20","subOpt21"}, 
                                                  {"Option 3","subOpt30","subOpt31"}, 
                                                  {"Option 4","subOpt40","subOpt41"},
                                                  {"Option 5","subOpt50","subOpt51"}};




int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 

    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  

    return btnNONE;                // when all others fail, return this.
}


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
}

// Menu Counter, Z, Gives you the position in the main menu
// you are on. subMenu Counter, K, gives you the position in
// the subMenu you are on.
int Z = 0, K = 0; 


// Main Program 
void loop() {
 
  lcd.setCursor(0,0);             // move to the begining of the second line
  lcd_key = read_LCD_buttons();   // read the buttons
  
  switch (lcd_key){               // depending on which button was pushed, we perform an action
    
       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
            lcd.print("RIGHT");
            delay(400);
            break;
       }
       case btnLEFT:{
             lcd.print("LEFT "); //  push button "LEFT" and show the word on the screen
             Z = menuLeft(Z);
             delay(400);
             break;
       }    
       case btnUP:{
             lcd.print("UP   ");  //  push button "UP" and show the word on the screen
             Z = menuUp(Z);
             delay(400);
             break;
       }
       case btnDOWN:{
             lcd.print("DOWN ");  //  push button "DOWN" and show the word on the screen
             Z = menuDown(Z);
             delay(400);
             break;
             
       }
       case btnSELECT:{
             lcd.print("SEL  ");  //  push button "SELECT" and show the word on the screen
             Z = menuSel(Z);
             delay(400);
             break;
       }
       case btnNONE:{
           lcd.print("NONE ");  //  No action  will show "None" on the screen
           lcd.setCursor(0,1);
           lcd.print(Z);
             break;
       }
  }

}


//////////////////////////////////////////////////////////////////
// Function: menuCursor takes in a parameter "d" from one of the 
//           up, down, left, or select functions. "d" indicates 
//           the main menu position and this function takes "d"
//           and uses it to display the correct screen. 
//////////////////////////////////////////////////////////////////
void menuCursor(int d){

  // Clear the astick area. Note: I could have just used lcd.clear()
  // but using it makes the screen perform a noticable flicker.
  lcd.setCursor(5,0);
  lcd.print(" ");
  lcd.setCursor(5,1);
  lcd.print(" ");
  lcd.setCursor(6,0);

  
  if(menuMode == 0){  // If in main menu mode
    lcd.setCursor(5, d % 2);     // Set cursor to place astrick
    lcd.print("*");
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
     lcd.print("*");
     
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
      Serial.print("menuMode = ");   // debug code for sub menu navigation problem 
      Serial.print(menuMode);
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
  menuMode = 1;
  K += 1 ;
  lcd.setCursor(0,1);
  lcd.print(i);
  menuCursor(i);  
  return i;
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


