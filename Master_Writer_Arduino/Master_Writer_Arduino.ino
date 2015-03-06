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

void setup(){
  
  Wire.begin();
  
}

void loop(){
  
  Wire.beginTransmission(1);
  Wire.write("INSERT EXAMPLE STRING HERE");
  Wire.write(x); //can also send variables
  Wire.endTransmission();
  
}
