 #include <Smartcar.h>


Car car;

SR04 front;
SR04 back;

GP2Y0A21 sideFrontIR; 

Odometer encoderLeft(570), encoderRight(570);



const int TRIGGER_PIN_F = 6; //D6
const int ECHO_PIN_F = 7;

const int TRIGGER_PIN_B = A9; //D
const int ECHO_PIN_B = A10;
const int SIDE_FRONT_PIN = A0;




const int fSpeed = 40; //70% of the full speed forward
const int bSpeed = -40; //70% of the full speed backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right
const int parkSpeed = 30;


void setup() {
  Serial3.begin(9600);
  car.begin(); //initialize the car using the encoders and the gyro
  front.attach(TRIGGER_PIN_F, ECHO_PIN_F); 
  back.attach(TRIGGER_PIN_B, ECHO_PIN_B); 
//  right.attach(TRIGGER_PIN_R, ECHO_PIN_R); 
  encoderRight.attach(2, 12, HIGH);
  encoderLeft.attach(3, 13, LOW);
  sideFrontIR.attach(SIDE_FRONT_PIN);
}

void loop() {
  handleInput();
//  int frontDistance = front.getDistance();
//  int backDistance = back.getDistance();
//
//  Serial.println(backDistance);
//   
//  if(car.getSpeed() == fSpeed && (frontDistance < 25 && frontDistance >0)){
//    car.stop();
//    Serial.println("an obstacle infront of the car !");
//  }
//  if(car.getSpeed() == bSpeed && (backDistance < 25 && backDistance>0)){
//    car.stop();
//    Serial.println("an obstacle behind the car !");
//  }
  
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
      case 'f': //go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
      case 'b': //go back
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
      case 's':
        car.setSpeed(parkSpeed);
        car.setAngle(0);
        findSpot();
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}


void findSpot(){

  
   const int ENOUGH_SPACE = 15;
   int spotStart,spotEnd,rightDistance,totalLength;
   
   
   
   while(car.getSpeed()!= 0) {
    
     rightDistance = sideFrontIR.getDistance();
     
    if(rightDistance == 0 || rightDistance > 25){ 
      
       encoderLeft.begin();
       spotStart = encoderLeft.getDistance();

       while(rightDistance == 0 || rightDistance > 25) {
           rightDistance = sideFrontIR.getDistance();
          Serial3.println(" checking the spot");
      }
      
      spotEnd = encoderLeft.getDistance();
       totalLength =  spotEnd - spotStart;
       Serial3.println(" Spot length is :")
       Serial3.println(totalLength);
       
       if(totalLength > ENOUGH_SPACE) {
        car.stop();
        Serial3.println("the car has Stopped");
        break;
      }
      
    }
    else{
      car.setSpeed(parkSpeed);
    }
    
    
   }
   
 
}

