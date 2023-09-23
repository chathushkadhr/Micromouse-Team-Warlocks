void fast_run_test(){
 mpu.update();
 globle_theta= mpu.getAngleZ();
 SerialBT.println("Give value for cellCountFast:");
 cellCountFast = BTreadInt();
 blinkLED(1);
 int commands_size = 15;
 int commands[commands_size] = {-6,DIR_LEFT,-3,DIR_LEFT,-4,DIR_LEFT,-1,DIR_RIGHT,-1,DIR_LEFT,-2,DIR_RIGHT,-1,DIR_LEFT,DIR_LEFT}; 
 for (int i = 0; i < commands_size; i++)
    {
        // Serial.print(fast_run_commands[i]);
        // Serial.print(" ");
        int f_cmd = commands[i];
        if(f_cmd<0){
            f_cmd = -f_cmd;
            for(int i=0;i<f_cmd;i++){
                if (i==f_cmd-1){
                goCellFast(true);
                }
                else{
                  goCellFast(false);
                }
            }
        }
        else if(f_cmd==DIR_LEFT){
            turn_align(1);
        }
        else if(f_cmd==DIR_RIGHT){
            turn_align(-1);
        }
        else{
            // dont remove;
        }
    }          
}
 
 
 void alignTest(){
//    SerialBT.println("Give value for kp:");
//    kp_align = BTreadFloat();
//
//    SerialBT.println("Give value for kd:");
//    kd_align = BTreadFloat();
    align();
    delay(1000);
 }
  void IRtest(){
    float r=0;
      SerialBT.print("right : ");
      r=readIR(rightIR);
  SerialBT.println(String(r));
  delay(5);
  r=readIR(leftIR);
    SerialBT.print("left : ");
  SerialBT.println(String(r)); delay(5);
  }
  
  void cellRunTest(){
//    SerialBT.println("Give value for cell count:");
//    cellCount = BTreadInt();
//      SerialBT.println("Number of iterations");
//      it = BTreadInt();
//      SerialBT.println("Give value for kp:");
//    kp_cell = BTreadFloat();
//////
//    SerialBT.println("Give value for kd:");
//    kd_cell = BTreadFloat();
//    SerialBT.println("Give value for k_tof:");
//    k_tof = BTreadFloat();
//      

//
//    SerialBT.println("Give value for r:");
//    r = BTreadInt();
    mpu.update();
    globle_theta= mpu.getAngleZ();
//    blinkLED(1);
    for (int i=0;i<it;i++){
      goCellDevelIR();
    }
    breakNow2(breakCount);
    
 }


void IR_cell_run(){
//    SerialBT.println("Give value for cell count:");
//    cellCount = BTreadInt()  ;
    SerialBT.println("Give value for k_ir:");
    k_ir = BTreadFloat();
  
    mpu.update();
    globle_theta= mpu.getAngleZ();
    for (int i=0;i<5;i++){
      goCellDevelIR();
    }    
}
void turnTest(){
//    SerialBT.println("Give value for kp:");
//    kp_gyro_turn = BTreadFloat();
//    SerialBT.println("Give value for kd:");
//    kd_gyro_turn = BTreadFloat();
    //SerialBT.println("Give value for cell count:");
    //turnCount = BTreadInt();
    blinkLED(2);
    turn90(1);
    blinkLED(2);
    turn90(-1);

}
 

void tofTest(){
  float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
//  
//  SerialBT.print("left1 : ");
//  SerialBT.println(l1);
//  SerialBT.print("left2 : ");
//  SerialBT.println(l2);
////  SerialBT.print("left error : ");
////  SerialBT.println(l1-l2);
//  SerialBT.print("right1 : ");
//  SerialBT.println(r1);
//  SerialBT.print("right2 : ");
//  SerialBT.println(r2);
//  SerialBT.print("right error : ");
//  SerialBT.println(r1-r2);
  float f = readTOF(frontTOF);
  SerialBT.print("front : ");
  SerialBT.println(f);
  
  float IR_l = readIR2(leftIR);
  float IR_r = readIR2(rightIR);
  
  SerialBT.print("Left IR : ");
  SerialBT.println(IR_l);  
    SerialBT.print("Right IR : ");
  SerialBT.println(IR_r);
  //,b = readTOF(backTOF);
  //SerialBT.println(readTOF(rightTOF1));

  //SerialBT.print("back : ");
  //SerialBT.println(b);
  
  SerialBT.println("*");
}
//
//void straightRunTest(){
////     SerialBT.println("Give value for first count:");
////    int firstCount = BTreadInt();
////    SerialBT.println("Give value for second count:");
////    int secondCount = BTreadInt();
////    SerialBT.println("max speed:");
////    maxSpeed = BTreadInt();
////    SerialBT.println("min speed:");
////    minpwm = BTreadInt();
//    SerialBT.println("Give count:");
//    firstCount = BTreadInt();
////    SerialBT.println("Give value for kp:");
////    kpcell = BTreadFloat();
////    SerialBT.println("Give value for kd:");
////    kdcell = BTreadFloat();
////    SerialBT.println("Give value for ktof");
////    ktof = BTreadFloat();
//    
////    SerialBT.println("Give value for kd:");
////    kd_cell = BTreadFloat();
////    midSpeed = (maxSpeed+minpwm)/2;
//    blinkLED(2);
////    goCellTOF();
//    goForwardspecificDistanceLeft();
//     SerialBT.println(count1);
//    SerialBT.println(count2);
//    
//}
  
void blinkLED(int x){
  for(int j=0;j<x;j++){digitalWrite(2,HIGH);delay(1000);digitalWrite(2,LOW);delay(1000);}
  }

int BTreadInt(){
  int x;
     while(!SerialBT.available()){}
    while(SerialBT.available()){
      x = SerialBT.readString().toInt();
      }
      SerialBT.println("Got :"+String(x));
      return x;
 }

  float BTreadFloat(){
  float x;
     while(!SerialBT.available()){}
    while(SerialBT.available()){
      x = SerialBT.readString().toFloat();
      }
      SerialBT.println("Got :"+String(x));
      return x;
      
 }

 void Buzz(int c){
  if(c==0){

    digitalWrite(buzzPin, HIGH);
    delay(2);
    digitalWrite(buzzPin, LOW);
  }
  else{
    for(int i=0;i<c;i++){
      digitalWrite(buzzPin, HIGH);
    delay(50);
    digitalWrite(buzzPin, LOW);
    delay(50);
    }
  }
  
 }
