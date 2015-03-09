/*
  this is the base info for the master writer arduino (the main arduino)
  
  you need the include and the begin statements, as well as the begin/endTransmission before every bit of data is sent
  
  when transmitting data to the second arduino here what the 1st charactre will be
  
  d = writing distance to the LCD
  s = Speed 
  t = temperature
  w = writing
  
  anything else will have it printing waiting, but you need to includ an extra char
  
  after that it'll print your string in the second row.
  
  ex:
  Wire.write("d120");
  will show as
  
  Distance
  120
*/

#include <Wire.h>
#include <SoftwareSerial.h>

void setup(){
  Wire.begin();
  Serial.begin(9600);
}

void loop(){
  
  delay(1000);
  Wire.beginTransmission(1);
  Wire.write("d120"); //distance 120
  Wire.endTransmission();
  
  delay(1000);
  
  Wire.beginTransmission(1);
  Wire.write("w1"); //writing 1
  Wire.endTransmission();
  
  delay(1000);
  
  Wire.beginTransmission(1);
  Wire.write("sMax"); //speed Max
  Wire.endTransmission();
  
  delay(1000);
  
  Wire.beginTransmission(1);
  Wire.write("a"); // waiting and no other text
  Wire.endTransmission();
  
  delay(1000);
  
  Wire.beginTransmission(1);
  char* toPrint = parseTransmission( 's', "100");
  String test = toPrint;
  Serial.print(toPrint);
  Wire.write(toPrint);
  Wire.endTransmission();
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
