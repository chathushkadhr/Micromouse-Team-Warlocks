 void goCell(){
  count1 = 0; count2 = 0;
  mpu.update();
//  intitial= mpu.getAngleZ();
  while((count1 + count2)/2<cellCount){   

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta);
    
    float speedR =  midpwm+r  - (kp_gyro*error + kd_gyro*(error - pre_error)) ;  
    float speedL =  midpwm  + (kp_gyro*error + kd_gyro*(error - pre_error)) ;  
  
    if (speedR < minpwm){speedR = minpwm+r;}else if(speedR > maxpwm+r){speedR = maxpwm+r;}else{speedR = (int)speedR;}
    if (speedL < minpwm){speedL = minpwm;}else if(speedL > maxpwm){speedL = maxpwm;}else{speedL = (int)speedL;}
  
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,speedL);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,speedR);
   
    delay(2); 
    pre_error = error;

    float f = (readTOF(frontTOF));

    if (f<66){
      ledcWrite(PWMm1a,0);
      ledcWrite(PWMm1b,0);
      ledcWrite(PWMm2a,0);
      ledcWrite(PWMm2b,0);
      delay(10); 
      breakNow();
      return;
    }
       
  }  
    ledcWrite(PWMm1a,500);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,500);
    ledcWrite(PWMm2b,0);
    delay(100); 
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);
    delay(200);     
//  breakNow();
    float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
    if(l1<wall_thresh && l2<wall_thresh){align(1);}
    else if(r1<wall_thresh && r2<wall_thresh){align(0);} 
    adjustFrontDistance();
 }

void turn90(int dir){
  //dir=1 for left turn
  //dir=-1 for right turn
  //using encoders
  int right_dir,left_dir;
  
  if(dir==1){right_dir = 1,left_dir = -1;}
  else{right_dir = -1,left_dir = 1;}
  
  int turn_speed1=0,turn_speed2=0;
  float  pre_error1 = 0,pre_error2 = 0, dt = 0, tot_error1 = 0, tot_error2 = 0;
  double f_t;
  count1 = 0;count2=0;
  float error1 = right_dir*turnCount, error2 = left_dir*turnCount;

  mpu.update();
  float initial_yaw = mpu.getAngleZ();
  float desired_yaw;
  if (dir==1){desired_yaw = initial_yaw-90;}
  else {desired_yaw = initial_yaw+90;}
  
  while(abs(error1)>0.1 || abs(error2)>0.1){
      mpu.update();
      float turn_speed_f1 = kp_turn*abs(error1) + kd_turn*(error1 - pre_error1);
      float turn_speed_f2 = kp_turn*abs(error2) + kd_turn*(error2 - pre_error2);
      if (turn_speed_f1 < minpwm){turn_speed1 = minpwm;}else if(turn_speed_f1 > maxpwm){turn_speed1 = maxpwm;}else{turn_speed1 = (int)turn_speed_f1;}
      if (turn_speed_f2 < minpwm){turn_speed2 = minpwm;}else if(turn_speed_f2 > maxpwm){turn_speed2 = maxpwm;}else{turn_speed2 = (int)turn_speed_f2;}
      
      if(error1>0){
        //forward rotation
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,turn_speed1);
      }
      else{
        //backward rotation
        ledcWrite(PWMm1a,turn_speed1);
        ledcWrite(PWMm1b,0); 
      }
  
      if(error2>0){
        //forward rotation
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,turn_speed2);
      }
      else{
        //backward rotation
        ledcWrite(PWMm2a,turn_speed2);
        ledcWrite(PWMm2b,0);  
      }
      delay(5); 
      pre_error1 = error1;
      error1 = (right_dir*turnCount - count1);
  
      pre_error2 = error2;
      error2 = (left_dir*turnCount - count2);
     
    }
    
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);   
    delay(100); 
    


    mpu.update();  
    if (abs(mpu.getAngleZ() - desired_yaw)>5){
      SerialBT.println(mpu.getAngleZ() - desired_yaw);
      mpu.update();
      while(abs(mpu.getAngleZ() - desired_yaw)>0.1){
        float pre_yaw_error = 0;
        while(true){
          mpu.update();
          float yaw_error = desired_yaw - mpu.getAngleZ(); 

          if (abs(yaw_error)<0.1){
            ledcWrite(PWMm1a,0);
            ledcWrite(PWMm1b,0);
            ledcWrite(PWMm2a,0);
            ledcWrite(PWMm2b,0);
            break;
          }
          
          float speedR = (kp_gyro_turn*abs(yaw_error) + kd_gyro_turn*(yaw_error - pre_yaw_error)) ;  
          float speedL = (kp_gyro_turn*abs(yaw_error) + kd_gyro_turn*(yaw_error - pre_yaw_error)) ;  
        
          if (speedR < minpwm){speedR = minpwm;}else if(speedR > maxpwm){speedR = maxpwm;}else{speedR = (int)speedR;}
          if (speedL < minpwm){speedL = minpwm;}else if(speedL > maxpwm){speedL = maxpwm;}else{speedL = (int)speedL;}
  
          if (yaw_error>0){
            ledcWrite(PWMm1a,abs(speedL));
            ledcWrite(PWMm1b,0);
            ledcWrite(PWMm2a,0);
            ledcWrite(PWMm2b,abs(speedR));
            delay(10);
          }
          else {
            ledcWrite(PWMm1a,0);
            ledcWrite(PWMm1b,abs(speedL));
            ledcWrite(PWMm2a,abs(speedR));
            ledcWrite(PWMm2b,0);
            delay(10);
          }        
          pre_yaw_error = yaw_error;
        }
        mpu.update();  
      }
          
    }
    
    float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
    if(l1<wall_thresh && l2<wall_thresh){align(1);}
    else if(r1<wall_thresh && r2<wall_thresh){align(0);}
    else{mpu.update();globle_theta= mpu.getAngleZ(); }

}

void breakNow(){
  int dir = 1;
  int breakCount = 2;
  int right_dir,left_dir;
  if(dir==1){right_dir = 1,left_dir = 1;}
  else{right_dir = -1,left_dir = -1;}
    int turn_speed1=0,turn_speed2=0;
    float  pre_error1 = 0,pre_error2 = 0, dt = 0, tot_error1 = 0, tot_error2 = 0;
    
    double f_t;
    count1 = 0;count2=0;
    float error1 = right_dir*breakCount, error2 = left_dir*breakCount;

    while(abs(error1)>0.1 || abs(error2)>0.1 ){
      mpu.update();
      float turn_speed_f1 = midpwm  + kp_cell*abs(error1) + kd_cell*1000*(error1 - pre_error1) ;
      float turn_speed_f2 = midpwm - r + kp_cell*abs(error2) + kd_cell*1000*(error2 - pre_error2);
      if (turn_speed_f1 < minpwm){turn_speed1 = minpwm;}else if(turn_speed_f1 > maxpwm){turn_speed1 = maxpwm;}else{turn_speed1 = (int)turn_speed_f1;}
      if (turn_speed_f2 < minpwm){turn_speed2 = minpwm - r;}else if(turn_speed_f2 > maxpwm){turn_speed2 = maxpwm - r;}else{turn_speed2 = (int)turn_speed_f2;}
      
      if(error1>0){
        //forward rotation
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,turn_speed1);
      }
      else{
        //backward rotation
        ledcWrite(PWMm1a,turn_speed1);
        ledcWrite(PWMm1b,0); 
      }

      if(error2>0){
        //forward rotation
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,turn_speed2);
      }
      else{
        //backward rotation
        ledcWrite(PWMm2a,turn_speed2);
        ledcWrite(PWMm2b,0);  
      }
      delay(5); 

      
      
      pre_error1 = error1;
      tot_error1 += error1;
      error1 = (right_dir*breakCount - count1);

      pre_error2 = error2;
      tot_error2 += error2;
      error2 = (left_dir*breakCount - count2);

      
      
    }

    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);
    delay(10);
    
//    float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
//    if(l1<wall_thresh && l2<wall_thresh){align(1);}
//    else if(r1<wall_thresh && r2<wall_thresh){align(0);}
//    
//    adjustFrontDistance();
//    
//    r1 = readTOF(rightTOF1);r2 = readTOF(rightTOF2);l1 =readTOF(leftTOF1); l2 = readTOF(leftTOF2);
//    if(l1<wall_thresh && l2<wall_thresh){align(1);}
//    else if(r1<wall_thresh && r2<wall_thresh){align(0);}  
  
   
}
void goCellTOF(){
  count1 = 0; count2 = 0;
  mpu.update();
//  intitial= mpu.getAngleZ();
  while((count1 + count2)/2<firstCount){   

    float distance_error = 0;
    
    float l1 = readTOF(leftTOF1)/10;
    float l2 = readTOF(leftTOF2)/10; 
    float r1 = readTOF(rightTOF1)/10;
    float r2 = readTOF(rightTOF2)/10; 


    
    if (l1<15 && l2<15){
      distance_error = (l2 - l1);
      if (l2<6){
          distance_error +=(6 - l2)*ktof; 
      }
    }
    else if (r1<15 && r2<15){
      distance_error = -1*(r2 - r1);
      if (r2<6){
          distance_error -=(6 - r2)*ktof; 
      }      
    }
   
    float error = distance_error;
    
    float speedR =  midpwm+r  + (kp_tof*error + kd_tof*(error - pre_error)) ;  
    float speedL =  midpwm  - (kp_tof*error + kd_tof*(error - pre_error)) ;  
  
    if (speedR < minpwm){speedR = minpwm+r;}else if(speedR > maxpwm+r){speedR = maxpwm+r;}else{speedR = (int)speedR;}
    if (speedL < minpwm){speedL = minpwm;}else if(speedL > maxpwm){speedL = maxpwm;}else{speedL = (int)speedL;}
    
  
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,speedL);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,speedR);
   
    delay(2); 
    pre_error = error;

    float f = (readTOF(frontTOF));
    ;
    if (f<66){
      ledcWrite(PWMm1a,0);
      ledcWrite(PWMm1b,0);
      ledcWrite(PWMm2a,0);
      ledcWrite(PWMm2b,0);
      delay(10); 
      breakNow();
      return;
    }
       
  }  
    ledcWrite(PWMm1a,500);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,500);
    ledcWrite(PWMm2b,0);
    delay(100); 
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);
    delay(200);     
//  breakNow();
  
 }

  void align(int d){
    //d=1 for left, d=0 for right
    
   float error ;
   if(d==1){
       error = readTOF(leftTOF1)-readTOF(leftTOF2);
//       kp_align=3;kd_align=10 ;ki_align=0; //SLIIT 2,1
       //5 //5 //0
      }
    else{
      error = readTOF(rightTOF2) -readTOF(rightTOF1);
//      kp_align=3;kd_align=10 ;ki_align=0;
      //5 //8 //0
      
      }
  
    
    float prevError = 0;
    float totalError = 0;
    bool isPos = false;

    if (error>=0){isPos = true;}

    while ((abs(error)>3)){
      
   
      while(abs(error)>3){
  
         if ((isPos && error<0)||(!isPos && error>=0)){totalError=0;}
          
         totalError +=error;
        
         float Pterm = error*kp_align;
         float Dterm = (error - prevError)*kd_align;
         float Iterm =  totalError*ki_align;
  
         float pidVal = Pterm + Dterm + Iterm;
         int velocity = (int) pidVal;
  
         if (velocity>=maxSpeed){
            velocity=maxSpeed;
            delay(2); 
          }
        else if (velocity<=-1*maxSpeed){
            velocity=-1*maxSpeed;
            delay(2); 
          }
  
        //int shifted= ((abs(velocity)*(600-300))/600)+300;
//        float shiftedR = map(abs(velocity),0,maxSpeed,230,400);
//        float shiftedL = map(abs(velocity),0,maxSpeed,230,400);
        float shiftedR = map(abs(velocity),0,maxSpeed,210,500);
        float shiftedL = map(abs(velocity),0,maxSpeed + 80,210,500);
        
        if (velocity>=0){
          ledcWrite(PWMm1a,0);
          ledcWrite(PWMm1b,shiftedR);
          ledcWrite(PWMm2a,shiftedL);
          ledcWrite(PWMm2b,0);
          delay(2); 
          }
        else{
          ledcWrite(PWMm1a,shiftedR);
          ledcWrite(PWMm1b,0);
          ledcWrite(PWMm2a,0);
          ledcWrite(PWMm2b,shiftedL);
          delay(2);
          }
  
        prevError = error;   
           if(d==1){
               error = readTOF(leftTOF1)-readTOF(leftTOF2);
           }
           else{
              error = readTOF(rightTOF2)-readTOF(rightTOF1);
           }
  //      SerialBT.print(error);
  //      SerialBT.print(" ");
  //      SerialBT.println(shifted);
      }
     
     ledcWrite(PWMm1a,0);
     ledcWrite(PWMm1b,0);
     ledcWrite(PWMm2a,0);
     ledcWrite(PWMm2b,0); 
     delay(50);
      if(d==1){
         error = readTOF(leftTOF1)-readTOF(leftTOF2);
      }
      else{
        error = readTOF(rightTOF2) -readTOF(rightTOF1);
      }
  }
//  //Printing error################
//  if(d==1){
//         error = readTOF(leftTOF1)-readTOF(leftTOF2);
//      }
//      else{
//        error = readTOF(rightTOF2) -readTOF(rightTOF1);
//      }
//    SerialBT.println(error);
    //############################
    mpu.update();
    globle_theta=mpu.getAngleZ();
}


void goForwardspecificDistanceLeft() {
  firstCount = 220;
  kpcell = 2;
  kdcell = 0;
  ktof = 2;
  int secondCount=0;
  count1 = 0;
  count2 = 0;
  int mid_Speed = midpwm;
  int min_pwm = minpwm;
  int max_pwm = maxpwm;
  int totalCount = firstCount + secondCount;
  int rightMotorSpeed,leftMotorSpeed,decreaseSpeed;
  
  float previousError = 0;
  float enError;
  //k_tof=0.35;
  while ((count1 + count2) / 2 < totalCount) {

    //enError = count1-count2;
    double t1 = readTOF(leftTOF1);
    double t2 = readTOF(leftTOF2);

    enError =0;

    if (t1<150 && t2<150){
         enError = (t2-t1)/10 ;
         if (t2<65){
          enError +=(6.5- t2/10)*ktof; 
          }
      }

    
    
    
    int motorSpeed = enError * kpcell  + (enError - previousError) * kdcell;
    previousError = enError;
    //OLED(0, 0, 1);

    if ((count1 + count2) / 2 < firstCount) {
      rightMotorSpeed = constrain((mid_Speed - motorSpeed), min_pwm, max_pwm);
      leftMotorSpeed  = constrain((mid_Speed + motorSpeed), min_pwm, max_pwm);
    } else {
      //minSpeed = 50;
      decreaseSpeed = map((count1 + count2) / 2, firstCount, totalCount, mid_Speed, min_pwm);
      rightMotorSpeed = constrain((decreaseSpeed - motorSpeed), min_pwm, 2 * decreaseSpeed - min_pwm);
      leftMotorSpeed  = constrain((decreaseSpeed + motorSpeed), min_pwm, 2 * decreaseSpeed - min_pwm);
    }
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,rightMotorSpeed);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,leftMotorSpeed);

    float f = (readTOF(frontTOF));

    if (f<66){
      ledcWrite(PWMm1a,0);
      ledcWrite(PWMm1b,0);
      ledcWrite(PWMm2a,0);
      ledcWrite(PWMm2b,0);
      delay(10); 
      breakNow();
      break;
    }

  }
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);
    delay(200);
       float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
    if(l1<wall_thresh && l2<wall_thresh){align(1);}
    else if(r1<wall_thresh && r2<wall_thresh){align(0);}
    
    adjustFrontDistance();
    

    
}

void goForwardspecificDistanceRight() {
  firstCount = 205;
  kpcell = 4;
  kdcell = 0;
  ktof = 2;
  int secondCount=0;
  count1 = 0;
  count2 = 0;
  int mid_Speed = midpwm;
  int min_pwm = midpwm;
  int max_pwm = maxpwm;
  int totalCount = firstCount + secondCount;
  int rightMotorSpeed,leftMotorSpeed,decreaseSpeed;
  
  float previousError = 0;
  float enError;
//  k_tof=0.45;
  while ((count1 + count2) / 2 < totalCount) {

    //enError = count1-count2;
    double t1 = readTOF(rightTOF1);
    double t2 = readTOF(rightTOF2);

    enError =0;

    if (t1<150 && t2<150){
         enError = (t1-t2)/10 ;
         if (t2<65){
          enError -=(6.5 - t2/10)*ktof; 
          }
      }

    
    
    
    int motorSpeed = enError * kpcell  + (enError - previousError) * kdcell;
    previousError = enError;
    //OLED(0, 0, 1);

    if ((count1 + count2) / 2 < firstCount) {
      rightMotorSpeed = constrain((mid_Speed - motorSpeed), min_pwm, max_pwm);
      leftMotorSpeed  = constrain((mid_Speed + motorSpeed), min_pwm, max_pwm);
    } else {
      //minSpeed = 50;
      decreaseSpeed = map((count1 + count2) / 2, firstCount, totalCount, midSpeed, minpwm);
      rightMotorSpeed = constrain((decreaseSpeed - motorSpeed), min_pwm, 2 * decreaseSpeed - min_pwm);
      leftMotorSpeed  = constrain((decreaseSpeed + motorSpeed), min_pwm, 2 * decreaseSpeed - min_pwm);
    }
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,rightMotorSpeed);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,leftMotorSpeed);

    float f = (readTOF(frontTOF));
    if (f<66){
      ledcWrite(PWMm1a,0);
      ledcWrite(PWMm1b,0);
      ledcWrite(PWMm2a,0);
      ledcWrite(PWMm2b,0);
      delay(10); 
      breakNow();
      break;
    }    

  }

  
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,0);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,0);
    delay(200);
    float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
    if(l1<wall_thresh && l2<wall_thresh){align(1);}
    else if(r1<wall_thresh && r2<wall_thresh){align(0);}
    
    adjustFrontDistance();

    
}

void adjustFrontDistance(){

//  while(true){
    float f = readTOF(frontTOF);

    if (f<30){
      int maximum_speed = maxpwm;
      while (f<30){
             int ii =(60-f)*12;
             int x = max(ii , minpwm);
             ledcWrite(PWMm1a,min(x,maximum_speed));
             ledcWrite(PWMm1b,0);
             ledcWrite(PWMm2a,min(x,maximum_speed));
             ledcWrite(PWMm2b,0); 
             delay(2);
             f = readTOF(frontTOF);
  //           SerialBT.println(f);
      } 
       ledcWrite(PWMm1a,0);
       ledcWrite(PWMm1b,0);
       ledcWrite(PWMm2a,0);
       ledcWrite(PWMm2b,0);     
       delay(50);
  //     SerialBT.println("Done");
    }

   else if(f<140 && f>62){
    int maximum_speed = maxpwm;
      while (f>62){
             int ii =(f-82)*25;
             int y = max( ii , minpwm);
             ledcWrite(PWMm1a,0);
             ledcWrite(PWMm1b, min(y,maximum_speed));
             ledcWrite(PWMm2a,0);
             ledcWrite(PWMm2b,min(y,maximum_speed)); 
             delay(10);
             f = readTOF(frontTOF);
  //           SerialBT.println(f);
      } 
       ledcWrite(PWMm1a,0);
       ledcWrite(PWMm1b,0);
       ledcWrite(PWMm2a,0);
       ledcWrite(PWMm2b,0);     
       delay(50);
  } 

  else if(f>140){
      float l1 = readTOF(leftTOF1),l2 = readTOF(leftTOF2), r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2);
      bool expresion = (l1<wall_thresh ^ l2<wall_thresh) || (r1<wall_thresh ^ r2<wall_thresh);

      bool go_front;
      
      if (l1<wall_thresh ^ l2<wall_thresh){
        if(prev_left == l2<wall_thresh){go_front=true;}
        else{go_front=false;}
      }

      else if (r1<wall_thresh ^ r2<wall_thresh){
        if(prev_right == r2<wall_thresh){go_front=true;}
        else{go_front=false;}      
      }
      
      while(expresion){
             if(go_front){
               ledcWrite(PWMm1a,0);
               ledcWrite(PWMm1b,280);
               ledcWrite(PWMm2a,0);
               ledcWrite(PWMm2b,280); 
               delay(2);}
             else{
               ledcWrite(PWMm1a,250);
               ledcWrite(PWMm1b,0);
               ledcWrite(PWMm2a,250);
               ledcWrite(PWMm2b,0); 
               delay(2);}          
               l1 = readTOF(leftTOF1),l2 = readTOF(leftTOF2),r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2);
               expresion = (l1<wall_thresh ^ l2<wall_thresh) || (r1<wall_thresh ^ r2<wall_thresh);    
      }  
             
      } 
       ledcWrite(PWMm1a,0);
       ledcWrite(PWMm1b,0);
       ledcWrite(PWMm2a,0);
       ledcWrite(PWMm2b,0);     
       delay(100);

    float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
    if(l1<wall_thresh && l2<wall_thresh){align(1);}
    else if(r1<wall_thresh && r2<wall_thresh){align(0);}
 }

 void goCellDevel(){
  //
  float kp_gyro =40, kd_gyro =80, k_tof =4;
//  int cellCount =260;
  
  count1 = 0; count2 = 0;
  mpu.update();

  bool IR_prev_l = readIR2(leftIR)<IR_THRESH;
  bool IR_prev_r = readIR2(rightIR)<IR_THRESH;
  
  while((count1 + count2)/2<cellCount){ 
    float tof_error =0;
    
    if (prev_l){
      float l1= readTOF(leftTOF1);
      delay(2);
      if(l1 < wall_thresh){
        tof_error = (l1 - left_distance)/10;
      }
      else{
        float r1= readTOF(rightTOF1);
        delay(2);
        if(r1 < wall_thresh){
          tof_error = (right_distance - r1)/10;
          prev_l =false; prev_r = true;
        }  
      }
    }

    else if (prev_r){
      float r1= readTOF(rightTOF1);
      delay(2);
      if(r1 < wall_thresh){
        tof_error = (right_distance - r1)/10;
      }
      else{
        float l1= readTOF(leftTOF1);
        delay(2);
         if(l1 < wall_thresh){
          tof_error = (l1 - left_distance)/10;
          prev_r =false; prev_l = true;
        }  
      }
    }
    
     
    
    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + tof_error*k_tof ;
    
    float speedR =  midpwm+r  - (kp_gyro*error + kd_gyro*(error - pre_error)) ;  
    float speedL =  midpwm  + (kp_gyro*error + kd_gyro*(error - pre_error)) ;  
  
    if (speedR < minpwm){speedR = minpwm+r;}else if(speedR > maxpwm+r){speedR = maxpwm+r;}else{speedR = (int)speedR;}
    if (speedL < minpwm){speedL = minpwm;}else if(speedL > maxpwm){speedL = maxpwm;}else{speedL = (int)speedL;}
    
  
  
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,speedL);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,speedR);
   
    delay(2); 
    pre_error = error;

    bool IR_left = readIR(leftIR)<IR_THRESH;
    bool IR_right = readIR(rightIR)<IR_THRESH;

    
    
    float f = (readTOF(frontTOF));

    if (f<120){
      float speed_front = 0;
      
      while(true){
          float error_front = 0;
          float pre_error_front = 0;
          while(true){
            if ( abs(f-38) <2){break;}
            
            error_front = (f - 38);
          
            speed_front =  (kp_front_tof*error_front + kd_front_tof*(error_front - pre_error_front)) ;  
    
          
            if (speed_front < -1*maxpwm){speed_front = -1*maxpwm;}
            if (speed_front > maxpwm){speed_front = maxpwm;}

            if (abs(speed_front)<minpwm){speed_front = minpwm * (speed_front/abs(speed_front));}
            
            speed_front = (int)speed_front;
          
            if (speed_front>0){
              ledcWrite(PWMm1a,0);
              ledcWrite(PWMm1b,speed_front);
              ledcWrite(PWMm2a,0);
              ledcWrite(PWMm2b,speed_front);         
            }
            else{
              ledcWrite(PWMm1a,-1*speed_front);
              ledcWrite(PWMm1b,0);
              ledcWrite(PWMm2a,-1*speed_front);
              ledcWrite(PWMm2b,0);
            }
            delay(2);
            
            pre_error_front = error_front;
            f = (readTOF(frontTOF));   
        } 
        f = (readTOF(frontTOF));
        if (abs(f-38) <2){break;}       
      }
      
      ledcWrite(PWMm1a,0);
      ledcWrite(PWMm1b,0);
      ledcWrite(PWMm2a,0);
      ledcWrite(PWMm2b,0);
      delay(10); 
      Buzz(cellTone);
      return;
    }
       
  }  
    Buzz(cellTone);
//    breakNow();
    ledcWrite(PWMm1a,0);
    ledcWrite(PWMm1b,210);
    ledcWrite(PWMm2a,0);
    ledcWrite(PWMm2b,210);
    delay(1);     


 }
