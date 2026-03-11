#include <Servo.h>

Servo myservo;

const int tringPin = 12;
const int echoPin = 13;


// volatile int pos = 0;
volatile int servoPos = 0;
bool servoReverse = false;

unsigned long startTime = 0;
volatile long duration, inches, cm;

bool obstacleDetected = false;
int obstacleDetectionDistance = 15;



long microsecondsToInches(long microseconds) { 
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) { 
  return microseconds / 29 / 2;
}

void servoSweep() {

  if (obstacleDetected) {
    Serial.println("intruder!");
    return;  
  }

  myservo.write(servoPos);
  Serial.println(servoPos);
  sonicPulse();
  delay(15);

  if (!servoReverse) {
    servoPos++;
    if (servoPos >= 180) {
      servoPos = 180;
      servoReverse = true;
    }
  } 
  else {
    servoPos--;
    if (servoPos <= 0) {
      servoPos = 0;
      servoReverse = false;
    }
  }
}

void sonicPulse(){

  digitalWrite(tringPin, LOW);
  delayMicroseconds(2);
  digitalWrite(tringPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tringPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
 
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  if(cm <= obstacleDetectionDistance){
    obstacleDetected=true;
    Serial.println("intruder!");
  }

  // Serial.print(inches);
  // Serial.print("in, ");
  // Serial.print(cm);
  // Serial.print("cm");
  // Serial.println(obstacleDetected);

  // delay(10);
} 


float checkdistance() {
  digitalWrite(12, LOW);
  delayMicroseconds(2);
  digitalWrite(12, HIGH);
  delayMicroseconds(10);
  digitalWrite(12, LOW);
  float distance = pulseIn(13, HIGH) / 58.00;
  delay(10);
  return distance;
}

void Ultrasonic_Sensor_Module() {
  int Distance = 0;
  Distance = checkdistance();
  stopMotors();
  alarm();
  if(Distance > obstacleDetectionDistance){
    obstacleDetected = false;
    stopAlarm();
  }
  
  // Serial.print("Distance:");
  // Serial.print(Distance);
  // Serial.println("CM");
  // Serial.println(Distance);

  // Serial.println(obstacleDetected);
  delay(100);
}

void Infrared_Tracing() {

  int Left_Tra_Value  = digitalRead(7);
  int Right_Tra_Value = digitalRead(9);
  int Black = 1;

  int baseSpeed     = 40;
  int reverseSpeed  = 40;
  int turnRate      = 40;

  if (Left_Tra_Value != Black && Right_Tra_Value != Black) {
    digitalWrite(2, HIGH);
    analogWrite(5, baseSpeed);

    digitalWrite(4, LOW);
    analogWrite(6, baseSpeed);
  }

  else if (Left_Tra_Value == Black && Right_Tra_Value != Black) {

    digitalWrite(2, LOW);
    analogWrite(5, turnRate);

    digitalWrite(4, LOW);
    analogWrite(6, baseSpeed);
  }

  else if (Left_Tra_Value != Black && Right_Tra_Value == Black) {

    digitalWrite(2, HIGH);
    analogWrite(5, baseSpeed);

    digitalWrite(4, HIGH);
    analogWrite(6, turnRate);
  }

  else if (Left_Tra_Value == Black && Right_Tra_Value == Black) {

    digitalWrite(2, HIGH);
    analogWrite(5, baseSpeed);

    digitalWrite(4, HIGH);
    analogWrite(6, reverseSpeed);
  }
}


void pirDetection(){
  int motionState = digitalRead(11);
  myservo.write(90);
  

  if (motionState == HIGH) {
    alarm();
    // Serial.println("Motion Detected!");
    Serial.println("intruder!");
  } else if(motionState == LOW) {
    stopAlarm();
    Serial.println("0");
  }

}

void stopMotors() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);

  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

void alarm(){
  digitalWrite(A0, HIGH);
  tone(A3, 2000);
  delay(500);
  stopAlarm();
}

void stopAlarm() {
  digitalWrite(A0, LOW);
  noTone(A3);
}




void setup() {
 
  Serial.begin(9600);

  myservo.attach(10);

  startTime = millis();
  pinMode(7, INPUT);
  pinMode(9, INPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);

  
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - startTime;

  if (elapsed < 20000) { 
    if(!obstacleDetected){
      servoSweep();
      Infrared_Tracing();
    }else{
      Ultrasonic_Sensor_Module();
      
    }
  }
  else if(elapsed < 30000) { 
    stopMotors();
    pirDetection();
  }else{
    stopAlarm();
    startTime = currentTime;
  }

}
