// GLOBAL VARIABLES
unsigned long prevTime = 2000;
unsigned long currentTime;
int count = 0;
float dist_ahead;

// PIN ASSIGNMENTS AND OTHER CONSTANTS
const int motorRightDirection = 4;
const int motorRightPWM = 5;
const int motorLeftPWM = 6;
const int motorLeftDirection = 7;
const int trig = 2;
const int echo = 3;
const int temp = 0; //Analog pin

const int FULL_SPEED = 255;
const int MIN_SPEED = 170;
const int STOP_SPEED = 0;

const int SLOW_DIST = 40; //distance to start slowing down is 40 cm
const int STOP_DIST = 6; //distance to stop is 6 cm FOR NOW, (improve later)

const int TURN_90_TIME = 1000;
const int LOOP_DELAY = 1000;

void setup(){
  //set sensor modes
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);

  analogReference(INTERNAL); //change aRef to ~1.1V

  //set up pin modes
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(motorLeftDirection, OUTPUT);

  setMotorsForward(); // initialize to forward, but dont drive anywhere yet
  setMotorSpeed(STOP_SPEED);

  Serial.begin(9600);
}

void loop(){ 
//BASIC MOTOR TEST
/* setMotorsForward();
  setMotorSpeed(FULL_SPEED);
  delay(1000);
  setMotorSpeed(STOP_SPEED);
  delay(1000);
  turnRight();
  delay(1000);
  turnLeft();
  delay(1000);
  */
         
  currentTime = millis();    //take the current time for the next calculation
  if(currentTime - prevTime > LOOP_DELAY){   //process new instruction every ONE SECOND
    prevTime = currentTime;
    dist_ahead = getDistance();   // Read Ultrasonic sensor and calculate distance
    
    if(dist_ahead <= STOP_DIST) {  // At STOP_DIST,(6cm), and turn left
      turnLeft();
    } else if(dist_ahead <= SLOW_DIST) { // Slow down to MIN_SPEED
      slowDown(dist_ahead);
    } else {                   // Otherwise keep driving full speed
      setMotorSpeed(FULL_SPEED);
    }
    
    // Print distance reading  to the serial monitor    TODO: print to LCD
    Serial.print(" |Distance: ");
    Serial.print(dist_ahead);
    Serial.println(" cm|");
  }
}

void setMotorsForward() {   //Configure motor settings, doesnt actually drive
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, HIGH);
  delay(10); //delay to make sure the correct direction is going to proces
}

// CURRENTLY WE ARE NOT USING THIS MEHOD
//void setMotorsBackward() {
//  digitalWrite(motorLeftDirection, LOW);
//  digitalWrite(motorRightDirection, LOW);
//  delay(100); //delay to make sure the correct direction is going to process
//}

void setMotorSpeed( int pwm_speed){
  analogWrite(motorLeftPWM, pwm_speed);
  analogWrite(motorRightPWM, pwm_speed); //start turning again  
  Serial.print(" |Motor Speed: ");      // print motor speed every time we set it 
  Serial.print(pwm_speed);
  Serial.print("|");
  delay(10);
}

void turnLeft() {
  setMotorSpeed(STOP_SPEED);
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, LOW);
  setMotorSpeed(FULL_SPEED);
  delay(TURN_90_TIME);
  setMotorSpeed(STOP_SPEED);
}

// CURRENTLY WE ARE NOT USING THIS MEHOD
//void turnRight() {
//  digitalWrite(motorRightDirection, LOW);
//  digitalWrite(motorLeftDirection, HIGH);
//  setMotorSpeed(FULL_SPEED);
//  delay(TURN_90_TIME);
//  setMotorSpeed(STOP_SPEED);
//}


void slowDown(float dist) {
  float slowrate_f = ((dist-STOP_DIST)/(SLOW_DIST-STOP_DIST)) * 255;
  int slowrate_i = (int)slowrate_f;  
  
  if(slowrate_i < MIN_SPEED)  //THE END OF THE BEEPING
    slowrate_i = MIN_SPEED;
 
  setMotorSpeed(slowrate_i);
}


float getDistance() {
  float echotime; //time of pulse in microseconds
  float temperature; //temperature from IC sensor in degree Celsius
  float speedSound; //calculated speed of sound using temperature from IC sensor
  float distance; //distance in cm
  
  temperature = analogRead(temp)/9.31;       //read temperature from IC sensor(in Celsius);
  speedSound = 331.5 + (0.6 * temperature);  //calculate speed of sound with temperature in m/s
  speedSound = 10000/speedSound;             //convert speed of sound to us/cm
   
  digitalWrite(trig, LOW);      //10us pulse to trigger
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
   
  echotime = pulseIn(echo, HIGH); //waits for echo to go HIGH, starts timer, stops timer when echo goes LOW
   
  //calculate distance in cm
  if (echo > 38000) { //if the distance is too far, just set distance to 0.
    distance = 0;
  } else {
    distance = echotime/speedSound/2;
  }
  
  return distance;
}
