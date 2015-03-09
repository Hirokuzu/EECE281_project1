#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(7, 2, 3, 4, 5, 6);

void setup(){
  
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  
  lcd.begin(16, 2);
  lcd.print("Initalized");
  Serial.begin(9600);
  
}

void loop(){
  delay(50);
}

void receiveEvent(int howMany){
  lcd.clear();
  
  int NUMBER_BYTES = Wire.available(); //take info from other arduino
  char INCOMING_STRING[NUMBER_BYTES]; //create char array to store info before printing
  char SENSOR_READING; //figure out what it's supposed to 
  
  lcd.setCursor(0, 0); //set writing to first row
  
  int i;
  for(i = 0; i < NUMBER_BYTES; i++){  
     INCOMING_STRING[i] = Wire.read();
  }
  
  SENSOR_READING = INCOMING_STRING[0]; //takes the first char as the reading variable
  
  char PRINT_STRING[i];
  
  for(int j = 0; j < (NUMBER_BYTES - 1); j++){
    PRINT_STRING[j] = INCOMING_STRING[j+1];
  }
  PRINT_STRING[i-1] = '\0';
  
  if(SENSOR_READING == 'd'){
    lcd.print("Distance"); //can be 16 chars long, will display on first line
  } else if (SENSOR_READING == 's'){
    lcd.print("Speed");
  } else if (SENSOR_READING == 't'){
    lcd.print("Temperature");
  } else if (SENSOR_READING == 'w'){
    lcd.print("Writing");
  } else {
    lcd.print("Waiting");
  }
  
  lcd.setCursor(0, 1); //change to the second row
  
  if(NUMBER_BYTES > 16){
    //need to scroll
    lcd.print(PRINT_STRING);
    for(int positionCounter = 0; positionCounter < NUMBER_BYTES; positionCounter++){
      lcd.scrollDisplayLeft(); 
    }
  } else{
    lcd.print(PRINT_STRING); //print to second row
  }
  
}
