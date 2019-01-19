#include <Servo.h> 

//#include "NewPing.h"

// Motor pin definitions
#define servoPin  4     // IN1 on the ULN2003 driver 1
Servo myservo; 
int posServo = 0;

#define ledPin1 6
#define ledPin2 7

#define MAX_DISTANCE 400

//distance CM
float duration, distance;

int maxDisplayDistance =30;
int minDistance =2;
int triggerDistance = 10;
bool isTrainComing =false;

int tonePin=5;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); 
  myservo.write(posServo);
  pinMode(ledPin1, OUTPUT); 
  pinMode(ledPin2, OUTPUT); 
  
  pinMode(tonePin,OUTPUT);

}

void loop() {
  if (isTrainComing == false && posServo==0){
    isTrainComing = isComing();
    delay(200);
  }
  if (isTrainComing == true || posServo != 0) {  
      if(posServo==0){
        blinkLights();
      }    
      trainTone();
      servoTurn();     
   }

 }
void trainTone(){
  for(int j=0;j<2;j++){
      for(int i=500;i<=1000;i++) 
      {     
        tone(tonePin,i);       
        delay(4);       
      }  
      for(int i=1000;i>=500;i--) 
     {     tone(tonePin,i);  
        delay(3);  
     } 
  }
  noTone(tonePin);
}
void servoTurn(){
    for(posServo = 0; posServo < 100; posServo += 1)
    {
        myservo.write(posServo);
        delay(15);
        //blinkLights();
    } 
    delay(3000);
    isTrainComing=false;
    blinkLights();
    for(posServo = 100; posServo>=1; posServo-=1)
    {
        myservo.write(posServo);
        delay(15);
        //blinkLights();
    }
}

bool isComing()
{
  distance =(analogRead(A0)*3)/10;
  Serial.println(distance);
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
  if (isTrainComing==true){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  } 
  else{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
}
