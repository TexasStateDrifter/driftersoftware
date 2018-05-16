//This code was written to be easy to understand.
//Code efficiency was not considered.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the EC circuit.
//An Arduino MEGA was used to test this code.


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

float ECset;
float TDSset;
float SALset;
float GRAVset;

/*
 * EC Setup 
 * 
 * This function allows the user to set each probe on
 * or off before measuring and collecting data
 */
 
void ECsetup() 
{
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(38400);                                //set baud rate for software serial port_3 to 38400
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}

/******************************************************
* Run EC
*
* Description - This function runs a loop that populates
* an input string based on EC and will end the loop when
* the string array is filled, parsed, and converted to float
*
*
******************************************************/

void runEC()                        
{
   int r = 1;
   while(r) // program will run until a conductivity measurement is read
   { 
    serialEvent3();
      

    if (input_string_complete == true) //if a string from the PC has been received in its entirety
    {                
    Serial3.print(inputstring);                       //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
    }
    
  if (sensor_string_complete == true) 
  {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) 
    {          //if the first character in the string is a digit
      //Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
        char sensorstring_array[30];                        //we make a char array
        char *EC;                                           //char pointer used in string parsing
        char *TDS;                                          //char pointer used in string parsing
        char *SAL;                                          //char pointer used in string parsing
        char *GRAV;                                         //char pointer used in string parsing
      
        float ecNumb;
        float tdsNumb;
        float salNumb;
        float gravNumb;
      
        sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array
        EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
        TDS = strtok(NULL, ",");                            //let's pars the array at each comma
        SAL = strtok(NULL, ",");                            //let's pars the array at each comma
        GRAV = strtok(NULL, ",");                           //let's pars the array at each comma
      
        ecNumb = atof(EC);           //EC is converted to float
        tdsNumb = atof(TDS);         //TDS is converted to float
        salNumb = atof(SAL);         //SAL is converted to float
        gravNumb = atof(GRAV);       //GRAV is converted to float
      
        setEC(ecNumb);               //EC is set to a setter function
        setTDS(tdsNumb);             //TDS is set to a setter function
        setSAL(salNumb);             //SAL is set to a setter function
        setGRAV(gravNumb);           //GRAV is set to a setter function
        
        r = 0;                       //r is set at zero to end the while loop
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell
  } 
  }
}


/*
 * Get/Set Functions
 * 
 * A getter and setter function that is stored and called
 * in order to return multiple values for storage
 * 
 */
 
float getEC() // Conductivity Read
{
  return ECset;
}

void setEC(float ecNumb)
{
  ECset = ecNumb;
}

//////////////////////////////////////////////////

float getTDS() // Total Dissolved Solids Read
{
  return TDSset;
}

void setTDS(float tdsNumb)
{
  TDSset = tdsNumb;
}


//////////////////////////////////////////////////

float getSAL() // Salinity Read
{
  return SALset;
}

void setSAL(float salNumb)
{
  SALset = salNumb;
}


//////////////////////////////////////////////////

float getGRAV() // Gravity Read
{
  return GRAVset;
}

void setGRAV(float gravNumb)
{
  GRAVset = gravNumb;
}
