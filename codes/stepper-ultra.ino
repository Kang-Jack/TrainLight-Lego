#include <AccelStepper.h>
#include "NewPing.h"
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 1

#define ledPin1 6
#define ledPin2 7

#define MAX_DISTANCE 400

//distance CM
float duration, distance;
int iterations = 5;
int maxDisplayDistance =30;
int minDistance =2;
int triggerDistance = 10;
bool isTrainComing =false;

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
//Define pins ultrasonic(trig,echo)
NewPing  sonar(A0,A1,MAX_DISTANCE);

int steps1 = 0;
int FWSteps = 1100;
int BWSteps = -1100;
int stepperSpeed = 200;
boolean turn1 = false; 
void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT); 
  pinMode(ledPin2, OUTPUT); 
  stepper1.setMaxSpeed(2000.0);
  stepper1.move(1);  // I found this necessary
  stepper1.setSpeed(stepperSpeed);

}//--(end setup )---

void loop() {
   /*delay(200);
  if (isTrainComing())
    Serial.print("Traing is coming");
  else
    Serial.print("Traing is not coming");
    */
  if (isTrainComing == false && steps1==0){
    isTrainComing = isComing();
    delay(200);
  }
  if (isTrainComing == true || steps1 != 0) {  
      if(steps1==0 && turn1==false){
        blinkLights();
      }      
      stepTurn(); 
   }

 }
bool isComing()
{
  duration = sonar.ping_median(iterations);
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance = (duration / 2) * 0.0343;
  if (distance > maxDisplayDistance)
      distance = maxDisplayDistance;
  if (distance < minDistance)
      distance = maxDisplayDistance;
  if (distance >triggerDistance)
    return false;
  return true;
  /*Serial.print("Object found at: ");
  Serial.print(distance);
  Serial.println("cm");
  */
 } 
void blinkLights(){
  for (int i=0 ;i<=20;i++)
  {
    digitalWrite(ledPin1, HIGH);    // HIGH,LOW
    digitalWrite(ledPin2, LOW);    // HIGH,LOW
    delay(50);
    digitalWrite(ledPin1, LOW);    // HIGH,LOW
    digitalWrite(ledPin2, HIGH); 
    delay(50);
  }
  if (turn1==false){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  } 
  else{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
}
 void restStep(){
    stepper1.move(20000);
    stepper1.setSpeed(stepperSpeed);
    stepper1.runSpeedToPosition();
  }
 void stepTurn(){
   if (steps1 == 0) {
     int target = 0;
     if (turn1 == true) {
        delay (1000);
        blinkLights();
        target = FWSteps;
        isTrainComing = false;
      }
      else {
        target = BWSteps;
      }
      stepper1.move(target);
      stepper1.setSpeed(stepperSpeed);
      turn1 = !turn1;
   }
    steps1 = stepper1.distanceToGo();
    stepper1.runSpeedToPosition();
  }



