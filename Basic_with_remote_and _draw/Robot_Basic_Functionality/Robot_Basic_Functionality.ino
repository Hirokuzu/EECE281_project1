#include "IRremote.h"
#include "IRremoteInt.h"

/* PIN ASSIGNMENTS */
const int TEMP = 0; //Analog pin
const int PHOTOCELL = 2; //Analog pin
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
//const int backLight = 12; //PIN ASSINGMENT TO CHANGE?

/* DISTANCE(ULTRASONIC) VARIABLES */
float dist_ahead;

/* MOTOR VARIABLES & CONSTANTS */
const int MAX_SPEED = 255;
const int SLOW_SPEED = 90;
const int MIN_SPEED = 80;
const int STOP_SPEED = 0;
const int SLOW_DIST = 40; //distance to start slowing` down is 40 cm
const int STOP_DIST = 6; //distance to stop is 6 cm FOR NOW, (improve later)
const int TURN_90_TIME = 505;
const int TURN_SPEED = 145;

/* LIGHT/BLINKER VARIABLES */
unsigned long currentTime = 0;
unsigned long prevTime = 0;
int headlightOverride;
int photocellValue;
int count = 0;
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

/* DRAWING VARIABLES & CONSTANTS */
const int SEGMENT_TIME= 1000;


int first = 0; //DONT KNOW WHAT THIS ONE IS FOR


void setup(){
  
  analogReference(INTERNAL); //change aRef to ~1.1V
  Serial.begin(9600);
  
  // Start the receiver
  ir_recv.enableIRIn(); 
  
  //set up pin modes
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);

  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);

  pinMode(HEADLIGHT, OUTPUT);
  pinMode(BRAKELIGHT, OUTPUT);
  pinMode(LEFT_BLINKER, OUTPUT);
  pinMode(RIGHT_BLINKER, OUTPUT);
  //pinMode(backLight, OUTPUT);
  
  //set sensor modes
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO,INPUT);
  
  digitalWrite(HEADLIGHT, HIGH);

  dist_ahead = getDistance();
  
  setMotorsForward();
  stopMotors();  
  
}

void loop(){ 
 
    
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
        drawSix();
        break;
      case KEY_7:
        Serial.println("The button pressed is 7.");
        drawSeven();
        break;
      case KEY_8:
        Serial.println("The button pressed is 8.");
        drawEight();
        break;
      case KEY_9:
        Serial.println("The button pressed is 9.");
        drawNine();
        break;
      case KEY_0:
        Serial.println("The button pressed is 0.");
        drawZero();
        break;
      case KEY_UP: //set motors to go forward, doesnt move tho
        Serial.println("The button pressed is up.");
        setMotorsForward();
        break;
      case KEY_DOWN: //set motors to go backward, doesnt move tho
        Serial.println("The button pressed is down.");
        setMotorsBackward();
        break;
      case KEY_RIGHT: //turns right and sets motors forward
        Serial.println("The button pressed is ->");
        turnRight();
        break;
      case KEY_LEFT: //turns left and sets motors forward
        Serial.println("The button pressed is <-");
        turnLeft();
        break;
      case KEY_OK: //doesnt have any functionality currently
        Serial.println("The button pressed is OK.");
        break;
      case KEY_RED: //red is for stop motors
        Serial.println("The button pressed is red.");
        stopMotors();
        break;
      case KEY_GREEN: //green is for set motors MAX_SPEED
        Serial.println("The button pressed is green.");
        setMotorSpeed(MAX_SPEED);
        break;
      case KEY_YELLOW: //yellow is for set motors SLOW_SPEED
        Serial.println("The button pressed is yellow.");
        setMotorSpeed(SLOW_SPEED);
        break;
      case KEY_BLUE: //doesnt have any functionality currently
        Serial.println("The button pressed is blue.");
        break;
      case KEY_AV: //maybe used to switch to 2nd functionality
        Serial.println("The button pressed is AV.");
        break;
      case KEY_TV: //maybe used to switch to basic functionality
        Serial.println("The button pressed is TV.");
        break;
      default:
        Serial.println("Cannot recognize keycode");
        break;
    }
  ir_recv.resume(); //receive next value
  }
    
 
    
    /* THIS IS THE LOOP FOR BASIC FUNCTIONALITY */ 
    /* 
    //Use Ultrasonic sensor to detect and then calculate distance
    dist_ahead = getDistance();
    setHeadlights();   
    
    if(dist_ahead > SLOW_DIST){
      setMotorSpeed(MAX_SPEED);
      digitalWrite(BRAKELIGHT, LOW);
      digitalWrite(LEFT_BLINKER, LOW);
    } else if(dist_ahead > STOP_DIST){
      slowDown(dist_ahead);
      digitalWrite(BRAKELIGHT, HIGH);
      //blinkLeft();
      digitalWrite(LEFT_BLINKER, HIGH);
    } else {
      turnLeft();
      digitalWrite(BRAKELIGHT, LOW);
      digitalWrite(LEFT_BLINKER, LOW);
      setMotorsForward();
    }
    */    
    
    // Print distance reading  to the serial monitor    TODO: print to LCD
}

void setMotorsForward() {   //Configure motor settings, doesnt actually drive
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
}

void setMotorsBackward() {
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  //digitalWrite(backLight, HIGH);
  delay(100); //delay to make sure the correct direction is going to process
}

void setMotorSpeed(int pwm_speed){
  analogWrite(MOTOR_LEFT_PWM, pwm_speed); //adjust motor speed difference
  analogWrite(MOTOR_RIGHT_PWM, pwm_speed); //start turning again  
}

void stopMotors() {
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}

void turnLeft() {
  stopMotors();
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(LEFT_BLINKER, HIGH);
  //blinkLeft();
  setMotorSpeed(TURN_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
}

void turnRight() {
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  digitalWrite(RIGHT_BLINKER, HIGH);
  blinkRight();
  setMotorSpeed(MAX_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
}


void slowDown(float dist) {
  float slowrate_f = ((dist-STOP_DIST)/(SLOW_DIST-STOP_DIST)) * 255;
  int slowrate_i = (int)slowrate_f;  
  
  if(slowrate_i < MIN_SPEED)  //THE END OF THE BEEPING
    slowrate_i = MIN_SPEED;

  setMotorSpeed(slowrate_i);
}

void setHeadlights() {
    headlightOverride = 0;
  
  //HEADLIGHT Functionality
  if(headlightOverride == 0){
      photocellValue = analogRead(PHOTOCELL); //reads the light level
      
    if(photocellValue > MED_LIGHT){
      analogWrite(HEADLIGHT, OFF);
    } else if (photocellValue > LOW_LIGHT){
      analogWrite(HEADLIGHT, DAY_TIME_LIGHTS);
    } else {
      analogWrite(HEADLIGHT, ON);
    }
    
  } else if(headlightOverride == 1){ //over ride and on
    analogWrite(HEADLIGHT, ON);
  } else { //over ride and off
    analogWrite(HEADLIGHT, OFF);
  }
  
}

void blinkLeft() {
  //turn light on to start, then start blinking
  digitalWrite(LEFT_BLINKER, ON);
    //take the current time for the next calculation
    currentTime = millis(); 
    if(currentTime - prevTime > 500){ //light blinks ever .5 seconds
      prevTime = currentTime;
      digitalWrite(LEFT_BLINKER, ON); //will get it to switch between the on off every .5 seconds
    } else {
      digitalWrite(LEFT_BLINKER, OFF);
    }
}

void blinkRight() {
  //turn light on to start, then start blinking
  digitalWrite(RIGHT_BLINKER, ON);
  //take the current time for the next calculation
  currentTime = millis(); 
  if(millis() - currentTime > 500){ //light blinks ever .5 seconds
    if( count % 2 == 0){
      digitalWrite(RIGHT_BLINKER, ON); //will get it to switch between the on off every .5 seconds
    } else {
      digitalWrite(RIGHT_BLINKER, OFF);
    }
  }
}


float getDistance() {
  float echotime; //time of pulse in microseconds
  float temperature; //temperature from IC sensor in degree Celsius
  float speedSound; //calculated speed of sound using temperature from IC sensor
  float distance; //distance in cm
  
  temperature = analogRead(TEMP)/9.31;       //read temperature from IC sensor(in Celsius);
  speedSound = 331.5 + (0.6 * temperature);  //calculate speed of sound with temperature in m/s
  speedSound = 10000/speedSound;             //convert speed of sound to us/cm
   
  digitalWrite(TRIG, LOW);      //10us pulse to TRIGger
  delayMicroseconds(10);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
   
  echotime = pulseIn(ECHO, HIGH); //waits for ECHO to go HIGH, starts timer, stops timer when ECHO goes LOW
   
  //calculate distance in cm
  if (echotime > 38000) { //if the distance is too far, just set distance to 0.
    distance = 0;
  } else {
    distance = echotime/speedSound/2;
  }
  
  return distance;
}

void forwardDraw() {
  setMotorSpeed(MAX_SPEED);
  delay(SEGMENT_TIME);
}

void backwardDraw() {
  setMotorsBackward();
  setMotorSpeed(MAX_SPEED);
  delay(SEGMENT_TIME);
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
  stopMotors();
}
