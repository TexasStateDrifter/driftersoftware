#define DoSensorPin  A1
void funcSetup() 
{
  pinMode(DoSensorPin,INPUT);
  readDoCharacteristicValues();
}

