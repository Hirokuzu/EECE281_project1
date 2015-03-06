#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  
  lcd.begin(16, 2);
  lcd.print("Initalized");
  
}

void loop(){
  delay(50);
}

void receiveEvent(int howMany){
  
  int NUMBER_BYTES = Wire.available(); //take info from other arduino
  char INCOMING_STRING[NUMBER_BYTES + 1]; //create char array to store info before printing
  char SENSOR_READING = Wire.read(); //figure out what it's supposed to 
  
  lcd.setCursor(0, 0); //set writing to first row
  
  if(SENSOR_READING == d){
    lcd.print("Distance"); //can be 16 chars long, will display on first line
  } else if (SENSOR_READING == s){
    lcd.print("Speed");
  ) else if (SENSOR_READING == t){
    lcd.print("Temperature");
  } else if (SENSOR_READING == w){
    lcd.print("Writing");
  } else {
    lcd.print("Waiting");
  }
  
  lcd.setCursor(0, 1); //change to the second row
  
  for(int i = 0; i < NUMBER_BYTES; i++){
     INCOMING_STRING[i] = Wire.read();
  }
  
  INCOMING_STRING[NUMBER_BYTES + 1] = '\0';
  
  if(NUMBER_BYTES > 16){
    //need to scroll
    lcd.print(INCOMING_STRING);
    for(int positionCounter = 0; positionCounter < NUMBER_BYTES; positionCounter++){
      lcd.ScrollDisplayLeft(); 
    }
  } else{
    lcd.print(INCOMING_STRING); //print to second row
  }
  
}
    
