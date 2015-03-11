#include "IRremote.h"
#include "IRremoteInt.h"

/* PIN ASSIGNMENTS */
const int ECHO = 2;
const int TRIG = 3;
const int MOTOR_RIGHT_DIR = 4;
const int MOTOR_RIGHT_PWM = 5;
const int MOTOR_LEFT_PWM = 6;
const int MOTOR_LEFT_DIR = 7;
const int HEADLIGHT = 8;
const int BRAKELIGHT = 9;
const int LEFT_BLINKER = 10;
const int RIGHT_BLINKER = 11;
const int IR_PIN = 12;  // Connect the output of the IR receiver diode to pin 12.
const int COMS_A = 4; //analog pins
const int COMS_B = 5;
const int TEMP = 0;

/* DISTANCE(ULTRASONIC) VARIABLES */
float dist_ahead;
uint16_t key_pressed;
boolean in_basic_mode = false;


/* MOTOR VARIABLES & CONSTANTS */
const int MAX_SPEED = 255;
const int SLOW_SPEED = 90;
const int MIN_SPEED = 80;
const int STOP_SPEED = 0;
const int SLOW_DIST = 15; //distance to start slowing` down is 10 cm
const int STOP_DIST = 5; //distance to stop is 6 cm FOR NOW, (improve later)
const int TURN_90_TIME = 220;//-------------------------------------------------------------------------------------------------------------------------delay here
const int TURN_SPEED = 145;

/* LIGHT/BLINKER VARIABLES */
unsigned long currentTime = 0;
unsigned long prevTime = 0;
const int LOW_LIGHT = 700;
const int MED_LIGHT = 900;
const int DAY_TIME_LIGHTS = 50;
const int ON = HIGH;
const int OFF = LOW;

/* IR SENSOR VARIABLES & CONSTANTS */ 
IRrecv ir_recv(IR_PIN); // Initialize the irrecv part of the IRremote  library 
decode_results keycode; // Stores the IR received codes
uint16_t last_code = 0; // Keeps track of the last code RX'd
//KEY CODES
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
const uint16_t KEY_RED = 0x36C9;
const uint16_t KEY_GREEN = 0x28D7;
const uint16_t KEY_YELLOW = 0xA857;
const uint16_t KEY_BLUE = 0x6897;
const uint16_t KEY_AV = 0x807F;
const uint16_t KEY_TV = 0xD827;

  float echotimeComp; //time of pulse in microseconds
  float temperatureComp; //temperature from IC sensor in degree Celsius
  float speedSoundComp; //calculated speed of sound using temperature from IC sensor
  float distanceComp; //distance in cm
  float slowrate_f;
  int slowrate_i;

/* DRAWING VARIABLES & CONSTANTS */
const int SEGMENT_TIME= 1000;

void setup(){
  analogReference(INTERNAL); //change aRef to ~1.1V
  Serial.begin(9600);
  ir_recv.enableIRIn(); 

  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(HEADLIGHT, OUTPUT);
  pinMode(BRAKELIGHT, OUTPUT);
  pinMode(LEFT_BLINKER, OUTPUT);
  pinMode(RIGHT_BLINKER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO,INPUT);
  
  digitalWrite(HEADLIGHT, HIGH);
  dist_ahead = getDistance();
  setMotorsForward();
  stopMotors();  
  delay(1000);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop(){  
       operate_in_basic_mode();
}

void turnLeft() {
  Serial.print("turnLeft, ");
  digitalWrite(LEFT_BLINKER, ON);
  stopMotors();
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  delay(50);
  setMotorSpeed(MAX_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
  digitalWrite(LEFT_BLINKER, ON);
  delay(100);
}

void stopMotors() {
  Serial.print("stopMotors, ");
  analogWrite(MOTOR_LEFT_PWM, 0); //adjust motor speed difference
  analogWrite(MOTOR_RIGHT_PWM, 0); //start turning again 
  delay(50);
}

void setMotorsForward() {   //Configure motor settings, doesnt actually drive
  Serial.print("setMotorsForward, ");
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  delay(50);
}

void setMotorsBackward() {
  Serial.print("setMotorsBackward, ");
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  delay(50); //delay to make sure the correct direction is going to process
}

void setMotorSpeed(int pwm_speed){
  Serial.print("setMotorSpeed, ");
  analogWrite(MOTOR_LEFT_PWM, pwm_speed-1); //adjust motor speed difference
  analogWrite(MOTOR_RIGHT_PWM, pwm_speed); //start turning again  
  delay(50);
  Serial.print("MS: ");
  Serial.print(pwm_speed);
}

void turnRight() {
  Serial.print("turnRight, ");
  digitalWrite(RIGHT_BLINKER, ON);
  stopMotors();
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  delay(50);
  setMotorSpeed(MAX_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
  digitalWrite(RIGHT_BLINKER, ON);
  delay(100);
}

void operate_in_basic_mode() {
    float dist_read = getDistance();
    Serial.print(" | D: ");
    Serial.println(dist_read);
    Serial.println();
    // Now, process the distance appropriately:
    if(dist_read > SLOW_DIST) {  // At STOP_DIST,(6cm), and turn left
      setMotorSpeed(MAX_SPEED);
    } else if(dist_read > STOP_DIST) { // Slow down to MIN_SPEED
        slowDown(dist_read);
    } else {
        turnLeft();
    }
}

void slowDown(float dist) {
  slowrate_f = ((dist-STOP_DIST)/(SLOW_DIST-STOP_DIST)) * 255;
  slowrate_i = (int)slowrate_f;  
  
  if(slowrate_i < MIN_SPEED)  //THE END OF THE BEEPING
    slowrate_i = MIN_SPEED+1;

  setMotorSpeed(slowrate_i);
}

float getDistance() {
  
  temperatureComp = analogRead(TEMP)/9.31;       //read temperature from IC sensor(in Celsius);
  
  speedSoundComp = 331.5 + (0.6 * temperatureComp);  //calculate speed of sound with temperature in m/s
  speedSoundComp = 10000/speedSoundComp;             //convert speed of sound to us/cm
   
  digitalWrite(TRIG, LOW);      //10us pulse to TRIGger
  delayMicroseconds(10);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
   
  echotimeComp = pulseIn(ECHO, HIGH); //waits for ECHO to go HIGH, starts timer, stops timer when ECHO goes LOW
  
  distanceComp = echotimeComp/speedSoundComp/2;
 
  return distanceComp;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void processInstruction( uint16_t key){
    switch (key) {
      case KEY_1: drawOne(); break;
      case KEY_2: drawTwo(); break;
      case KEY_3: drawThree(); break;
      case KEY_4: drawFour(); break;
      case KEY_5: drawFive(); break;
      case KEY_6: drawSix(); break;
      case KEY_7: drawSeven(); break;
      case KEY_8: drawEight(); break;
      case KEY_9: drawNine(); break;
      case KEY_0: drawZero(); break;
      case KEY_UP: setMotorsForward(); break; //set motors to go forward, doesnt move tho
      case KEY_DOWN: setMotorsBackward(); break;
      case KEY_RIGHT: turnRight(); break;
      case KEY_LEFT: turnLeft(); break;
      case KEY_OK: in_basic_mode = true; break;
      case KEY_RED: stopMotors(); break;
      case KEY_GREEN: setMotorSpeed(MAX_SPEED); break;
      case KEY_YELLOW: setMotorSpeed(MIN_SPEED); break;
      case KEY_BLUE: break;
      case KEY_AV: break;
      case KEY_TV: break;
      default: break;
    }
}


void forwardDraw() {
  setMotorsForward();
  setMotorSpeed(MAX_SPEED);
  delay(SEGMENT_TIME);
  stopMotors();
}

void backwardDraw() {
  setMotorsBackward();
  setMotorSpeed(MAX_SPEED);
  delay(SEGMENT_TIME);
  stopMotors();
  setMotorsForward();
}


void drawOne() {
  forwardDraw();
  forwardDraw();
  stopMotors();
}

void drawTwo() {
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  stopMotors();
}

void drawThree() {
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  backwardDraw();
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  stopMotors();
}

void drawFour() {
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  backwardDraw();
  backwardDraw();
  stopMotors();
}

void drawFive() {
  turnLeft();
  forwardDraw();
  backwardDraw();
  turnRight();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  stopMotors();
}

void drawSix() {
  turnLeft();
  forwardDraw();
  backwardDraw();
  turnRight();
  forwardDraw();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  stopMotors();
}

void drawSeven() {
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  forwardDraw();
  stopMotors();
}

void drawEight() {
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  stopMotors();
}

void drawNine() {
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  backwardDraw();
  turnLeft();
  forwardDraw();
  forwardDraw();
  turnRight();
  forwardDraw();
  stopMotors();
}

void drawZero() {
  forwardDraw();
  forwardDraw();
  turnLeft();
  forwardDraw();
  forwardDraw();
  turnLeft();
  forwardDraw();
  ;
}
    
