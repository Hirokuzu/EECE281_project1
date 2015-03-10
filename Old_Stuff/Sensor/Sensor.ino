//CONSTANT
const int trig = 7;
const int echo = 6;
const int temp = 0; //Analog pin
const int slowdist = 20; //distance to start slowing down
const int stopdist = 5; //distance to stop

//VARIABLES
float echotime; //time of pulse in microseconds
float temperature; //temperature from IC sensor in degree Celsius
float speedSound; //calculated speed of sound using temperature from IC sensor
float distance; //distance in cm

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  analogReference(INTERNAL); //change aRef to ~1.1V
  Serial.begin(9600);
}

void loop() {
  //read temperature from IC sensor(in Celsius);
   temperature = analogRead(temp)/9.31;
   Serial.print("Temp: ");
   Serial.println(temperature);
   
   //calculate speed of sound with temperature in m/s
   speedSound = 331.5 + (0.6 * temperature);
   
   //convert speed of sound to us/cm
   speedSound = 10000/speedSound;
   Serial.print("Speed of sound: ");
   Serial.println(speedSound); 
   
   //10us pulse to trigger
   digitalWrite(trig, LOW);
   delay(2); //TODO: CHANGE TO 10us DELAY
   digitalWrite(trig, HIGH);
   delay(2); //TODO: CHANGE TO 10us DELAY
   digitalWrite(trig, LOW);
   
   //waits for echo to go HIGH, starts timer, stops timer when echo goes LOW
   echotime = pulseIn(echo, HIGH); 
   
   //calculate distance in cm
   if (echo > 38000) { //if the distance is too far, just set distance to 0.
     distance = 0;
   }
   else {
     distance = echotime/speedSound/2;
     }
   Serial.print("Distance: ");
   Serial.println(distance); 
   
   delay(1000);
   
//   if (distance <= stopdist) {
//     //make motor stop
//   }
//   else if (distance <= slowdist) {
//        speed of robot at every cm interval = ((distance-stopdist)/(slowdist-stopdist)) * 255
//     //make motor gradually slow down
//   }
//   else {
//     //normal speed: 255
//   }
}
