//attributes//
const int forwardSpeed = 30;                   //Speed forward
const int backwardSpeed = -30;                 //Speed backward
const int leftDegrees = -65;                   //degrees to turn left
const int rightDegrees = 65;                   //degrees to turn right

//driving methods//
void driveForward(){
  car.setSpeed(fordwardSpeed);
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

//GETTERS AND SETTERS if you want (IDK IF THEY WORK THE SAME WAY AS IN JAVA THO, SO IM NOT SURE IF THIS WORKS//
//Speed//
int getForwardSpeed(){
  return forwardSpeed;
}
int getBackwardSpeed(){
  return backwardSpeed;
}
void setForwardSpeed(int speed){
  forwardSpeed = speed;
}
void setBackwardSpeed(int speed){
  backwardSpeed = speed;
}

//degrees (used for turning left or right//
int getLeftDegrees(){
  return leftDegrees;
}
int getRightDegrees(){
  return rightDegrees;
}
void setLeftDegrees(int degrees){
  leftDegrees = degrees;
}
void setRightDegrees(int degrees){
  rightDegrees = degrees;
}
