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
