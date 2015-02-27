/*
  Program: Light Control for EECE 281 Project 1 - Car
  
  Uses a state machine to control most lights on the car, and a photo cell to control the headlights of the car.
  The state machine uses a bit to determine which state it's in:
  
  0 - No lights are on (except maybe headlights, assumed for general driving)
  1 - Braking, the car is slowing down or stopped (while resting the car should be in this state)
  2 - Left turn, puts on the left blinker and blinks
  3 - Right turn, puts on the right blinker and blinks
  4 - Moving backwards, turns on the brake light to simulate any backwards facing lights for reversing
  5 - LED Testing, all LED's blink quickly
  
  Headlights are controlled based on the light levels the car is in, will turn on day time running lights
  when the light levels get a little lower, and turn on 100% when it's in a low light environment
  
  Uses headlightOverride as a variable where
  0 - automatic headlights
  1 - Override ON
  2 - Override OFF
  
*/

//movement state machine
const char movPause = 0;
const char movBreaking = 1;
const char movLeft = 2;
const char movRight = 3;
const char movBack = 4;
const char lightTest = 5;
int stateMachine;
//LED pin names
const int headLights = 8;
const int brakeLight = 11;
const int leftBlinker = 12;
const int rightBlinker = 13;
//LED constant names
const int ON = HIGH;
const int OFF = LOW;
unsigned long currentTime;
int count = 0;

//Headlight control
int headlightOverride;
const int photocell = 1;
const int lowLight = 700;
const int medLight = 900;
const int dayTimeRunningLights = 50;
int photocellValue;

void setup(){
  //set up pin modes
  pinMode(headLights, OUTPUT);
  pinMode(brakeLight, OUTPUT);
  pinMode(leftBlinker, OUTPUT);
  pinMode(rightBlinker, OUTPUT);
  pinMode(photocell, INPUT);
}

void loop(){
  stateMachine = 0;
  headlightOverride = 0;
  
  //Headlight Functionality
  if(headlightOverride == 0){
      photocellValue = analogRead(photocell); //reads the light level
    if(photocellValue > medLight){
      analogWrite(headLights, OFF);
    } else if (photocellValue > lowLight){
      analogWrite(headLights, dayTimeRunningLights);
    } else {
      analogWrite(headLights, ON);
    }
  } else if(headlightOverride == 1){ //over ride and on
    analogWrite(headLights, ON);
  } else { //over ride and off
    analogWrite(headLights, OFF);
  } 
  
  //Stop Light Functionality
  if(stateMachine == movBreaking || stateMachine == movBack){
    digitalWrite(brakeLight, ON);
  } else {
    digitalWrite(brakeLight, OFF);
  }
  
  //Right Blinker Functionality
  if(stateMachine == movRight){
    //turn light on to start, then start blinking
    digitalWrite(rightBlinker, ON);
    //take the current time for the next calculation
    currentTime = millis(); 
    if(millis() - currentTime > 500){ //light blinks ever .5 seconds
      if( count % 2 == 0){
        digitalWrite(rightBlinker, ON); //will get it to switch between the on off every .5 seconds
      } else {
        digitalWrite(rightBlinker, OFF);
      }
    }
  } else {
    digitalWrite(rightBlinker, OFF);
  }
  
  //Left Blinker Functionality
  if(stateMachine == movLeft){
    //turn light on to start, then start blinking
    digitalWrite(leftBlinker, ON);
    //take the current time for the next calculation
    currentTime = millis(); 
    if(millis() - currentTime > 500){ //light blinks ever .5 seconds
      if( count % 2 == 0){
        digitalWrite(leftBlinker, ON); //will get it to switch between the on off every .5 seconds
      } else {
        digitalWrite(leftBlinker, OFF);
      }
    }
  } else {
    digitalWrite(leftBlinker, OFF);
  }
  
  //LED Testing Functionality
  if(stateMachine == lightTest){
    int i;
    for(i = 0; i != 2; ++i){
      digitalWrite(leftBlinker, ON);
      digitalWrite(rightBlinker, ON);
      digitalWrite(headLights, ON);
      digitalWrite(brakeLight, ON);
      delay(50);
      digitalWrite(leftBlinker, OFF);
      digitalWrite(rightBlinker, OFF);
      digitalWrite(headLights, OFF);
      digitalWrite(brakeLight, OFF);
      delay(50);
    }
  }
  
  ++count;
}
