float readIR(int t)
{
  
      float voltage_temp_average=0;
      
      for(int i=0; i < AVERAGE_OF; i++)
    {
      int sensorValue = analogRead(t);
      delay(1);      
      voltage_temp_average +=sensorValue * (3.3 / 4095.0);
    }
     voltage_temp_average /= AVERAGE_OF;

  // eqution of the fitting curve
  ////33.9 + -69.5x + 62.3x^2 + -25.4x^3 + 3.83x^4
  float d=  33.9 + -69.5*(voltage_temp_average) + 62.3*pow(voltage_temp_average,2) + -25.4*pow(voltage_temp_average,3) + 3.83*pow(voltage_temp_average,4);
  return d;
}

float readIR2(int t)
{
    float d1;
    float d2;
    int sensorValue;
    float voltage1;
    float voltage2;
    
    while(true){
      sensorValue = analogRead(t);
      delay(1);      
      voltage1 =sensorValue * (3.3 / 4095.0);
      sensorValue = analogRead(t);
      delay(1); 
      voltage2 =sensorValue * (3.3 / 4095.0);  
       
      d1=  33.9 + -69.5*(voltage1) + 62.3*pow(voltage1,2) + -25.4*pow(voltage1,3) + 3.83*pow(voltage1,4);
      d2=  33.9 + -69.5*(voltage2) + 62.3*pow(voltage2,2) + -25.4*pow(voltage2,3) + 3.83*pow(voltage2,4);

      if (abs(d1-d2)<4){
        break; 
      }
    }
    
  return (d1+d2)/2;
}
