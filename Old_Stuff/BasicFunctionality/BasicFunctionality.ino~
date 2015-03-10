unsigned long prevTime = 2000;
unsigned long currentTime;

//CONSTANT
const int trig = 2;
const int echo = 3;
const int temp = 0; //Analog pin
const int slowdist = 20; //distance to start slowing down
const int stopdist = 1; //distance to stop
const int motorRightDirection = 4;
const int motorRightPWM = 5;
const int motorLeftPWM = 6;
const int motorLeftDirection = 7;
const int looptime = 100; //microseconds it takes to sense new distance and change motor rate
const int turntimeninety = 2000; //time it takes to turn 90 degrees at max speed
const int turntimefortyfive = 1000; //time it takes to turn 45 degrees at max speed

//VARIABLES
float echotime; //time of pulse in microseconds
float temperature; //temperature from IC sensor in degree Celsius
float speedSound; //calculated speed of sound using temperature from IC sensor
float distance; //distance in cm

void setup() {
  
  //set up sensor modes
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  analogReference(INTERNAL); //change aRef to ~1.1V
  
  //set up pin modes
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(motorLeftDirection, OUTPUT);
  
  //set motor directons
  forward();
  
  Serial.begin(9600);
}

void loop() {
  
  currentTime = millis(); //take the current time for the next calculation
  
  if(currentTime - prevTime > 2000) { //every two seconds will change direction (which the following code controls)
    prevTime = currentTime;
  
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
       delayMicroseconds(10);
       digitalWrite(trig, HIGH);
       delayMicroseconds(10);
       digitalWrite(trig, LOW);
       
       //waits for echo to go HIGH, starts timer, stops timer when echo goes LOW
       echotime = pulseIn(echo, HIGH); 
       
       //calculate distance in cm
       distance = echotime/speedSound/2;

       Serial.print("Distance: ");
       Serial.println(distance);
       
       if (distance <= stopdist) {
         stop_robot();
         turnleft(); //turn left 90 degrees;
       }
       else if (distance <= slowdist) {
         int slowrate = ((distance-stopdist)/(slowdist-stopdist)) * 255;
         analogWrite(motorRightPWM, slowrate); //move at the defined slowrate
         analogWrite(motorLeftPWM, slowrate);  
       }
       else {
         analogWrite(motorRightPWM, 255); //move at the normal max speed
         analogWrite(motorLeftPWM, 255);
       }     
  }
}

//set tires to go forwards
void forward() {
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, HIGH);
  delay(100);
}

//set tires to go backwards
void backward() {
  digitalWrite(motorRightDirection, LOW);
  digitalWrite(motorLeftDirection, LOW);
  delay(100);
}

//stop the robot
void stop_robot() {
  analogWrite(motorRightPWM, 0); 
  analogWrite(motorLeftPWM, 0);
  delay(100);   
}

//change left wheel directions to go backwards, move left wheels until robot
//has turned to the left
void turnleft() {
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, LOW);
  analogWrite(motorRightPWM, 200); //start turning again
  analogWrite(motorLeftPWM, 200);
  delay(turntimeninety);
  stop_robot();
  delay(100);
}

//void turnright() {
//  //left wheel goes forward
//  //right wheel directions goes backward
//  //
//  //has turned to the right
//}


  
