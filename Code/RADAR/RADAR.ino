#include <Servo.h>
Servo myservo;

const int tringPin = 12;
const int echoPin = 13;

volatile long duration, inches, cm;

int pos = 0;  

void sweep(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  
    sonicPulse();               // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);     
    sonicPulse();         // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}

void setup() { 
  Serial.begin(9600);
  myservo.attach(10);
  pinMode(tringPin, OUTPUT);
  pinMode(echoPin, INPUT);


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

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  // delay(10);
}
  
void loop() { 
  sweep();
  // pop();
}

long microsecondsToInches(long microseconds) { 
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) { 
  return microseconds / 29 / 2;
}
