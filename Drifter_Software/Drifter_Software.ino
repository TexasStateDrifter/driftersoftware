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
* for easy interaction with the drifter.
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

void menuCursor(int d){
  if (d == 0) { 
    lcd.setCursor(5,0);
    lcd.print("*Option 0");
    lcd.setCursor(5,1);
    lcd.print(" Option 1");
  }
  else if ( d == 1) {
    lcd.setCursor(5,0);
    lcd.print(" Option 0");
    lcd.setCursor(5,1);
    lcd.print("*Option 1");
  }
  else if (d == 2){
    lcd.setCursor(6,0);
    lcd.print("Option 1");
    lcd.setCursor(6,1);
    lcd.print("Option 2");
  }
  else if (d ==3){
    lcd.setCursor(6,0);
    lcd.print("Option 2");
    lcd.setCursor(6,1);
    lcd.print("Option 3");    
  }


}

int Z = 0 ;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("*Option 0");
  lcd.setCursor(6,1);
  lcd.print("Option 1");
  Serial.begin(9600);
}

 
void loop() {
  // put your main code here, to run repeatedly:
   
  lcd.setCursor(0,1);             // move to the begining of the second line
 
 
  lcd_key = read_LCD_buttons();   // read the buttons
  
  switch (lcd_key){               // depending on which button was pushed, we perform an action
    
       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
       //     lcd.print("RIGHT ");
            break;
       }
       case btnLEFT:{
       //      lcd.print("LEFT   "); //  push button "LEFT" and show the word on the screen
             break;
       }    
       case btnUP:{
       //    lcd.print("UP    ");  //  push button "UP" and show the word on the screen
             Z = Z - 1;
             if(Z+1 == 1) {
                Z = 0 ;
             }
             else if (Z + 1 <= 0 ){
                Z = 0;
             }
             lcd.setCursor(0,1);
             lcd.print(Z);
             menuCursor(Z);
             delay(400);
             break;
       }
       case btnDOWN:{
       //      lcd.print("DOWN  ");  //  push button "DOWN" and show the word on the screen
             Z = Z + 1 ;
             if(Z-1 == 0) {
                Z = 1;
             }
             else if (Z-1 >= 3) {
                Z = 3; 
             }
             lcd.setCursor(0,1);
             lcd.print(Z);
             Serial.print(Z); // prints Z to serial
             menuCursor(Z);
             delay(400);
             break;
             
       }
       case btnSELECT:{
       //      lcd.print("SELECT");  //  push button "SELECT" and show the word on the screen
             break;
       }
       case btnNONE:{
       //      lcd.print("NONE  ");  //  No action  will show "None" on the screen
           lcd.setCursor(0,1);
   lcd.print(Z);
             break;
       }
  }
   
}
