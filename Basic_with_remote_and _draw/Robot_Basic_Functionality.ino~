#include "IRremote.h"
#include "IRremoteInt.h"

//DRAW constants
const int toggleDraw = 4;
const int numberHeight = 2000;
const int numberWidth = 1000;

/* Define key codes */
const uint16_t KEY_1 = 0x20DF;
const uint16_t KEY_2 = 0xA05F;
const uint16_t KEY_3 = 0x609F;
const uint16_t KEY_4 = 0x10EF;
const uint16_t KEY_5 = 0x906F;
const uint16_t KEY_6 = 0x50AF;
const uint16_t KEY_7 = 0x30CF;
const uint16_t KEY_8 = 0xB04F;
const uint16_t KEY_9 = 0x708F;
const uint16_t KEY_0 = 0x8877;
const uint16_t KEY_UP = 0x06F9;
const uint16_t KEY_DOWN = 0x8679;
const uint16_t KEY_RIGHT = 0x46B9;
const uint16_t KEY_LEFT = 0xA659;
const uint16_t KEY_OK = 0x16E9;

/* Connect the output of the IR receiver diode to pin 11. */
int ir_pin = 12;
/* Initialize the irrecv part of the IRremote  library */
IRrecv ir_recv(ir_pin);
decode_results keycode; // This will store our IR received codes
uint16_t last_code = 0; // This keeps track of the last code RX'd

// GLOBAL VARIABLES
int photocellValue;
int count = 0;
int headLightOverride;
float dist_ahead;

// PIN ASSIGNMENTS AND OTHER CONSTANTS
const int temp = 0; //Analog pin
const int photoCell = 2; //Analog pin
const int echo = 2;
const int trig = 3;
const int motorRightDirection = 4;
const int motorRightPWM = 5;
const int motorLeftPWM = 6;
const int motorLeftDirection = 7;
const int headLight = 8;
const int brakeLight = 9;
const int leftBlinker = 10;
const int rightBlinker = 11;
int first = 0;
unsigned long currentTime = 0;
unsigned long prevTime = 0;


const int FULL_SPEED = 255;
const int MIN_SPEED = 80;
const int STOP_SPEED = 0;

const int SLOW_DIST = 40; //distance to start slowing` down is 40 cm
const int STOP_DIST = 6; //distance to stop is 6 cm FOR NOW, (improve later)

const int TURN_90_TIME = 400;
const int TURN_SPEED = 145;

const int lowLight = 700;
const int medLight = 900;
const int dayTimeRunningLights = 50;

const int ON = HIGH;
const int OFF = LOW;

void setup(){
  
  analogReference(INTERNAL); //change aRef to ~1.1V

  // Start the receiver
  ir_recv.enableIRIn(); 
  
  //set up pin modes
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);

  pinMode(motorLeftDirection, OUTPUT);
  pinMode(motorRightDirection, OUTPUT);

  pinMode(headLight, OUTPUT);
  pinMode(brakeLight, OUTPUT);
  pinMode(leftBlinker, OUTPUT);
  pinMode(rightBlinker, OUTPUT);
  
  //set sensor modes
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  
  digitalWrite(headLight, HIGH);

  dist_ahead = getDistance();
  
  setMotorsForward();
  setMotorSpeed(STOP_SPEED);
  
  Serial.begin(9600);
  
}

void loop(){ 
    
//  if(first == 0){
//    turnLeft();
//    first = 2;
//    delay(10);
//  }
    
    
  if (ir_recv.decode(&keycode)) //if it sensed something
  {
    /* read the RX'd IR into a 16-bit variable: */
        uint16_t key = (keycode.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a 
     button is held down. If we get 0xFFFFFFF, let's just
     assume the previously pressed button is being held down */
    if (key == 0xFFFF)
      key = last_code;
    else
      last_code = key;
    
    switch (key) {
      case KEY_1:
        Serial.println("The button pressed is 1.");
        drawOne();
        break;
      case KEY_2:
        Serial.println("The button pressed is 2.");
        drawTwo();
        break;
      case KEY_3:
        Serial.println("The button pressed is 3.");
        drawThree();
        break;
      case KEY_4:
        Serial.println("The button pressed is 4.");
        drawFour();
        break;
      case KEY_5:
        Serial.println("The button pressed is 5.");
        drawFive();
        break;
      case KEY_6:
        Serial.println("The button pressed is 6.");
        break;
      case KEY_7:
        Serial.println("The button pressed is 7.");
        break;
      case KEY_8:
        Serial.println("The button pressed is 8.");
        break;
      case KEY_9:
        Serial.println("The button pressed is 9.");
        break;
      case KEY_0:
        Serial.println("The button pressed is 0.");
        break;
      case KEY_UP:
        Serial.println("The button pressed is up.");
        break;
      case KEY_DOWN:
        Serial.println("The button pressed is down.");
        break;
      case KEY_RIGHT:
        Serial.println("The button pressed is ->");
        break;
      case KEY_LEFT:
        Serial.println("The button pressed is <-");
        break;
      case KEY_OK:
        Serial.println("The button pressed is OK.");
        break;
      default:
        Serial.println("Cannot recognize keycode");
        break;
    }
  ir_recv.resume(); //receive next value
  }
    
    
    
    
  
    
    /*****THIS is the actual loop******
    //Use Ultrasonic sensor to detect and then calculate distance
    dist_ahead = getDistance();
    setHeadLights();   

    
    if(dist_ahead > SLOW_DIST){
      setMotorSpeed(FULL_SPEED);
      digitalWrite(brakeLight, LOW);
      digitalWrite(leftBlinker, LOW);
    } else if(dist_ahead > STOP_DIST){
      slowDown(dist_ahead);
      digitalWrite(brakeLight, HIGH);
      //blinkLeft();
      digitalWrite(leftBlinker, HIGH);
    } else {
      turnLeft();
      digitalWrite(brakeLight, LOW);
      digitalWrite(leftBlinker, LOW);
      setMotorsForward();
    }
    *****THIS is the actual loop*****/
    
    
    
    
    /*
    if(dist_ahead <= STOP_DIST) {  // At STOP_DIST,(6cm), and turn left
      digitalWrite(brakeLight, LOW);
      turnLeft();
      setMotorsForward();
    } else if(dist_ahead <= SLOW_DIST) { // Slow down to MIN_SPEED
      slowDown(dist_ahead);
      digitalWrite(brakeLight, HIGH);
      //blinkLeft();
      digitalWrite(leftBlinker, LOW);
    } else {                   // Otherwise keep driving full speed
      setMotorSpeed(FULL_SPEED);
    }
    */
    
    
    // Print distance reading  to the serial monitor    TODO: print to LCD
}

void setMotorsForward() {   //Configure motor settings, doesnt actually drive
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, HIGH);
}

// CURRENTLY WE ARE NOT USING THIS METHOD
//void setMotorsBackward() {
//  digitalWrite(motorLeftDirection, LOW);
//  digitalWrite(motorRightDirection, LOW);
//  delay(100); //delay to make sure the correct direction is going to process
//}

void setMotorSpeed(int pwm_speed){
  analogWrite(motorLeftPWM, pwm_speed); //adjust motor speed difference
  analogWrite(motorRightPWM, pwm_speed); //start turning again  
}

void turnLeft() {
  setMotorSpeed(STOP_SPEED);
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, LOW);
  digitalWrite(leftBlinker, HIGH);
  setMotorSpeed(TURN_SPEED);
  delay(TURN_90_TIME);
  setMotorSpeed(STOP_SPEED);
  setMotorsForward();
}

void turnRight() {
  digitalWrite(motorRightDirection, LOW);
  digitalWrite(motorLeftDirection, HIGH);
  setMotorSpeed(FULL_SPEED);
  delay(TURN_90_TIME);
  setMotorSpeed(STOP_SPEED);
  setMotorsForward();
}


void slowDown(float dist) {
  float slowrate_f = ((dist-STOP_DIST)/(SLOW_DIST-STOP_DIST)) * 255;
  int slowrate_i = (int)slowrate_f;  
  
  if(slowrate_i < MIN_SPEED)  //THE END OF THE BEEPING
    slowrate_i = MIN_SPEED;

  setMotorSpeed(slowrate_i);
}

void setHeadLights() {
    headLightOverride = 0;
  
  //Headlight Functionality
  if(headLightOverride == 0){
      photocellValue = analogRead(photoCell); //reads the light level
      
    if(photocellValue > medLight){
      analogWrite(headLight, OFF);
    } else if (photocellValue > lowLight){
      analogWrite(headLight, dayTimeRunningLights);
    } else {
      analogWrite(headLight, ON);
    }
    
  } else if(headLightOverride == 1){ //over ride and on
    analogWrite(headLight, ON);
  } else { //over ride and off
    analogWrite(headLight, OFF);
  }
  
}

void blinkLeft() {
  //turn light on to start, then start blinking
  digitalWrite(leftBlinker, ON);
    //take the current time for the next calculation
    currentTime = millis(); 
    if(currentTime - prevTime > 500){ //light blinks ever .5 seconds
      prevTime = currentTime;
      digitalWrite(leftBlinker, ON); //will get it to switch between the on off every .5 seconds
    } else {
      digitalWrite(leftBlinker, OFF);
    }
}

void blinkRight() {
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
}


float getDistance() {
  float echotime; //time of pulse in microseconds
  float temperature; //temperature from IC sensor in degree Celsius
  float speedSound; //calculated speed of sound using temperature from IC sensor
  float distance; //distance in cm
  
  temperature = analogRead(temp)/9.31;       //read temperature from IC sensor(in Celsius);
  speedSound = 331.5 + (0.6 * temperature);  //calculate speed of sound with temperature in m/s
  speedSound = 10000/speedSound;             //convert speed of sound to us/cm
   
  digitalWrite(trig, LOW);      //10us pulse to trigger
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
   
  echotime = pulseIn(echo, HIGH); //waits for echo to go HIGH, starts timer, stops timer when echo goes LOW
   
  //calculate distance in cm
  if (echo > 38000) { //if the distance is too far, just set distance to 0.
    distance = 0;
  } else {
    distance = echotime/speedSound/2;
  }
  
  return distance;
}

void drawOne() {
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight);
  setMotorSpeed(STOP_SPEED);
}

void drawTwo() {
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  setMotorSpeed(STOP_SPEED);
}

void drawThree() {
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnLeft();
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnRight();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  setMotorSpeed(STOP_SPEED);
}

void drawFour() {
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnLeft();
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight);
  setMotorSpeed(STOP_SPEED);
}

void drawFive() {
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberWidth);
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight/2);
  turnLeft();
  turnLeft();
  setMotorSpeed(FULL_SPEED);
  delay(numberHeight);
  setMotorSpeed(STOP_SPEED);
}
