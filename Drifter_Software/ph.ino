#define SensorPin 9 //pH meter Analog output to Arduino Analog Input 9
#define Offset 0.00
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp1;

/******************************************************
* GetPH
*
* Description - This function reads 10 analog reads,
* stores them into an array, sort them from small to
* large. The program will find the average of the middle 
* 6 values. From here the system will convert the volt to
* millivolt and run a function to convert to pH. The pH
* is then returned as a float
*
*
******************************************************/

float getPH()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp1=buf[i];
        buf[i]=buf[j];
        buf[j]=temp1;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue+Offset; //convert the millivolt into pH value

  return phValue;

  //digitalWrite(13, HIGH);       
  //digitalWrite(13, LOW); 
}



