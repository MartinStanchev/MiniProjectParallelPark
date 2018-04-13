#include <Smartcar.h>

Car car;

const int forwardSpeed = 50;                   //Speed forward
const int backwardSpeed = -50;                  //Speed backward
const int leftDegrees = -65;               //degrees to turn left
const int rightDegrees = 65;                //degrees to turn right

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

////// methods for moving the car //////
void driveForward(){
  car.setSpeed(forwardSpeed);
  car.setAngle(0);
}
void driveBackward(){
  car.setSpeed(backwardSpeed);
  car.setAngle(0);
}
void driveLeft(){
   car.setSpeed(forwardSpeed);
   car.setAngle(leftDegrees);
}
void driveRight(){
  car.setSpeed(forwardSpeed);
  car.setAngle(rightDegrees);
}
void stopCar(){
  car.setSpeed(0);
}

void parkInSpot(){ //need delay 1000 in many places or else it will not turn as it should
  rotateOnSpot(-35);
  Serial3.print("After rotation");
  delay(200);
  getBackDistance();

  int lengthToGoBack = (spotSize / 3)+5; //weird, cuz the size of the spot comes out as very long
  
  if(lengthToGoBack < 5 || lengthToGoBack > 30){
    delay(200);
    Serial.print(lengthToGoBack);
    delay(200);
  }
  else{
    car.go(-lengthToGoBack); //car.go(-25);
    while(car.getSpeed() > 0) {  
    }
    stopCar();
    delay(1000);
    rotateOnSpot(35);
    
    //int turn = gyro.getAngularDisplacement();
   /* if(turn < 0){ //Idk if this if else is correct
      rotateOnSpot(-(turn));
    }else if(turn > 0){
      rotateOnSpot(-turn);
    } */ 
    delay(1000);
    //findMiddle();
    //while(car.getSpeed() > 0) {  
    //}
  }
}

