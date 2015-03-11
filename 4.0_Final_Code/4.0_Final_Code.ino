/*
  TURN_90_TIME needs to be updated when it gets new batteries
  need to test IR functionality (that the ok key does stuff and doesnt screw up the basic functionality
  be careful of using serial becaues the IR stuff interfaces wiht it in a way that i dont quite understand, so once you're done using it remove it fully from the code
  will ocasionally start by doing a very small turn or go forward for a few seconds and then just stop as if nothing had happened, only to seemingly restart again as if nothing had happened
*/

#include "IRremote.h"
#include "IRremoteInt.h"
#include <Wire.h>

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


boolean IS_BASIC = 0; //1 is true, 0 is false
unsigned int counterLoop = 0;


/* DISTANCE(ULTRASONIC) VARIABLES */
float dist_ahead;
uint16_t key_pressed;

/* MOTOR VARIABLES & CONSTANTS */
const int MAX_SPEED = 200;
const int SLOW_SPEED = 90;
const int MIN_SPEED = 150;
const int STOP_SPEED = 0;
const int SLOW_DIST = 60; //distance to start slowing` down is 10 cm
const int STOP_DIST = 15; //distance to stop is 6 cm FOR NOW, (improve later)
const int TURN_90_TIME = 350;//-------------------------------------------------------------------------------------------------------------------------delay here, change based on the battery levels (will need to be fine tuned before demo, was 220 earlier yesterday
const int TURN_SPEED = 140;

/* IR SENSOR VARIABLES & CONSTANTS */ 
IRrecv ir_recv(IR_PIN); // Initialize the irrecv part of the IRremote  library 
decode_results keycode; // Stores the IR received for basic loop
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

void setup(){
  analogReference(INTERNAL); //change aRef to ~1.1V - was used for photo cell implementation
  Serial.begin(9600);
  ir_recv.enableIRIn(); 
  Wire.begin();

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
  Wire.beginTransmission(1);
  delay(51);
  Wire.write("dMaster-Slave Up"); //writing to show connection between arduino
  delay(51);
  Wire.endTransmission();
  delay(1000);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop(){  
  
   if(ir_recv.decode(&keycode)){
      Serial.println("Key received.");
     //will scan for a key press 
       uint16_t key_pressed = (keycode.value & 0xFFFF);  //really not sure why we have to & here, the documentation points to not having to, but i've been up for too long to be sure
       
       if (key_pressed == 0xFFFF){ //my understanding is that key held down => 0xFFFF as key pressed so this stops that
          key_pressed = last_code;
        } else {
          last_code = key_pressed;
        }
        
       processInstruction(key_pressed); //decide what to do based on the key that was pressed
       ir_recv.resume();
   }
   
   
   if(IS_BASIC) {
     operate_in_basic_mode();
   }
   
  /*
  if(IS_BASIC == 1){ //It's in basic mode 
    operate_in_basic_mode(); //So it should do basic functionality that was tested
    //Serial.println("i'm in basic mode");
    
    if(ir_recv.decode(&keycode)){
      Serial.println("key was pressed");
     //will scan for a key press 
       uint16_t key_pressed = (keycode.value & 0xFFFF);  //really not sure why we have to & here, the documentation points to not having to, but i've been up for too long to be sure
        
       if(key_pressed == KEY_AV){ //this hasn't been tested for working --------------------------------------------------------------
          Serial.println("OK key was pressed");
          IS_BASIC = 0;
          stopMotors(); //stop what it's doing 
          setMotorsForward(); //set motors forward then wait
          transmitWait();
      }
      ir_recv.resume();
     } 
  }// END OF BASIC MODE LOOP
  
    else {
    //do extra functionality
      if(ir_recv.decode(&keycode)){ //will scan for a key press 
        
        uint16_t key_pressed = (keycode.value & 0xFFFF); // just following the stuff from before not sure if we need the & value here, or can just use keycode.value... which seems to be the case
        
        if (key_pressed == 0xFFFF){ //my understanding is that key held down => 0xFFFF as key pressed so this stops that
          key_pressed = last_code;
        } else {
          last_code = key_pressed;
        }
        
        processInstruction(key_pressed); //decide what to do based on the key that was pressed
        ir_recv.resume();
      }
      
      
      if( countLoop > 100 ){ //will update every hundred loops 
        countLoop = 0;
        transmitDistance(getDistance());
    }
    ++countLoop;
  }
*/  
  ++counterLoop;
  if(counterLoop > 10000){
    transmitDistance(getDistance());
    counterLoop = 0;
  }
}



/*
  Method to stop motors, doesnt change direction or anything, just makes them stop turning
*/
void stopMotors() {
  analogWrite(MOTOR_LEFT_PWM, 0); 
  analogWrite(MOTOR_RIGHT_PWM, 0); 
  delay(100);
}

/*
  Method to set both motors to go the same way and forward (toward the ultrasonic sensor)
*/
void setMotorsForward() {   //Configure motor settings, doesnt actually drive
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  delay(100);
}

/*
  Method to set both motors to go the same way and backwards (away from the ultrasonic sensor)
  note that we have no detection of any sort while going backwards
*/
void setMotorsBackward() {
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  delay(100); //delay to make sure the correct direction is going to process
}

/*
  Method that takes a pwm value for the speed (0-255) and sets the motors to go that speed (doesnt modify direction)
*/
void setMotorSpeed(int pwm_speed){
  analogWrite(MOTOR_LEFT_PWM, pwm_speed-12); //adjust motor speed difference
  analogWrite(MOTOR_RIGHT_PWM, pwm_speed); //start turning again  
  delay(50);
}

/*
  Method to turn left by 90 degrees
*/
void turnLeft() {
  digitalWrite(LEFT_BLINKER, HIGH);
  stopMotors();
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  delay(50);
  setMotorSpeed(MAX_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
  digitalWrite(LEFT_BLINKER, LOW);
}

/*
  Method to turn right by 90 degrees
*/
void turnRight() {
  digitalWrite(RIGHT_BLINKER, HIGH);
  stopMotors();
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  delay(50);
  setMotorSpeed(MAX_SPEED);
  delay(TURN_90_TIME);
  stopMotors();
  setMotorsForward();
  digitalWrite(RIGHT_BLINKER, LOW);
}

/*
  Main method for controling the robot while in default mode
  Logic goes forward at max speed until approaching an object
  will slow down once it hits 15cm away, then will stop at 5cm away
  at that point it will turn 90 degrees left
*/
void operate_in_basic_mode() {
    float dist_read = getDistance();
    //Serial.print("Distance value read was: ");
    //Serial.println(dist_read);
    // Now, process the distance appropriately:
    if (dist_read <= STOP_DIST ){
      while(dist_read <= STOP_DIST) {  // if we need to turn left, repeat left turning until clear
          stopMotors();
          digitalWrite(MOTOR_RIGHT_DIR, HIGH);
          digitalWrite(MOTOR_LEFT_DIR, LOW);
          delay(100);
          setMotorSpeed(MAX_SPEED);
          delay(350);
          setMotorsForward();
          dist_read = getDistance();
      } 
    }
    else if(dist_read <= SLOW_DIST) { // Slow down to stop distance (5cm)
        slowDown(dist_read);
    } 
    else {
        setMotorSpeed(MAX_SPEED);
    }
}

/*
  Method invoked by basic mode which slows it down as a proportion of the distance left to travel
*/
void slowDown(float dist) {
  float slowrate_f;
  int slowrate_i;
  
  slowrate_f = ((dist-STOP_DIST)/(SLOW_DIST-STOP_DIST)) * 255;
  slowrate_i = (int)slowrate_f;  
  
  if(slowrate_i < MIN_SPEED)  //THE END OF THE BEEPING
    slowrate_i = MIN_SPEED;

  setMotorSpeed(slowrate_i);
}

/*
  Method that gets the distance away from the ultrasonic sensor the thing in front of it
  returns the value as a float
*/
float getDistance() {

  float echotimeComp; //time of pulse in microseconds
  float temperatureComp; //temperature from IC sensor in degree Celsius
  float speedSoundComp; //calculated speed of sound using temperature from IC sensor
  float distanceComp; //distance in cm
  
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

/*
  Method to concat the mode character and a string message for sending to the slave arduino
  returns a const char* that can be integraded by using strcpy(String, char*) and then sending the resulting string to the slave
*/
const char* parseTransmission(char mode, String message){
  int i;
  char currentChar = 'a';
  for( i = 0; currentChar != '\0' ; i++){
    currentChar = message[i];
  }//i is now the size of string
  char parsed [i+1];
  parsed[i] = '\0'; 
  parsed[0] = mode;
  for(int j = 1; j < i ; j++){
    parsed[j] = message[j-1];
  }
  const char* resultS = parsed;
  return resultS;
}

/*
  method to parse an int value
  returns a const char* that can be integraded by using strcpy(String, char*) and then sending the resulting string to the slave
*/
const char* parseNum(char mode, int value){
  String valueString = String(value);
  const char* resultN = parseTransmission(mode, valueString);
  return resultN;
}

/*
  method that transmit the signal to have the lcd print Waiting
*/
void transmitWait(){
  Wire.beginTransmission(1);
  delay(51);
  Wire.write('a'); //writing being controlled
  delay(51);
  Wire.endTransmission();

}

/*
  Method to transmit the signal to have the lcd print that it's being remote controlled
*/
void transmitRC(){
  Wire.beginTransmission(1);
  delay(51);
  Wire.write('r'); //writing being controlled
  delay(51);
  Wire.endTransmission();
}

/*
  Method to have the LCD print the given distance
*/
void transmitDistance(int distance){
  const char* constDisString = parseNum('d', distance);
  char ptrString[100];
  memset(ptrString, '\0', sizeof(ptrString));
  strcpy(ptrString, constDisString);
  Wire.beginTransmission(1);
  delay(51);
  Wire.write(ptrString); //writing Speed then value
  delay(51);
  Wire.endTransmission();
}

/*
  Method to have the LCD print the passed speed
*/
void transmitSpeed(int speedy){
  const char* constPtrString = parseNum('s', speedy);
  char ptrString[100];
  memset(ptrString, '\0', sizeof(ptrString));
  strcpy(ptrString, constPtrString);
  Wire.beginTransmission(1);
  
  Wire.write(ptrString); //writing Speed then value
  delay(10);
  Wire.endTransmission();
}

/*
  Method to have the LCD print that it's drawing a number as well as display the number being drawn
*/
void transmitDraw(int number){
  delay(51);
  const char* constStringPoint = parseNum('w', number);
  char ptrString[100];
  memset(ptrString, '\0', sizeof(ptrString));
  strcpy(ptrString, constStringPoint);
  Wire.beginTransmission(1);
  delay(51);
  Wire.write(ptrString); //writing then number
  delay(51);
  Wire.endTransmission();
  delay(51);
}


void processInstruction( uint16_t key){ //-------------------------------------------------------------------------------I was THROWING ERRORS, now i am beautiful /s
    Serial.println("Processing key...");
    
    if( key == KEY_1) goto was1;
    if( key == KEY_2) goto was2;
    if( key == KEY_3) goto was3;
    if( key == KEY_4) goto was4;
    if( key == KEY_5) goto was5;
    if( key == KEY_6) goto was6;
    if( key == KEY_7) goto was7;
    if( key == KEY_8) goto was8;
    if( key == KEY_9) goto was9; 
    if( key == KEY_0) goto was0;
    if( key == KEY_UP) goto wasUp; 
    if( key == KEY_DOWN) goto wasDown;
    if( key == KEY_RIGHT) goto wasRight; 
    if( key == KEY_LEFT) goto wasLeft; 
    if( key == KEY_OK) goto ending;
    if( key == KEY_RED) goto wasRed;
    if( key == KEY_GREEN) goto wasGreen;
    if( key == KEY_YELLOW) goto wasYellow;
    if( key == KEY_BLUE) goto ending;
    if( key == KEY_AV) goto wasAV;
    if( key == KEY_TV) goto wasTV;
    else goto ending;
    //this part sucks, but just takes the old case statements as if statements now and uses goto as the way to decide where to go (like a look-up table when optimized)
    was1:
        transmitDraw(1);
        drawOne();
        goto ending;
    was2:
        transmitDraw(2);
        drawTwo();
        goto ending;
    was3:
        transmitDraw(3);
        drawThree();
        goto ending;
    was4:
        transmitDraw(4);
        drawFour();
        goto ending;
    was5:
        transmitDraw(5);
        drawFive();
        goto ending;
    was6:
        transmitDraw(6);
        drawSix();
        goto ending;
    was7:
        transmitDraw(7);
        drawSeven();
        goto ending;
    was8:
        transmitDraw(8);
        drawEight();
        goto ending;
    was9:
        transmitDraw(9);
        drawNine();
        goto ending;
    was0:
        transmitDraw(0);
        drawZero();
        goto ending;
    wasUp:
        transmitRC();
        setMotorsForward(); 
        goto ending;
    wasDown:
        transmitRC();
        setMotorsBackward(); 
        goto ending;
    wasRight:
        transmitRC();
        turnRight(); 
        goto ending;
    wasLeft:
        transmitRC(); 
        turnLeft();
        goto ending;
    wasTV: //this key needs to be different because it's special -----------------------------------------------
        IS_BASIC = 1;
        stopMotors();
        setMotorsForward();
        goto ending;
    wasAV:
        IS_BASIC = 0;
        stopMotors();
        setMotorsForward();
        goto ending;
    wasRed:
        transmitWait();
        stopMotors();
        goto ending;
    wasGreen:
        setMotorSpeed(MAX_SPEED);
        goto ending;
    wasYellow:
        setMotorSpeed(MIN_SPEED);
        goto ending;
    ending:
        int i = 0;
  //after finishing will display that it's waiting because you know... it actually is
}

//--------------------------------------------------------------------------------------------Haven't tested anything below here
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
}

void drawThree() {
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  turnRight();
  forwardDraw();
  backwardDraw();
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
}

void drawFour() {
  forwardDraw();
  turnLeft();
  forwardDraw();
  turnLeft();
  forwardDraw();
  backwardDraw();
  backwardDraw();
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
}

void drawSeven() {
  turnLeft();
  forwardDraw();
  turnRight();
  forwardDraw();
  forwardDraw();
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
}

void drawZero() {
  forwardDraw();
  forwardDraw();
  turnLeft();
  forwardDraw();
  forwardDraw();
  turnLeft();
  forwardDraw();
}
