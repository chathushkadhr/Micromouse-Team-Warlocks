void selectTOF(uint8_t s){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << s);          // send byte to select bus
  Wire.endTransmission();
}

double readTOF(uint8_t s){
  selectTOF(s);
  double l = tof[s].readRangeSingleMillimeters();
  if(s==leftTOF1){l-=14;}
  if(s==leftTOF2){l-=33;}
  if(s==rightTOF1){l-=11;}
  if(s==rightTOF2){l-=23;}
  if(s==frontTOF){l-=24;}
  return l; 
}

void initTOFs(){
  for(int i=1;i<6;i++){
      selectTOF(i);
      tof[i].setTimeout(500);
      tof[i].init();
      #if defined LONG_RANGE
        // lower the return signal rate limit (default is 0.25 MCPS)
        tof[i].setSignalRateLimit(0.1);
        // increase laser pulse periods (defaults are 14 and 10 PCLKs)
        tof[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        tof[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      #endif
    
      #if defined HIGH_SPEED
        // reduce timing budget to 20 ms (default is about 33 ms)
        tof[i].setMeasurementTimingBudget(20000);
      #elif defined HIGH_ACCURACY
        // increase timing budget to 200 ms
        tof[i].setMeasurementTimingBudget(200000);
      #endif
  } 
}

void calibTOFThresh(){
  for(int i=0;i<50;i++){
    left_distance += readTOF(leftTOF1);
    delay(20);
    right_distance += readTOF(rightTOF1);
    delay(20);    
  }
  left_distance=left_distance/50;
  right_distance=right_distance/50;
}
