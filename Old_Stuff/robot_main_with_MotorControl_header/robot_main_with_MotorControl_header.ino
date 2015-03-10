#include "IRremote.h"
#include "IRremoteInt.h"
#include "MotorControl.h"
#include <Wire.h>
#define LOOP_DELAY 1000

boolean in_basic_mode = false;
// ultrasonic sensor variables and constants
unsigned long prevTime = 0;
unsigned long currentTime = 0;
int headlightOverride;
int photocellValue;
int count = 0;
const int backLight = 12;
float dist_ahead;
const int TRIG = 2;             // PIN ASSIGNMENTS
const int ECHO = 3;
const int TEMP = 0;             //Analog pin
uint16_t key_pressed;
const int IR_PIN = 12; 
const uint16_t KEY_1 = 0x20DF;  //KEY CODES
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

MotorControl motor_o(R_DIR, L_DIR, R_PWM, L_PWM, HEADL, BRAKEL, SIG_L, SIG_R); // construct class variable

/* IR SENSOR VARIABLES & CONSTANTS */ 
IRrecv ir_recv(IR_PIN); // Initialize the irrecv part of the IRremote  library 
decode_results keycode; // Stores the IR received codes
uint16_t last_code = 0; // Keeps track of the last code RX'd


void setup(){
  //note: motor gets setup in motor library, so its not here
  Serial.begin(9600);
  analogReference(INTERNAL); //change aRef to ~1.1V
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO,INPUT);
  ir_recv.enableIRIn();    // Start the receiver
  Wire.begin();
  
  Wire.beginTransmission(1);
  Wire.write("a"); //waiting
  Wire.endTransmission();
}

void loop(){  
  
  if ( in_basic_mode && ir_recv.decode(&keycode) ){ // if in basic mode and button gets pressed...
       key_pressed = (keycode.value & 0xFFFF);           // change out of basic mode only if OK was pressed:
       if ( key_pressed != KEY_OK )
          operate_in_basic_mode();
       else
          in_basic_mode = false;
          
       ir_recv.resume(); //receive next value
  }
  
  else if( !in_basic_mode && ir_recv.decode(&keycode) ){
      key_pressed = (keycode.value & 0xFFFF);
      
      if (key_pressed == 0xFFFF)     //The remote will continue to spit out 0xFFFFFFFF if button is held down
        key_pressed = last_code;
      else
        last_code = key_pressed;
        
      processInstruction( key_pressed);
      ir_recv.resume(); //receive next value
  }
}
        
void processInstruction( uint16_t key){ //jake added code here, all the transmit stuff (functions are at the bottom) ---------------------------------------
    switch (key) {
      case KEY_1: transmitDraw("1"); motor_o.drawOne(); break;
      case KEY_2: transmitDraw("2"); motor_o.drawTwo(); break;
      case KEY_3: transmitDraw("3"); motor_o.drawThree(); break;
      case KEY_4: transmitDraw("4"); motor_o.drawFour(); break;
      case KEY_5: transmitDraw("5"); motor_o.drawFive(); break;
      case KEY_6: transmitDraw("6"); motor_o.drawSix(); break;
      case KEY_7: transmitDraw("7"); motor_o.drawSeven(); break;
      case KEY_8: transmitDraw("8"); motor_o.drawEight(); break;
      case KEY_9: transmitDraw("9"); motor_o.drawNine(); break;
      case KEY_0: transmitDraw("0");motor_o.drawZero(); break;
      case KEY_UP: transmitRC(); motor_o.setMotorsForward(); break; //set motors to go forward, doesnt move tho
      case KEY_DOWN: transmitRC(); motor_o.setMotorsBackward(); break;
      case KEY_RIGHT: transmitRC(); motor_o.turnRight(); break;
      case KEY_LEFT: transmitRC(); motor_o.turnLeft(); break;
      case KEY_OK: in_basic_mode = true; break;
      case KEY_RED: transmitSpeed(STOP_SPEED); motor_o.setMotorSpeed(STOP_SPEED); break;
      case KEY_GREEN: transmitSpeed(FULL_SPEED); motor_o.setMotorSpeed(FULL_SPEED); break;
      case KEY_YELLOW: transmitSpeed(MIN_SPEED); motor_o.setMotorSpeed(MIN_SPEED); break;
      case KEY_BLUE: break;
      case KEY_AV: break;
      case KEY_TV: break;
      default: transmitWait(); break;
    }
}

void operate_in_basic_mode() {
  
    float echotime; //time of pulse in microseconds
    float temperature; //temperature from IC sensor in degree Celsius
    float speedSound; //calculated speed of sound using temperature from IC sensor
    float distance; //distance in cm
    
    temperature = analogRead(TEMP)/9.31;       //read temperature from IC sensor(in Celsius);
    speedSound = 331.5 + (0.6 * temperature);  //calculate speed of sound with temperature in m/s
    speedSound = 10000/speedSound;             //convert speed of sound to us/cm
    
    digitalWrite(TRIG, LOW);      //10us pulse to trigger
    delayMicroseconds(10);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    echotime = pulseIn(ECHO, HIGH); //waits for echo HIGH, start timer, & stop timer when echo  LOW
    if (ECHO > 38000) { //Calculate distance in cm
      distance = 0;
    } else {
      distance = echotime/speedSound/2.0;
    }
    // Now, process the distance appropriately:
    if(distance <= STOP_DIST) {  // At STOP_DIST,(6cm), and turn left
      motor_o.turnLeft();
    } else if(distance <= SLOW_DIST) { // Slow down to MIN_SPEED
        motor_o.slowDown(distance);
    } else {
        motor_o.setMotorSpeed(255);
    }
    
    // STUFF JAKE ADDED BELOW-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    String stringToSend;
    
    char* stringPointer = parseInt('d',distance);
    stringToSend = stringPointer;
    
    Wire.beginTransmission(1);
    Wire.write(stringToSend); //distance, then value
    Wire.endTransmission();    
    // END OF STUFF ADDED-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

/*
  Use this function to parse the mode and a message together
*/
char* parseTransmission(char mode, String message){
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
  char* result = parsed;
  return result;
}

char* parseInt(char mode, int value){
 int i = 0;
 int tester = value;
 while (tester != 0){ //gives number of digits
   ++i;
   tester = tester/10;
 }
 char resultString[i+2];
 resultString[i+1] = '\0'; //null in last slot
 resultString[0] = mode; //mode in the first
 tester = value;
 
 for(int j = 1; j < i;j++){ //numbers in the rest
   resultString[i-j] = tester % 10;
   tester = tester/10;
 }
 char* result = resultString;
 return result;
}

void transmitWait(){
  Wire.beginTransmission(1);
  Wire.write('a'); //writing being controlled
  Wire.endTransmission();
}

void transmitRC(){
  Wire.beginTransmission(1);
  Wire.write('r'); //writing being controlled
  Wire.endTransmission();
}

void transmitSpeed(int speedy){
  char* ptrString = parseInt('s', speedy);
  String transmitString = ptrString;
  Wire.beginTransmission(1);
  Wire.write(transmitString); //writing Speed then value
  Wire.endTransmission();
}
void transmitDraw(String number);
  char* stringPoint = parseTransmission('w', number);
  String transmitString = stringPointl
  Wire.beginTransmission(1);
  Wire.write(transmitString); //writing 1
  Wire.endTransmission();
}
