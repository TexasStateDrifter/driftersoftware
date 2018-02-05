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
# include <LiquidCrystal.h> 

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
#define menuOption0 "Option 0"
#define menuOption1 "Option 1"
#define menuOption2 "Option 2"
#define menuOption3 "Option 3"
#define menuOption4 "Option 4"
#define menuOption5 "Option 5"
#define subOption00 "subOpt0-0"
#define subOption01 "subOpt0-1"
#define subOption10 "subOpt1-0"
#define subOption11 "subOpt1-1"
#define OPTION_COUNT 7 // Number of total options

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
  // setup code here, runs once at start up:
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("*");
  lcd.print(menuOption0);
  lcd.setCursor(6,1);
  lcd.print(menuOption1);
  Serial.begin(9600);
}

// Menu Counter
int Z = 0 ;

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
             lcd.print("DOWN   ");  //  push button "DOWN" and show the word on the screen
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

  // function takes menu counter and modifies lcd based on counter value
  void menuCursor(int d){
  if (d == 0) { 
    lcd.setCursor(5,0);
    lcd.print("*");
    lcd.print(menuOption0);
    lcd.setCursor(5,1);
    lcd.print(" ");
    lcd.print(menuOption1);
  }
  else if ( d == 1) {
    lcd.setCursor(5,0);
    lcd.print(" ");
    lcd.print(menuOption0);
    lcd.setCursor(5,1);
    lcd.print("*");
    lcd.print(menuOption1);
  }
  else if (d == 2){
    lcd.setCursor(6,0);
    lcd.print(menuOption1);
    lcd.setCursor(6,1);
    lcd.print(menuOption2);
  }
  else if (d ==3){
    lcd.setCursor(6,0);
    lcd.print(menuOption2);
    lcd.setCursor(6,1);
    lcd.print(menuOption3);    
  }
  else if (d ==4){
    lcd.setCursor(6,0);
    lcd.print(menuOption3);
    lcd.setCursor(6,1);
    lcd.print(menuOption4);    
  }
  else if (d == 5){
    lcd.setCursor(6,0);
    lcd.print(menuOption4);
    lcd.setCursor(6,1);
    lcd.print(menuOption5);
  }
  else if (d == 6){
    lcd.setCursor(5,0);
    lcd.print("*");
    lcd.print(subOption00);
    lcd.setCursor(5,1);
    lcd.print(" ");
    lcd.print(subOption01);
  }
  else if (d == 7){
    lcd.setCursor(5,0);
    lcd.print(" ");
    lcd.print(subOption00);
    lcd.setCursor(5,1);
    lcd.print("*");
    lcd.print(subOption01);
  }
}

// Pressing up button will decrement menu counter. Lowest value is 0. 
int menuUp(int i) {
    i = i - 1;
    if(i + 1 == 1) {
      i = 0 ;
    }
    else if (i + 1 <= 0 ){
      i = 0;
    }
    lcd.setCursor(0,1);
    lcd.print(i);
    menuCursor(i);
    return i;
}

// Pressing down button increase counter by 1. 
int menuDown(int i) {
    i = i + 1 ;
    if(i - 1 == 0) {
      i = 1;
    }
    else if (i-1 >= OPTION_COUNT) {
      i = OPTION_COUNT; 
    }  
    lcd.setCursor(0,1);
    lcd.print(i);
    menuCursor(i);
    return i;
}


int menuSel(int i){
  i = i + 6 ;
  lcd.setCursor(0,1);
  lcd.print(i);
  menuCursor(i);
  return i;
}


