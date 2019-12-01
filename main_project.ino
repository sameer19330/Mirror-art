#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include <SoftwareSerial.h>// import the serial library
SoftwareSerial cam_BT(12, 11);  // RX, TX
int ledpin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x42);
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  588 // this is the 'maximum' pulse length count (out of 4096)

byte curPos [36];   // stores 50 - 196 repr pulselen/3 of curr pos
byte tarPos [36];   // stores 50 - 196 repr pulselen/3 of target pos

// defines pins numbers
const int trigPin5 = 9;
const int echoPin5 = 10;
const int trigPin4 = 7;
const int echoPin4 = 8;
const int trigPin3 = 5;
const int echoPin3 = 6;
const int trigPin2 = 3;
const int echoPin2 = 4;
const int trigPin1 = 11;
const int echoPin1 = 12;
long duration;
int distance1, distance2, distance3, distance4, distance5;
int mypos=3;
int counter=0;

// input is row, col
// this table provides the servo number
const byte findNum[8][8] = {
  {0, 36, 36, 36, 36, 36, 36, 36},
  {1, 2, 36, 36, 36, 36, 36, 36},
  {3, 4, 5, 36, 36, 36, 36, 36},
  {6, 7, 8, 9, 36, 36, 36, 36},
  {10, 11, 12, 13, 14, 36, 36, 36},
  {15, 16, 17, 18, 19, 20, 36, 36},
  {21, 22, 23, 24, 25, 26, 27, 36},
  {28, 29, 30, 31, 32, 33, 34, 35}
};

const byte track[8][5] = {
{175,150,125,90,65},
{165,145,125,95,70},
{155,140,125,100,75},
{150,137,125,105,83},
{145,135,125,108,90},
{140,133,125,112,97},
{135,130,125,115,105},
{130,127,125,118,110}
};

const byte positions[5][36]={
{184,174,180,162,169,177,150,162,170,175,140,153,162,169,174,131,144,154,162,168,172,125,136,154,155,162,167,171,119,130,140,149,156,162,167,171},
{171,153,169,136,155,167,125,143,157,166,117,132,147,158,166,111,125,138,149,158,165,107,119,138,142,151,159,165,105,114,125,135,144,153,159,165},
{125,109,140,102,125,147,99,115,134,150,96,109,125,140,153,95,105,118,131,144,149,94,102,118,125,136,147,155,93,100,109,119,130,140,149,157},
{78,80,96,82,94,113,83,92,106,125,83,91,102,117,132,84,91,100,111,125,138,84,90,100,107,119,130,142,84,90,96,105,114,125,135,144},
{65,69,75,72,78,87,74,79,87,99,75,80,87,96,109,77,81,87,95,105,118,78,82,87,94,102,113,125,78,82,87,93,100,109,119,130}
};

const byte spiral[36] = {0,1,3,6,10,15,21,28,29,30,31,32,33,34,35,27,20,14,9,5,2,4,7,11,16,22,23,24,25,26,19,13,8,12,17,18};

void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin4, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin4, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin5, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin5, INPUT); // Sets the echoPin as an Input
  digitalWrite(13,LOW);

  pwm.begin();
  pwm2.begin();
  pwm3.begin();

  pwm.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  pwm3.setPWMFreq(60);
  delay(10);
  // set all servos to midpoint pos and intialize tables
  for (int myservo = 0; myservo < 36; myservo++) {
    if (myservo < 15)  pwm.setPWM(myservo, 0, 375);
    if (myservo > 14 && myservo < 28) pwm2.setPWM(myservo - 15, 0, 375);
    if (myservo > 27) pwm3.setPWM(myservo - 28, 0, 375);
    curPos[myservo] = 125;
    tarPos[myservo] = 125;
  }
cam_BT.begin(9600); //start the Bluetooth communication at 9600 baudrate
cam_BT.println("Ready to take commands");

Serial.begin(57600);

}



void loop() {


 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

  
  for (int count = 0; count < 5; count=count+2) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    for (int myservo  = 0; myservo < 36; myservo++) { 
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      setServo2(spiral[myservo], count);
      goToTargets(12, 2);
    }
 /*  for (int myservo  = 35; myservo > -1; myservo--) {
    if (count<4){
      setServo2(spiral[myservo], count+1);
     goToTargets(5, 3);
     } 
   }*/
  }
  for (int count = 3; count > 0; count=count-2) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    for (int myservo  = 0; myservo < 36; myservo++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      setServo2(spiral[myservo], count);
      goToTargets(12, 2);
    }
 /*  for (int myservo  = 35; myservo > -1; myservo--) {
    if (count>1){
      setServo2(spiral[myservo], count-1);
     goToTargets(5, 3);
     } 
   }*/
  }

  // interesting effects using the diagonals but with variable angles
  for (int count2 = 0; count2 < 2; count2++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    for (int count = 0; count < 8; count++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      rightDiag2(count + 1 , 14);
      goToTargets(20, 2 - count / 4);
    }
    delay(500);
    for (int count = 0; count < 8; count++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      leftDiag2(count + 1 , 15);
      goToTargets(20, 2 - count / 4);
    }
    delay(500);
  }
  for (int count2 = 0; count2 < 4; count2++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    for (int count = 0; count < 8; count++) {

 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      rightDiag2(count + 1 , 14);
    }
    goToTargets(15, 8);
    for (int count = 0; count < 8; count++) {
      leftDiag2(count + 1 , 15);
    }
    goToTargets(15, 8);
  }

  // now just using regular diagonls where all along diagonal are the same
  for (int count = 7; count > -1; count--) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    rightDiag(count + 1 , 4);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 7; count > -1; count--) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    rows(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 7; count > -1; count--) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    leftDiag(count + 1 , 0);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 7; count > -1; count--) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    rows(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 0; count < 8; count++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    leftDiag(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }
  delay(300);
  for (int count = 0; count < 8; count++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    if (count % 2) rightDiag(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }
  for (int myservo  = 0; myservo < 36; myservo++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    setTarget(myservo, 4);
  }
  // Wave routine
  for (int count = 0; count < 120; count++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    for (int myservo  = 0; myservo < 36; myservo++) {
      
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

      if (curPos[myservo] < 51 && tarPos[myservo] == 50) tarPos[myservo] = 196;
      if (curPos[myservo] > 195 && tarPos[myservo] == 196) tarPos[myservo] = 50;
    }
    goToTargets(0, 0);
  }
  for (int myservo = 0; myservo < 36; myservo++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    setServo2(myservo, 2);
  }
  goToTargets(36, 0);

  //Setup for first wave
  for (int servoNum = 0; servoNum < 36; servoNum++) {
    
 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 

    tarPos[servoNum] = 50 + servoNum * 2;
  }
  goToTargets(36, 0);
  for (int myservo  = 0; myservo < 36; myservo++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setTarget(myservo, 4);
  }
  // Wave routine
  for (int count = 0; count < 120; count++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    for (int myservo  = 0; myservo < 36; myservo++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
      if (curPos[myservo] < 51 && tarPos[myservo] == 50) tarPos[myservo] = 196;
      if (curPos[myservo] > 195 && tarPos[myservo] == 196) tarPos[myservo] = 50;
    }
    goToTargets(0, 0);
  }
  // set all to pos 0
  for (int myservo = 0; myservo < 36; myservo++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(myservo, 0);
  }
  goToTargets(36, 0);
  for (int count = 0; count < 8; count++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) rightDiag(count + 1 , 3);
    else rightDiag(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }
  for (int myservo = 0; myservo < 36; myservo++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(myservo, 4);
  }
  goToTargets(36, 0);
  for (int count = 0; count < 8; count++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) leftDiag(count + 1 , 1);
    else leftDiag(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }

  for (int myservo = 0; myservo < 36; myservo++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(myservo, 0);
  }
  goToTargets(36, 0);
  for (int count = 0; count < 8; count++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) rows(count + 1 , 1);
    else rows(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }

  for (int count = 0; count < 8; count++) {
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) rows(count + 1 , 3);
    else rows(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }

  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) rows(count + 1 , 1);
    else rows(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }

  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2) rows(count + 1 , 3);
    else rows(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }
  piramid ();
  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    rightDiag(count + 1 , 3);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    leftDiag(count + 1 , 1);
    goToTargets(20, 2 - count / 4);
  }
  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    rows(count + 1 , 0);
    goToTargets(20, 2 - count / 4);
  }
  // set up for the Ts wave
  for (int count = 0; count < 8; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    if (count % 2 == 0)rows(count + 1 , 4);
  }
  goToTargets(15, 3);
  //wave routine
  for (int count = 0; count < 240; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    for (int myservo  = 0; myservo < 36; myservo++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
      if (curPos[myservo] < 51 && tarPos[myservo] == 50) tarPos[myservo] = 196;
      if (curPos[myservo] > 195 && tarPos[myservo] == 196) tarPos[myservo] = 50;
    }
    goToTargets(0, 0);
  }
  setForWave();  // set up for second wave
  delay(300);
  //wave routine
  for (int count = 0; count < 240; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    for (int myservo  = 0; myservo < 36; myservo++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
      if (curPos[myservo] < 51 && tarPos[myservo] == 50) tarPos[myservo] = 196;
      if (curPos[myservo] > 195 && tarPos[myservo] == 196) tarPos[myservo] = 50;
    }
    goToTargets(0, 0);
  }
  setAllZero();
  delay(300);

  triangles();
  triangles();

  for (int count = 0; count < 36; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(count, 3);
  }
  goToTargets(21, 15);
  for (int count = 0; count < 36; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(count, 1);
  }
  goToTargets(36, 0);
  for (int count = 0; count < 36; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(count, 3);
  }
  goToTargets(36, 0);
  for (int count = 0; count < 36; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    setServo2(count, 1);
  }
  goToTargets(20, 0);


  
 BluetoothData=0;
  }

// this is the primary routine for moving the servos, movint them from there current position toward their target postions
void goToTargets(int reps, int mydelay) {
  for (int count = 0; count < 5 + 5 * reps; count++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    for (int myservo  = 0; myservo < 36; myservo++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
      if (curPos[myservo] > tarPos[myservo]) curPos[myservo]--;
      if (curPos[myservo] < tarPos[myservo]) curPos[myservo]++;
      if (curPos[myservo] != tarPos[myservo]) {
        if (myservo < 15)  pwm.setPWM(myservo, 0, 3 * curPos[myservo]);
        if (myservo > 14 && myservo < 28) pwm2.setPWM(myservo - 15, 0, 3 * curPos[myservo]);
        if (myservo > 27) pwm3.setPWM(myservo - 28, 0, 3 * curPos[myservo]);
      }
    }
    delay(mydelay);
  }
  // this is where we check if hand is over the center distance sensor; if so we go into tracker mode
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  // Sets the trigPin3 on HIGH state for 10 micro seconds
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin3, HIGH, 2500);
  if (duration > 0) {
    counter = 0;
    
  }
}
// this sets the target of a servo to one of five preset positions
void setTarget(int myservo, int pos) {
  tarPos[myservo] = pos * 38 + 50;
  if (tarPos[myservo] > 196)tarPos[myservo] = 196;
}
// this sets all servos to their minimum positions
void setAllZero() {
  for (int myservo  = 0; myservo < 36; myservo++) {
    tarPos[myservo] = 50;
  }
  for (int count = 0; count < 30; count++) {
    goToTargets(0, 0);
  }
}
// set target position of servo in rows 1-8 
// and coluumns 1-8, addressing them by row and column
// to one of the 5 preset positions
void setServo(int row, int col,  int myPos) {
  int servoNum = findNum[row - 1][col - 1];
  if (myPos == 0) tarPos[servoNum] = 50;
  else if (myPos == 4) tarPos[servoNum] = 196;
  else if (myPos == 2) tarPos[servoNum] = 125;
  else if (myPos == 1) tarPos[servoNum] = 93;
  else if (myPos == 3) tarPos[servoNum] = 153;
}
// set target position of servo by servo #
// this is a duplicate of setTarget
void setServo2(int servoNum, int myPos) {
  if (myPos == 0) tarPos[servoNum] = 50;
  else if (myPos == 4) tarPos[servoNum] = 196;
  else if (myPos == 2) tarPos[servoNum] = 125;
  else if (myPos == 1) tarPos[servoNum] = 93;
  else if (myPos == 3) tarPos[servoNum] = 153;
}

// set target position of servo in rows 1-8 and coluumns 1-8
// special version used by rightdiag2 and leftdiag2
void setServo3(int row, int col,  int num, int mult2) {
  int servoNum = findNum[row - 1][col - 1];
  if (mult2 % 2 == 0)tarPos[servoNum] = 180 - num * mult2;
  else tarPos[servoNum] = 60 + num * mult2;
}

// sets servo to point to target at row 0 and targetX
// this uses arctan to point servo at a preset position above the board
void setServo4(int row, int col, int targetX) {
  int servoNum = findNum[row - 1][col - 1];
  int trueCol = col + 8 - row;
  double angle = 1.57 + atan2(targetX - trueCol, row);
  tarPos[servoNum] = int(188 - 138 * angle / 3.14);
}

//setup for wave; not very efficient way to do it
void setForWave() {
  int mult = 50;
  tarPos[0] = 50 + 0;
  for (int myservo  = 1; myservo < 3; myservo++) {  // row 1
     if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 1) * mult / 3;
  }
  for (int myservo  = 3; myservo < 6; myservo++) {  // row 2 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 3) * mult / 3;
  }
  for (int myservo  = 6; myservo < 10; myservo++) {  // row 3 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 6) * mult / 3;
  }
  for (int myservo  = 10; myservo < 15; myservo++) {  // row 4 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 10) * mult / 3;
  }
  for (int myservo  = 15; myservo < 21; myservo++) {  // row 5 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 15) * mult / 3;
  }
  for (int myservo  = 21; myservo < 28; myservo++) {  // row 6 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 21) * mult / 3;
  }
  for (int myservo  = 28; myservo < 36; myservo++) { // row 7 if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 50 + (myservo - 28) * mult / 3;
  }
  goToTargets(36, 0);
  for (int myservo  = 0; myservo < 36; myservo++) { if (cam_BT.available()) //Read whats coming in through Bluetooth
 {
  BluetoothData=cam_BT.read();
  Serial.print("Incoming from BT:");
  Serial.println(BluetoothData);
 }

 if (BluetoothData==50||BluetoothData==49||BluetoothData==51||BluetoothData==52) //Turn Left
 {
 delay(1000);
 
 BluetoothData=0;
 }
 
    tarPos[myservo] = 196;
  }
}
//  the triagle patterns
void triangles () {
  setServo2(0, 0);
  setServo2(1, 0);
  setServo2(2, 0);
  setServo2(3, 0);
  setServo2(4, 0);
  setServo2(5, 0);
  setServo2(6, 2);
  setServo2(7, 2);
  setServo2(8, 2);
  setServo2(9, 2);
  setServo2(10, 0);
  setServo2(11, 2);
  setServo2(12, 2);
  setServo2(13, 2);
  setServo2(14, 0);
  setServo2(15, 0);
  setServo2(16, 0);
  setServo2(17, 2);
  setServo2(18, 2);
  setServo2(19, 0);
  setServo2(20, 0);
  setServo2(21, 0);
  setServo2(22, 0);
  setServo2(23, 0);
  setServo2(24, 2);
  setServo2(25, 0);
  setServo2(26, 0);
  setServo2(27, 0);
  for (int count = 28; count < 36; count++) {
    setServo2(count, 0);
  }
  goToTargets(15, 0);
  delay(500);
  setServo2(0, 2);
  setServo2(1, 2);
  setServo2(2, 2);
  setServo2(3, 2);
  setServo2(4, 2);
  setServo2(5, 2);
  setServo2(6, 0);
  setServo2(7, 0);
  setServo2(8, 0);
  setServo2(9, 0);
  setServo2(10, 2);
  setServo2(11, 0);
  setServo2(12, 0);
  setServo2(13, 0);
  setServo2(14, 2);
  setServo2(15, 2);
  setServo2(16, 2);
  setServo2(17, 0);
  setServo2(18, 0);
  setServo2(19, 2);
  setServo2(20, 2);
  setServo2(21, 2);
  setServo2(22, 2);
  setServo2(23, 2);
  setServo2(24, 0);
  setServo2(25, 2);
  setServo2(26, 2);
  setServo2(27, 2);
  for (int count = 28; count < 36; count++) {
    setServo2(count, 2);
  }
  goToTargets(15, 0);
  delay(500);
}

// way to progressively do something along right diagonal
void rightDiag(int num, int pos) {
  if (num == 1) {
    setServo(8, 8, pos);
  }
  if (num == 2) {
    setServo(7, 7, pos);
    setServo(8, 7, pos);
  }
  if (num == 3) {
    setServo(6, 6, pos);
    setServo(7, 6, pos);
    setServo(8, 6, pos);
  }
  if (num == 4) {
    setServo(5, 5, pos);
    setServo(6, 5, pos);
    setServo(7, 5, pos);
    setServo(8, 5, pos);
  }
  if (num == 5) {
    setServo(4, 4, pos);
    setServo(5, 4, pos);
    setServo(6, 4, pos);
    setServo(7, 4, pos);
    setServo(8, 4, pos);
  }
  if (num == 6) {
    setServo(3, 3, pos);
    setServo(4, 3, pos);
    setServo(5, 3, pos);
    setServo(6, 3, pos);
    setServo(7, 3, pos);
    setServo(8, 3, pos);
  }
  if (num == 7) {
    setServo(2, 2, pos);
    setServo(3, 2, pos);
    setServo(4, 2, pos);
    setServo(5, 2, pos);
    setServo(6, 2, pos);
    setServo(7, 2, pos);
    setServo(8, 2, pos);
  }
  if (num == 8) {
    setServo(1, 1, pos);
    setServo(2, 1, pos);
    setServo(3, 1, pos);
    setServo(4, 1, pos);
    setServo(5, 1, pos);
    setServo(6, 1, pos);
    setServo(7, 1, pos);
    setServo(8, 1, pos);
  }
}

// way to progressively do something along the left diagonal
void leftDiag(int num, int pos) {
  if (num == 1) {
    setServo(8, 1, pos);
  }
  if (num == 2) {
    setServo(7, 1, pos);
    setServo(8, 2, pos);
  }
  if (num == 3) {
    setServo(6, 1, pos);
    setServo(7, 2, pos);
    setServo(8, 3, pos);
  }
  if (num == 4) {
    setServo(5, 1, pos);
    setServo(6, 2, pos);
    setServo(7, 3, pos);
    setServo(8, 4, pos);
  }
  if (num == 5) {
    setServo(4, 1, pos);
    setServo(5, 2, pos);
    setServo(6, 3, pos);
    setServo(7, 4, pos);
    setServo(8, 5, pos);
  }
  if (num == 6) {
    setServo(3, 1, pos);
    setServo(4, 2, pos);
    setServo(5, 3, pos);
    setServo(6, 4, pos);
    setServo(7, 5, pos);
    setServo(8, 6, pos);
  }
  if (num == 7) {
    setServo(2, 1, pos);
    setServo(3, 2, pos);
    setServo(4, 3, pos);
    setServo(5, 4, pos);
    setServo(6, 5, pos);
    setServo(7, 6, pos);
    setServo(8, 7, pos);
  }
  if (num == 8) {
    setServo(1, 1, pos);
    setServo(2, 2, pos);
    setServo(3, 3, pos);
    setServo(4, 4, pos);
    setServo(5, 5, pos);
    setServo(6, 6, pos);
    setServo(7, 7, pos);
    setServo(8, 8, pos);
  }
}
// like the diagonals except progressively through the rows
void rows(int num, int pos) {
  if (num == 1) {
    setServo(1, 1, pos);
  }
  if (num == 2) {
    setServo(2, 1, pos);
    setServo(2, 2, pos);
  }
  if (num == 3) {
    setServo(3, 1, pos);
    setServo(3, 2, pos);
    setServo(3, 3, pos);
  }
  if (num == 4) {
    setServo(4, 1, pos);
    setServo(4, 2, pos);
    setServo(4, 3, pos);
    setServo(4, 4, pos);
  }
  if (num == 5) {
    setServo(5, 1, pos);
    setServo(5, 2, pos);
    setServo(5, 3, pos);
    setServo(5, 4, pos);
    setServo(5, 5, pos);
  }
  if (num == 6) {
    setServo(6, 1, pos);
    setServo(6, 2, pos);
    setServo(6, 3, pos);
    setServo(6, 4, pos);
    setServo(6, 5, pos);
    setServo(6, 6, pos);
  }
  if (num == 7) {
    setServo(7, 1, pos);
    setServo(7, 2, pos);
    setServo(7, 3, pos);
    setServo(7, 4, pos);
    setServo(7, 5, pos);
    setServo(7, 6, pos);
    setServo(7, 7, pos);
  }
  if (num == 8) {
    setServo(8, 1, pos);
    setServo(8, 2, pos);
    setServo(8, 3, pos);
    setServo(8, 4, pos);
    setServo(8, 5, pos);
    setServo(8, 6, pos);
    setServo(8, 7, pos);
    setServo(8, 8, pos);
  }
}
//  a particular sequence of patterns
void piramid () {
  setServo(2, 1, 1);
  setServo(2, 2, 3);
  goToTargets(10, 3);
  setServo(3, 1, 1);
  setServo(3, 3, 3);
  goToTargets(10, 3);
  setServo(4, 1, 1);
  setServo(4, 3, 3);
  setServo(4, 2, 1);
  setServo(4, 4, 3);
  goToTargets(10, 3);
  setServo(5, 1, 1);
  setServo(5, 4, 3);
  setServo(5, 2, 1);
  setServo(5, 5, 3);
  goToTargets(10, 3);
  setServo(6, 1, 1);
  setServo(6, 2, 1);
  setServo(6, 3, 1);
  setServo(6, 4, 3);
  setServo(6, 5, 3);
  setServo(6, 6, 3);
  goToTargets(10, 3);
  setServo(7, 1, 1);
  setServo(7, 2, 1);
  setServo(7, 3, 1);
  setServo(7, 5, 3);
  setServo(7, 6, 3);
  setServo(7, 7, 3);
  goToTargets(10, 3);
  setServo(8, 1, 1);
  setServo(8, 2, 1);
  setServo(8, 3, 1);
  setServo(8, 4, 1);
  setServo(8, 5, 3);
  setServo(8, 6, 3);
  setServo(8, 7, 3);
  setServo(8, 8, 3);
  goToTargets(10, 3);
  delay (500);
  setServo(1, 1, 1);
  setServo(3, 2, 1);
  setServo(5, 3, 1);
  setServo(7, 4, 1);
  goToTargets(20, 3);
  setServo(1, 1, 3);
  setServo(3, 2, 3);
  setServo(5, 3, 3);
  setServo(7, 4, 3);
  goToTargets(20, 3);
  setServo(1, 1, 1);
  setServo(3, 2, 1);
  setServo(5, 3, 1);
  setServo(7, 4, 1);
  goToTargets(20, 3);
  setServo(4, 3, 1);
  setServo(5, 4, 1);
  setServo(6, 4, 1);
  setServo(7, 5, 1);
  setServo(8, 5, 1);
  goToTargets(20, 3);
  setServo(6, 5, 1);
  setServo(7, 6, 1);
  setServo(8, 6, 1);
  setServo(8, 7, 1);
  goToTargets(20, 3);
  setServo(2, 2, 1);
  setServo(3, 3, 1);
  setServo(4, 4, 1);
  setServo(5, 5, 1);
  setServo(6, 6, 1);
  setServo(7, 7, 1);
  setServo(8, 8, 1);
  goToTargets(20, 3);
}

void rightDiag2(int num, int mult) {
  if (num == 1) {
    setServo3(8, 8, 1, mult);
  }
  if (num == 2) {
    setServo3(7, 7, 1, mult);
    setServo3(8, 7, 2, mult);
  }
  if (num == 3) {
    setServo3(6, 6, 1, mult);
    setServo3(7, 6, 2, mult);
    setServo3(8, 6, 3, mult);
  }
  if (num == 4) {
    setServo3(5, 5, 1, mult);
    setServo3(6, 5, 2, mult);
    setServo3(7, 5, 3, mult);
    setServo3(8, 5, 4, mult);
  }
  if (num == 5) {
    setServo3(4, 4, 1, mult);
    setServo3(5, 4, 2, mult);
    setServo3(6, 4, 3, mult);
    setServo3(7, 4, 4, mult);
    setServo3(8, 4, 5, mult);
  }
  if (num == 6) {
    setServo3(3, 3, 1, mult);
    setServo3(4, 3, 2, mult);
    setServo3(5, 3, 3, mult);
    setServo3(6, 3, 4, mult);
    setServo3(7, 3, 5, mult);
    setServo3(8, 3, 6, mult);
  }
  if (num == 7) {
    setServo3(2, 2, 1, mult);
    setServo3(3, 2, 2, mult);
    setServo3(4, 2, 3, mult);
    setServo3(5, 2, 4, mult);
    setServo3(6, 2, 5, mult);
    setServo3(7, 2, 6, mult);
    setServo3(8, 2, 7, mult);
  }
  if (num == 8) {
    setServo3(1, 1, 1, mult);
    setServo3(2, 1, 2, mult);
    setServo3(3, 1, 3, mult);
    setServo3(4, 1, 4, mult);
    setServo3(5, 1, 5, mult);
    setServo3(6, 1, 6, mult);
    setServo3(7, 1, 7, mult);
    setServo3(8, 1, 8, mult);
  }
}
void leftDiag2(int num, int mult) {
  if (num == 1) {
    setServo3(8, 1, 1, mult);
  }
  if (num == 2) {
    setServo3(7, 1, 1, mult);
    setServo3(8, 2, 2, mult);
  }
  if (num == 3) {
    setServo3(6, 1, 1, mult);
    setServo3(7, 2, 2, mult);
    setServo3(8, 3, 3, mult);
  }
  if (num == 4) {
    setServo3(5, 1, 1, mult);
    setServo3(6, 2, 2, mult);
    setServo3(7, 3, 3, mult);
    setServo3(8, 4, 4, mult);
  }
  if (num == 5) {
    setServo3(4, 1, 1, mult);
    setServo3(5, 2, 2, mult);
    setServo3(6, 3, 3, mult);
    setServo3(7, 4, 4, mult);
    setServo3(8, 5, 5, mult);
  }
  if (num == 6) {
    setServo3(3, 1, 1, mult);
    setServo3(4, 2, 2, mult);
    setServo3(5, 3, 3, mult);
    setServo3(6, 4, 4, mult);
    setServo3(7, 5, 5, mult);
    setServo3(8, 6, 6, mult);
  }
  if (num == 7) {
    setServo3(2, 1, 1, mult);
    setServo3(3, 2, 2, mult);
    setServo3(4, 3, 3, mult);
    setServo3(5, 4, 4, mult);
    setServo3(6, 5, 5, mult);
    setServo3(7, 6, 6, mult);
    setServo3(8, 7, 7, mult);
  }
  if (num == 8) {
    setServo3(1, 1, 1, mult);
    setServo3(2, 2, 2, mult);
    setServo3(3, 3, 3, mult);
    setServo3(4, 4, 4, mult);
    setServo3(5, 5, 5, mult);
    setServo3(6, 6, 6, mult);
    setServo3(7, 7, 7, mult);
    setServo3(8, 8, 8, mult);
  }
 BluetoothData=0;
}
