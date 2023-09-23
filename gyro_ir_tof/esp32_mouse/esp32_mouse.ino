#include "Wire.h"
#include "BluetoothSerial.h"
#include <VL53L0X.h> //library for ToF
#include <MPU6050_light.h>
 
//motors
#define m1a 4  ///17 //Left
#define m1b 16  //18
#define m2a 18  //16  //Right
#define m2b 17  //4

//encoders
#define en1a 33
#define en1b 27
#define en2a 25
#define en2b 26

//tof
#define frontTOF 3
#define backTOF 0
#define rightTOF1 2
#define rightTOF2 1
#define leftTOF1 4
#define leftTOF2 5
#define wall_thresh 100

//SharpIR 
#define rightIR 35 
#define leftIR 34 
//#define rightIR2 2
//#define leftIR2 32
#define AVERAGE_OF 1
#define IR_THRESH 10

//Buzzer
#define buzzPin 2
#define cellTone 0
#define destinationTone 3

//pwm props
const int PWMFreq = 1000;
const int PWMm1a = 0;
const int PWMm1b = 2;
const int PWMm2a = 4;
const int PWMm2b = 6;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
int midSpeed = 400;
int maxSpeed = 500;
int maxpwm =320;
int minpwm =240; //for 10 bit resolution
int midpwm =280;

//int maxpwm =400;
//int minpwm =240; //for 10 bit resolution
//int midpwm =320;

//encoders
volatile int count1=0; //right motor
volatile int count2=0; //left motor

//CELL RUN GYRO PARAMS
float kp_gyro=40,kd_gyro=40; 
float pre_error=0;
int cellCount = 230; //150;
int it = 1;
float k_tof = 4;
float k_ir = 3;

//breakNow2 params
float kp_cell=1.6,kd_cell=20;
float breakCount = 50;

//CELL RUN TOF PARAMS
float kp_tof = 0;
float kd_tof = 0;
float kp_front_tof= 10;
float kd_front_tof= 20;
//float ktof = 0;
//int firstCount = 140;

//WALL CELL RUN PARAMS
float kpcell=20,kdcell=30,kicell=0,ktof=0.45;
int firstCount = 190;

//TURN ENCODER PARAMS
float kp_turn = 4;
float kd_turn = 2;
float kp_gyro_turn = 4;
float kd_gyro_turn = 2;
int turnCount = 120;

//ALIGN PARAMS
float kp_align=8,kd_align=16 ,ki_align=0;
int min_align=250,max_align=300;
int tof_side;

int r = 0;
float intitial;
VL53L0X s1,s2,s3,s4,s5,s6;
VL53L0X tof[6] = {s1,s2,s3,s4,s5,s6};

//MPU
MPU6050 mpu(Wire);

float globle_theta;
BluetoothSerial SerialBT;

bool prev_left = true;
bool prev_right = true;

//save previous IR
bool prev_l_ir = true;
bool prev_r_ir = true;

//Save previous tof state
bool prev_l = true;
bool prev_r = false;

//TOF wall following thresh
float right_distance= 0;
float left_distance= 0;

float ir_left_distance= 0;
float ir_right_distance= 0;

int need_break = 1;
//############################################
#define DIM 10
#define TOTAL DIM*DIM+1
#define CENTER 0.5*DIM*(DIM-1)

int DESTINATION=64; //Up left corner cell of the destination

/* dir:
 1 - east
 2 - west
 3 - south
 4 - north
 6 - left
 7 - right
 8 - back 
 -1 - forward */

bool commingBack = false;

//int walls[TOTAL][4] =  {{0, 0, 0, 0},{2, 0, 0, 0},{1, 3, 0, 0},{2, 0, 0, 0},{10, 0, 0, 0},{11, 0, 0, 0},{0, 0, 0, 0},{8, 0, 0, 0},{7, 0, 0, 0},{15, 0, 0, 0},{16, 4, 0, 0},{5, 0, 0, 0},{0, 0, 0, 0},{19, 0, 0, 0},{15, 0, 0, 0},{14, 9, 0, 0},{10, 17, 0, 0},{16, 18, 23, 0},{17, 0, 0, 0},{13, 0, 0, 0},{21, 0, 0, 0},{20, 27, 0, 0},{28, 0, 0, 0},{29, 17, 0, 0},{0, 0, 0, 0},{26,0, 0, 0},{25, 27, 0, 0},{26, 21, 0, 0},{22, 34, 0, 0},{23, 30, 0, 0},{29, 0, 0, 0},{32, 0, 0, 0},{31, 0, 0, 0},{0,0, 0, 0},{28, 35, 0, 0},{34, 0, 0, 0},{0, 0, 0, 0}};
//int walls[TOTAL][4] = {{0, 0, 0, 0}, {2, 0, 0, 0}, {1, 3, 0, 0}, {2, 0, 0, 0}, {5, 0, 0, 0}, {4, 6, 0, 0}, {5, 0, 0, 0}, {8, 0, 0, 0}, {7, 0, 0, 0}, {15, 10, 0, 0}, {16, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {19, 0, 0, 0}, {15, 0, 0, 0}, {9, 21, 14, 16}, {10, 22, 15, 0}, {18, 0, 0, 0}, {24, 17, 0, 0}, {13, 0, 0, 0}, {26, 0, 0, 0}, {15, 22, 0, 0}, {16, 21, 0, 0}, {24, 0, 0, 0}, {18, 23, 0, 0}, {26, 0, 0, 0}, {20, 25, 27, 0}, {26, 28, 0, 0}, {34, 27, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {28, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
int maze[TOTAL][4] ;

typedef struct Cell{
    int cell_number;
    int dir;
    int maze_distance;
} cell;

cell neighbour;

cell neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int neighbours_length;

cell available_neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int available_neighbours_length;

cell min_neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int min_neighbours_length;

int starting_cell = 1;
int starting_dir = 3;

int current_cell;
int current_dir;

int next_cell;
int next_dir;

int coordinates[2];
int temp_coordinates[2];

int commands[2*TOTAL];
int commands_length;

// int md[DIM][DIM] = {{4,3,2,2,3,4},
//       {3,2,1,1,2,3,},
//       {2,1,0,0,1,2},
//       {2,1,0,0,1,2},
//       {3,2,1,1,2,3},
//       {4,3,2,2,3,4}};

int md[DIM][DIM];

int rev_md[DIM][DIM];


int queue[1000];
int queue_start = 0;
int queue_end = 0;
int q_size=0;

int index_ = 0;
int path[1000];
int filtered_path[TOTAL];
int filtered_size = 0;

//###########################################



//encoder interupt functions
void IRAM_ATTR encoder1(){
  //Serial.println("INT1");
  if(digitalRead(en1b)==LOW){
    count1++;
    if(count1>2000){count1=0;}
    }
   else{
    count1--;
    if(count1<-2000){count1=0;}
    }
}

void IRAM_ATTR encoder2(){
//Serial.println("INT2");
  if(digitalRead(en2b)==LOW){
    count2--;
    if(count2<-2000){count2=0;}
    }
  else{
    count2++;
    if(count2>2000){count2=0;}
  }
}



void setup() {

  SerialBT.begin("ESP32_mouse");
  Serial.begin(9600);
  
  // Motor driver pins
  pinMode(m1a,OUTPUT);
  pinMode(m1b,OUTPUT);
  pinMode(m2a,OUTPUT);
  pinMode(m2b,OUTPUT);
  pinMode(13,INPUT);

  // pwm attach motors
  ledcSetup(PWMm1a, PWMFreq, PWMResolution);
  ledcAttachPin(m1a, PWMm1a);
  ledcSetup(PWMm1b, PWMFreq, PWMResolution);
  ledcAttachPin(m1b, PWMm1b);
  ledcSetup(PWMm2a, PWMFreq, PWMResolution);
  ledcAttachPin(m2a, PWMm2a);
  ledcSetup(PWMm2b, PWMFreq, PWMResolution);
  ledcAttachPin(m2b, PWMm2b);
  
  // SharpIR pins
  pinMode(rightIR,INPUT);
  pinMode(leftIR,INPUT);
  
  // buzzer
  pinMode(buzzPin,OUTPUT);

  // Encoders
  pinMode(en1a,INPUT);
  pinMode(en2a,INPUT);
  attachInterrupt(en1a,encoder1,RISING);
  pinMode(en1b,INPUT);
  attachInterrupt(en2a,encoder2,RISING);
  pinMode(en2b,INPUT);

  // MPU
  Wire.begin();
  byte status = mpu.begin();
  //Serial.print(F("MPU6050 status: "));
  //Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050
  

  // Calibrate MPU
  //Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  //Serial.println("Done!\n");

  // Initializations of tof
  initTOFs();
  SerialBT.println("init TOFs Done!\n");

  // Calibrate TOF
  calibTOFThresh() ;



  initialize_maze();
  fill_md(DESTINATION);

  calibIRThresh();
//  blinkLED(1);
//  delay(2000);
  
  mpu.update();
  globle_theta=mpu.getAngleZ();
}

void loop() {
//  alignTest();
//  IR_cell_run();
//  cellRunTest();
//  blinkLED(1);
//  adjustCell();
//  delay(4000);
//go_count(200);
//delay(2000);
//  IRtest();
//  double t = millis();
//  while((millis()-t)<5000){
//    delay(10);
//    if(!digitalRead(13)){
//      blinkLED(4);
//      startMouse();
//    }
//  }
//  blinkLED(3);
//goCellDevel2();
//delay(4000);
  Buzz(2);
  startMouse();
//    delay(1000000);
//  mpu.update();
//  SerialBT.println(mpu.getAngleZ());
//  turnTest();
//  cellRunTest();
//  alignTest();
//  tofTest();
//
//  adjustFrontDistance();
//  align(1);
//  delay(2000);
//  align(0);
//  delay(2000);
//  straightRunTest();
//IRtest();
//Serial.println(analogRead(14));
//Serial.println(analogRead(15));
//Serial.println(" ");
//Buzz(cellTone);
//delay(5000);
//Buzz(destinationTone);
//delay(5000);
}
