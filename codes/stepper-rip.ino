#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 1

#define ledPin1 6
#define ledPin2 7

int ripPin = 5;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int ripVal = 0;                    // variable for reading the pin status

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
int steps1 = 0;
int FWSteps = 1100;
int BWSteps = -1100;
int stepperSpeed = 200;
boolean turn1 = false; 
void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT); 
  pinMode(ledPin2, OUTPUT);
  pinMode(ripPin, INPUT);  
  stepper1.setMaxSpeed(2000.0);
  stepper1.move(1);  // I found this necessary
  stepper1.setSpeed(stepperSpeed);

}//--(end setup )---

void loop() {
  if (ripVal == LOW && steps1==0){
    ripVal = digitalRead(ripPin);  // read input value
    //turn1 = true;
  }
  if (ripVal == HIGH || steps1 != 0) {  
      if(steps1==0 && turn1==false){
        blinkLights();
      }      
      stepTurn(); 
   }

 }
 
void blinkLights(){
  for (int i=0 ;i<=30;i++)
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
        delay (3000);
        blinkLights();
        target = FWSteps;
        ripVal = LOW;
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



