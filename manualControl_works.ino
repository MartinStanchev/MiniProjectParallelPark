#include <Smartcar.h>


Car car;

SR04 frontSonar;
SR04 sideSonar;
SR04 backSonar;
GP2Y0A21 backIR; 
Gyroscope gyro;


const int fSpeed = 25; //70% of the full speed forward
const int bSpeed = -25; //70% of the full speed backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right

Odometer encoderLeft;
Servo myservo;

const int frontTrigPin = 6; 
const int frontEchoPin = 7;
const int sideTrigPin = A11;
const int sideEchoPin = A12;
const int backTrigPin = A9;
const int backEchoPin = A10;

const int backIrPin = A0;

const int encoderLeftPin = 2;


const int SERVO_PIN = 50;

int spotSize;
int backDistanceInCm;
int frontDistanceInCm;
int sideDistanceInCm;
int irDistanceInCm;  

int maxBackDistance = 20;
int maxFrontDistance = 10; 
int maxSideDistance = 10;
int maxIrbackDistance = 13; 

int pos = 0; // variable to store the servo position



void setup() {
  Serial3.begin(9600);
  car.begin(); //initialize the car using the encoders and the gyro
  gyro.attach();
  gyro.begin();   
  sideSonar.attach(sideTrigPin, sideEchoPin);
  backSonar.attach(backTrigPin, backEchoPin);
  frontSonar.attach(frontTrigPin,frontEchoPin);
  encoderLeft.attach(encoderLeftPin);
  myservo.attach(SERVO_PIN);
  backIR.attach(backIrPin);
  car.begin(encoderLeft);
  car.begin(gyro);
  
}

void loop() {
  handleInput();
}

void handleInput() { //handle serial input if there is any
  if (Serial3.available()) {
    char input = Serial3.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'l': //rotate counter-clockwise going forward
        car.setSpeed(fSpeed);
        car.setAngle(lDegrees);
        break;
      case 'r': //turn clock-wise
        car.setSpeed(fSpeed);
        car.setAngle(rDegrees);
        break;
      case 's': //go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        findSpot();
        break;
      case 'p': //go back
      parkInSpot();
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

void findSpot(){
  const int ENOUGH_SPACE = 50;
  int spotStart,spotEnd,rightDistance,totalLength;
  
  while(car.getSpeed()!= 0) {
    
     rightDistance = sideSonar.getMedianDistance();
     
    if(rightDistance == 0 || rightDistance > 30){ 
      
       encoderLeft.begin();
       Serial3.println(" WOW let me check this Spot!");
       spotStart = encoderLeft.getDistance();

       while(rightDistance == 0 || rightDistance > 30) {
           rightDistance = sideSonar.getMedianDistance();
          Serial3.println(" Still Checking The SPOT ");
      }
      
      spotEnd = encoderLeft.getDistance();
       totalLength =  spotEnd - spotStart;
       Serial3.println(" Spot length is :");
       Serial3.println(totalLength);
       
       if(totalLength > ENOUGH_SPACE) {
        car.setSpeed(0);
        Serial3.println("Stop waiting to park");
        spotSize = totalLength;
        sideDistanceInCm = sideSonar.getDistance();
        break;
         
      }
    }
    else {
      car.setSpeed(fSpeed);
      car.setAngle(0);
    }
  }

 
}

void driveBackward(){
  
  irDistanceInCm = backIR.getDistance();
  
  Serial3.println("Distance of IR sensor is "); 
  Serial3.println(irDistanceInCm); 
  
  if(irDistanceInCm > maxIrbackDistance || irDistanceInCm == 0){
    
    car.setSpeed(bSpeed);
    car.setAngle(0);

    
    while(car.getSpeed()< 0){
      irDistanceInCm = backIR.getDistance();
      Serial3.println(irDistanceInCm);
      if(irDistanceInCm < maxIrbackDistance  && irDistanceInCm !=0){
        car.setSpeed(0); 
        break;
      }
      
    }
    
  }
  else {
    car.setSpeed(0);
    Serial3.println("I can NOT Man, I will Crash! ");
  }
  
 
 }
 
void parkInSpot(){  //need delay 1000 in many places or else it will not turn as it should
  
  int rotateDegree;   
  gyro.update();
  Serial3.println(gyro.getAngularDisplacement());
  
  if(sideDistanceInCm > 15){
     rotateDegree = -45;
     Serial3.println("side distance is ");
     Serial3.println(sideDistanceInCm);
  rotateOnSpot(rotateDegree);
  }
  else{
    rotateDegree = -30;
    Serial3.println("side distance is ");
     Serial3.println(sideDistanceInCm);
    rotateOnSpot(rotateDegree);
  }
  
  gyro.update();
  Serial3.println(gyro.getAngularDisplacement());
  
  delay(1000);
  
 driveBackward();
  
//  sqrt(pow((spotSize/3),2)+pow((sideDistanceInCm),2)); //weird, cuz the size of the spot comes out as very long

  delay(3000);

  car.go(-3);
  delay(500);
  
  rotateOnSpot(-(rotateDegree));

  
}

void rotateOnSpot(int targetDegrees) {
  targetDegrees %= 360; 
  if (!targetDegrees){
     return; 
  }
  if (targetDegrees > 0) { 
    car.setMotorSpeed(fSpeed, -fSpeed); 
  } else { 
    car.setMotorSpeed(-fSpeed, fSpeed); 
  }
  
  unsigned int initialHeading = gyro.getAngularDisplacement(); 
  int degreesTurnedSoFar = 0; 
  
  while (abs(degreesTurnedSoFar) < abs(targetDegrees)) { 
    gyro.update();
    int currentHeading = gyro.getAngularDisplacement(); 
    
    if ((targetDegrees < 0) && (currentHeading > initialHeading)) { 
      currentHeading -= 360; 
    } else if ((targetDegrees > 0) && (currentHeading < initialHeading)) {
      currentHeading += 360;
    }
    
    degreesTurnedSoFar = initialHeading - currentHeading;   
  }
 car.setSpeed(0);
}
