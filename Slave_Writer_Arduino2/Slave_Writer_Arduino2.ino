#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//CONSTANTS
const int BASIC_MODE = 0;
const int EXTRA_MODE = 1;
const int FWD = 0;
const int BCK = 1;
const int LFT = 2;
const int RGT = 3;

//VARIABLES
int mode = EXTRA_MODE;
int distance;
int motorDir;
int numDraw;
int doneFlag = 1;


void setup(){
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  
  lcd.begin(16, 2);
  lcd.print("EECE 281 202 L2C");
  lcd.setCursor(0, 1);
  lcd.print("Project 1 Robot");
  delay(1000);
}


void loop(){
  lcd.clear();
  
  if(mode == BASIC_MODE) {
    lcd.print("MODE: BASIC");
    lcd.setCursor(0, 1);
    lcd.print("DA: ");
    lcd.print(distance);
    lcd.setCursor(6, 1);
    lcd.print(" cm MD: ");
    
    switch(motorDir) {
      case FWD:
        lcd.print("FWD");
        break;
      case BCK: 
        lcd.print("BCK");
        break;
      case LFT:
        lcd.print("LFT");
        break;
      case RGT:
        lcd.print("RGT");
        break;
      default:
        lcd.print("ERR");
        break;
    }
  } else {
    lcd.print("MODE: EXTRA");
    lcd.setCursor(0, 1);
    
    if(doneFlag = 1) {
      lcd.print("IDLE"); //done drawing
    } else {
      lcd.print("DRAWING "); //drawing in progress
      lcd.print(numDraw);
    }   
  }
  
  delay(100);
}


/*
  Data received is assumed (and implemented) to be 
  3 bytes long. The latter 2 bytes have different 
  meanings based on the mode.
  data[0] - mode
  data[1] - distance/number being drawn
  data[2] - motor direction/done flag
*/
void receiveEvent(int howMany){
  int data[3];
  
  for(int i = 0; i < 3; i++){  
    data[i] = Wire.read();
  }
  
  mode = data[0];
  
  if(mode == BASIC_MODE) {
    distance = data[1];
    motorDir = data[2];
  } else {
    numDraw = data[1];
    doneFlag = data[2];
  }
}
