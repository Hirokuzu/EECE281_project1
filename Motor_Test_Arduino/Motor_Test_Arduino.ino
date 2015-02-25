unsigned long currentTime;
int count = 0;

const int motorRightDirection = 4;
const int motorRightPWM = 5;
const int motorLeftPWM = 6;
const int motorLeftDirection = 7;

void setup(){
  //set up pin modes
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(motorLeftDirection, OUTPUT);
  
  //set motor directons
  digitalWrite(motorRightDirection, HIGH);
  digitalWrite(motorLeftDirection, HIGH);
}

void loop(){
  
  currentTime = millis(); //take the current time for the next calculation
  
  if(millis() - currentTime > 2000){ //every two seconds will change direction (which the following code controls)
    
    analogWrite(motorRightPWM, 0); //turn the motors off to make sure there isn't any cross currents from the board (could damage the electronics)
    analogWrite(motorLeftPWM, 0);
    
    delay(100); //wait so that they do in fact stop
    
    if(count % 2 == 0){ //lets it alternate direction, if it's odd it'll spin the low way, if high will spin the high way
      //set motor directons
      digitalWrite(motorRightDirection, HIGH);
      digitalWrite(motorLeftDirection, HIGH);
    } else {
      digitalWrite(motorRightDirection, LOW);
      digitalWrite(motorLeftDirection, LOW);
    }
    
    delay(100); //delay to make sure the correct direction is going to go
    
    //send the motor pwm signal
    analogWrite(motorRightPWM, 255); //start turning again
    analogWrite(motorLeftPWM, 255);
    
    ++count; //incriment my counter variable
  }
}
