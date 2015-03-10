#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 2, 3, 4, 5, 6); //sets up the liquid crystal desplay using 4 pin implementation (better than 8 pin)

void setup(){
  
  Wire.begin(1); //start up code to set up the wire functionality (communictation through 2 arduinos using Master-Slave relation rather than serial) and given the label 1 (can hook up more arduinos if we need to in the future with this implementation)
  Wire.onReceive(receiveEvent); //creates a handle for reciving messages and doing things once it recieves a transmission
  
  lcd.begin(16, 2); // sets the lcd to use 2 row, 16 char spaces
  lcd.print("Initalized"); //prints to the lcd that it's started, used to make sure the set up works properly
  
}

void loop(){
}

void receiveEvent(int howMany){ //runs every time the slave arduino is given something by wire.transmit from the master
  lcd.clear(); //need to clear the lcd every time
  
  int NUMBER_BYTES = Wire.available(); //take info from other arduino
  char INCOMING_STRING[NUMBER_BYTES]; //create char array to store info before printing
  char SENSOR_READING; //figure out what it's supposed to 
  
  lcd.setCursor(0, 0); //set writing to first row
  
  int i;
  for(i = 0; i < NUMBER_BYTES; i++){  //processes the incoming message into an array of characters so we can parse it
     INCOMING_STRING[i] = Wire.read();
  }
  
  SENSOR_READING = INCOMING_STRING[0]; //takes the first char as the mode the robot is operating in
  
  char PRINT_STRING[i]; //will become the string we print to the second line of th elcd
  
  for(int j = 0; j < (i-1); j++){
    PRINT_STRING[j] = INCOMING_STRING[j+1]; //transfering the incoming bytes into a clean string to print
  }
  PRINT_STRING[i-1] = '\0'; //adding null character for char[] => string conversion
  
  if(SENSOR_READING == 'd'){ //figuring out which mode it is in using a sort of state machine (char controlled rather than number_
    lcd.print("Distance"); 
  } else if (SENSOR_READING == 's'){ //wont use too often as speed doesnt change super often
    lcd.print("Speed");
  } else if (SENSOR_READING == 't'){ //just included for robustness
    lcd.print("Temperature");
  } else if (SENSOR_READING == 'w'){ //used when the robot is drawing something, will be this state until it finishes writing at least, then should go back to waiting
    lcd.print("Writing");
  } else if (SENSOR_READING == 'r'){ //used when the remote is controlling the motion of the robot
    lcd.print("Being Controlled");
  } else {
    lcd.print("Waiting"); //if nothing else will just send a single bit to signify waiting, if it prints something else and waiting we know there's somthing wrong with what we're being sent
  }
  
  lcd.setCursor(0, 1); //change to the second row
  
  if(NUMBER_BYTES > 16){ //lcd has 16 spaces per row, so if we have more characters to print it'll need to scroll to read the whole message
    lcd.print(PRINT_STRING); //have to start by trying to print everything
    for(int positionCounter = 0; positionCounter < NUMBER_BYTES; positionCounter++){ //will then impliment a scrolling of the second row so everything can be read
      lcd.scrollDisplayLeft(); 
    }
  } else{
    lcd.print(PRINT_STRING); //print to second row as the number of characters fits in the second row
  }
  
}


