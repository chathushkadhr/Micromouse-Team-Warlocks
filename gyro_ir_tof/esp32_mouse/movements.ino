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
    if(l1<wall_thresh && l2<wall_thresh){align();}
    else if(r1<wall_thresh && r2<wall_thresh){align();} 
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
    if(l1<wall_thresh && l2<wall_thresh){align();}
    else if(r1<wall_thresh && r2<wall_thresh){align();}
    else{mpu.update();globle_theta= mpu.getAngleZ(); }

}

void breakNow(){
  int dir = 1;
  int breakCount = 100;
  int right_dir,left_dir;
  if(dir==1){right_dir = 1,left_dir = 1;}
  else{right_dir = -1,left_dir = -1;}
    int turn_speed1=0,turn_speed2=0;
    float  pre_error1 = 0,pre_error2 = 0, dt = 0, tot_error1 = 0, tot_error2 = 0;
    
    double f_t;
    count1 = 0;count2=0;
    float error1 = right_dir*breakCount, error2 = left_dir*breakCount;

    while(abs(error1)>1 || abs(error2)>1 ){
    while(abs(error1)>1 || abs(error2)>1 ){
      mpu.update();
      float turn_speed_f1 = midpwm  + kp_cell*abs(error1) + kd_cell*(error1 - pre_error1) ;
      float turn_speed_f2 = midpwm - r + kp_cell*abs(error2) + kd_cell*(error2 - pre_error2);
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
    delay(10); 
    error1 = (right_dir*breakCount - count1);
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

  void align(){
    
   float error = 0;
   int d = 0;
   
   float l1=readTOF(leftTOF1);delay(2);float l2=readTOF(leftTOF2);delay(2);
   if(l1>wall_thresh || l2>wall_thresh){
      float r1=readTOF(rightTOF1);delay(2); float r2=readTOF(rightTOF2);delay(2);
      if(r1>wall_thresh || r2>wall_thresh){
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,0);
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,0);
        delay(10);         
        return;
      }
      else{
        d = 0;
        error = r2-r1;
      }
   }
   else{
    d = 1;
    error = l1-l2;
   }
 

   

    float prevError = 0;
    float totalError = 0;
 
    while ((abs(error)>3)){
      while(abs(error)>3){
        
         int velocity =  error*kp_align + (error - prevError)*kd_align;
  
         if (velocity < -1*maxpwm){velocity = -1*maxpwm;}else if(velocity > maxpwm){velocity = maxpwm;}else{velocity = (int)velocity;}
         if(velocity!=0){
          if (abs(velocity)<210){velocity = 210 * velocity/abs(velocity); velocity = (int)velocity;}
         }

        if (velocity<0){
          ledcWrite(PWMm1a,abs(velocity));
          ledcWrite(PWMm1b,0);
          ledcWrite(PWMm2a,0);
          ledcWrite(PWMm2b,abs(velocity));
          delay(2); 
        }
        else{
          ledcWrite(PWMm1a,0);
          ledcWrite(PWMm1b,abs(velocity));
          ledcWrite(PWMm2a,abs(velocity));
          ledcWrite(PWMm2b,0);
          delay(2);
        }
  
        prevError = error;   
           if(d==1){
               error = readTOF(leftTOF1)-readTOF(leftTOF2);
           }
           else{
              error = readTOF(rightTOF2)-readTOF(rightTOF1);
           }
      }
     
     ledcWrite(PWMm1a,0);
     ledcWrite(PWMm1b,0);
     ledcWrite(PWMm2a,0);
     ledcWrite(PWMm2b,0); 
     delay(5);
     
      if(d==1){
         float l1= readTOF(leftTOF1);delay(2);
         float l2= readTOF(leftTOF2);delay(2);
         error = l1-l2;
      }
      else{
         float r1= readTOF(rightTOF1);delay(2);
         float r2= readTOF(rightTOF2);delay(2);
        error = r2-r1;
      }
    } 

    mpu.update();
    globle_theta=mpu.getAngleZ();
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
    if(l1<wall_thresh && l2<wall_thresh){align();}
    else if(r1<wall_thresh && r2<wall_thresh){align();}
 }

 void goCellDevel(){
  //
//  float kp_gyro =40, kd_gyro =80, k_tof =4;
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
    
    if (abs(tof_error)<1){tof_error = 0;}
    
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

//    if ((IR_left^prev_l_ir)||(IR_right^prev_r_ir)){
//      break;
//    }

    
    
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


 void breakNow2(){
  count1 = 0;
  count2 = 0;
  
  float error = breakCount - ((count1 + count2))/2;
  float pre_error = 0;

  while(true){
    if(abs(error)< 2){break;}
  
    while(true){ 
      if(abs(error)< 2){
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,0);
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,0);
        delay(10);
        break;
      } 
      
      float motorSpeed =  kp_cell*error + kd_cell*(error - pre_error);
    
      if (motorSpeed < -1*maxpwm){motorSpeed = -1*maxpwm;}else if(motorSpeed > maxpwm){motorSpeed = maxpwm;}else{motorSpeed = (int)motorSpeed;}
      if(motorSpeed!=0){
        if (abs(motorSpeed)<minpwm){motorSpeed = minpwm * motorSpeed/abs(motorSpeed); motorSpeed = (int)motorSpeed;}
      }
    
  
      if (motorSpeed>0){
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,abs(motorSpeed));
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,abs(motorSpeed));
        delay(2);
      }
      else{
        ledcWrite(PWMm1a,abs(motorSpeed));
        ledcWrite(PWMm1b,0);
        ledcWrite(PWMm2a,abs(motorSpeed));
        ledcWrite(PWMm2b,0);
        delay(2);
      }
      pre_error = error;
      error =  breakCount - ((count1 + count2))/2;   
    }
    error =  breakCount - ((count1 + count2))/2;  
  }
//  SerialBT.println("Cell count;");
//  SerialBT.println((count1 + count2)/2);
  
}

 void go_count(int total_count){
  count1 = 0;
  count2 = 0;
  
  float error = total_count - ((count1 + count2))/2;
  float pre_error = 0;

  while(true){
    if(abs(error)< 2){break;}
  
    while(true){ 
      if(abs(error)< 2){
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,0);
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,0);
        delay(10);
        break;
      } 
      
      float motorSpeed =  kp_cell*error + kd_cell*(error - pre_error);
    
      if (motorSpeed < -1*maxpwm){motorSpeed = -1*maxpwm;}else if(motorSpeed > maxpwm){motorSpeed = maxpwm;}else{motorSpeed = (int)motorSpeed;}
      if(motorSpeed!=0){
        if (abs(motorSpeed)<minpwm){motorSpeed = minpwm * motorSpeed/abs(motorSpeed); motorSpeed = (int)motorSpeed;}
      }
    
  
      if (motorSpeed>0){
        ledcWrite(PWMm1a,0);
        ledcWrite(PWMm1b,abs(motorSpeed));
        ledcWrite(PWMm2a,0);
        ledcWrite(PWMm2b,abs(motorSpeed));
        delay(2);
      }
      else{
        ledcWrite(PWMm1a,abs(motorSpeed));
        ledcWrite(PWMm1b,0);
        ledcWrite(PWMm2a,abs(motorSpeed));
        ledcWrite(PWMm2b,0);
        delay(2);
      }
      pre_error = error;
      error =  total_count - ((count1 + count2))/2;   
    }
    error =  total_count - ((count1 + count2))/2;  
  }
//  SerialBT.println("Cell count;");
//  SerialBT.println((count1 + count2)/2);
  
}


void adjustCell(){
// 
  float l1 = readTOF(leftTOF1);
  delay(5);
  float l2 = readTOF(leftTOF2);
  delay(5);
  float l_ir = readIR2(leftIR);
  delay(2);
//  SerialBT.println(l1);
//  SerialBT.println(" ");
//  SerialBT.println(l2);
//  SerialBT.println(" ");
//  SerialBT.println(l_ir);
//  SerialBT.println(" ");  
  
  if(  ((l1<wall_thresh)&&(l_ir<IR_THRESH) && (l2>wall_thresh))||((l1>wall_thresh)&&(l_ir>IR_THRESH) && (l2<wall_thresh))  ){
//    while( ((l_ir<IR_THRESH) && (l2>wall_thresh)) || ((l_ir>IR_THRESH) && (l2<wall_thresh)) ){
//      ledcWrite(PWMm1a,0);
//      ledcWrite(PWMm1b,260);
//      ledcWrite(PWMm2a,0);
//      ledcWrite(PWMm2b,260);
//      delay(200);
//      l2 = readTOF(leftTOF2);
//      l_ir = readIR2(leftIR);
//      delay(5);
//    }
//    ledcWrite(PWMm1a,0);
//    ledcWrite(PWMm1b,0);
//    ledcWrite(PWMm2a,0);
//    ledcWrite(PWMm2b,0);
//    delay(10);
    go_count(100);
    return;
    
  }

  else if (((l2<wall_thresh)&&(l_ir<IR_THRESH) && (l1>wall_thresh))||((l2>wall_thresh)&&(l_ir>IR_THRESH) && (l1<wall_thresh)) ){
//    while(((l_ir<IR_THRESH) && (l1>wall_thresh)) || ((l_ir>IR_THRESH) && (l1<wall_thresh))){
//      ledcWrite(PWMm1a,260);
//      ledcWrite(PWMm1b,0);
//      ledcWrite(PWMm2a,260);
//      ledcWrite(PWMm2b,0);
//      delay(200);
//      l1 = readTOF(leftTOF1);
//      l_ir = readIR2(leftIR);
//      delay(5);
//    }
//    ledcWrite(PWMm1a,0);
//    ledcWrite(PWMm1b,0);
//    ledcWrite(PWMm2a,0);
//    ledcWrite(PWMm2b,0);
//    delay(10); 
    go_count(-100);   
    return;
  }


  float r1 = readTOF(rightTOF1);
  delay(5);
  float r2 = readTOF(rightTOF2);
  delay(5);
  float r_ir = readIR2(rightIR);
  delay(2);

  SerialBT.println(r1);
  SerialBT.println(" ");
  SerialBT.println(r2);
  SerialBT.println(" ");
  SerialBT.println(r_ir);
  SerialBT.println(" ");  
  
  if(  ((r1<wall_thresh)&&(r_ir<IR_THRESH) && (r2>wall_thresh))||((r1>wall_thresh)&&(r_ir>IR_THRESH) && (r2<wall_thresh))  ){
//    while( ((r_ir<IR_THRESH) && (r2>wall_thresh)) || ((r_ir>IR_THRESH) && (r2<wall_thresh)) ){
//      ledcWrite(PWMm1a,0);
//      ledcWrite(PWMm1b,260);
//      ledcWrite(PWMm2a,0);
//      ledcWrite(PWMm2b,260);
//      delay(200);
//      r2 = readTOF(rightTOF2);
//      r_ir = readIR2(rightIR);
//      delay(5);
//    }
//    ledcWrite(PWMm1a,0);
//    ledcWrite(PWMm1b,0);
//    ledcWrite(PWMm2a,0);
//    ledcWrite(PWMm2b,0);
//    delay(10);
    go_count(100);
    return;
    
  }

  else if (((r2<wall_thresh)&&(r_ir<IR_THRESH) && (r1>wall_thresh))||((r2>wall_thresh)&&(r_ir>IR_THRESH) && (r1<wall_thresh)) ){

//    while(((r_ir<IR_THRESH) && (r1>wall_thresh)) || ((r_ir>IR_THRESH) && (r1<wall_thresh))){
//      ledcWrite(PWMm1a,260);
//      ledcWrite(PWMm1b,0);
//      ledcWrite(PWMm2a,260);
//      ledcWrite(PWMm2b,0);
//      delay(200);
//      r1 = readTOF(rightTOF1);
//      r_ir = readIR2(rightIR);
//      delay(5);
//    }
//    ledcWrite(PWMm1a,0);
//    ledcWrite(PWMm1b,0);
//    ledcWrite(PWMm2a,0);
//    ledcWrite(PWMm2b,0);
//    delay(10); 
    go_count(-100);   
    return;
  }
  
}
