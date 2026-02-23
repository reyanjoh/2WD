
#include <Servo.h>
Servo myservo;


unsigned long startTime;
volatile bool running = true;

volatile int D_mix;
volatile int D_mid;
volatile int D_max;
volatile int Front_Distance;
volatile int Left_Distance;
volatile int Right_Distance;
volatile int Right_IR_Value;
volatile int Left_IR_Value;


volatile long duration, inches, cm;

int pos = 0;  

const int MIN_DIST = 5;
const int MAX_DIST = 50;

const int ALERT_DIST_CM = 30;

unsigned long lastBeepTime = 0;
long beepInterval = 500;


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

void Detect_Left_and_Right__distance() {
  myservo.write(165);
  delay(500);
  Serial.println(Left_Distance);
  delay(100);
  Left_Distance = checkdistance();
  myservo.write(15);
  delay(500);
  Serial.println(Right_Distance);
  delay(100);
  Right_Distance = checkdistance();
  myservo.write(90);
}

void Ultrasonic_obstacle_avoidance() {
  if (Front_Distance <= D_mid) {
    digitalWrite(2,LOW);
    analogWrite(5,0);
    digitalWrite(4,HIGH);
    analogWrite(6,0);
    if (Front_Distance <= D_mix || Left_IR_Value == 0 && Right_IR_Value == 0) {
      digitalWrite(2,LOW);
      analogWrite(5,(4.5 * 22.5));
      digitalWrite(4,HIGH);
      analogWrite(6,(4.5 * 22.5));
      delay(300);
      digitalWrite(2,LOW);
      analogWrite(5,0);
      digitalWrite(4,HIGH);
      analogWrite(6,0);

    }
    Detect_Left_and_Right__distance();
    if ((D_mix < Left_Distance && Left_Distance < D_max) && (D_mix < Right_Distance && Right_Distance < D_max)) {
      if (Right_Distance > Left_Distance) {
        digitalWrite(2,HIGH);
        analogWrite(5,(9 * 22.5));
        digitalWrite(4,HIGH);
        analogWrite(6,(9 * 22.5));
        delay(150);

      } else {
        digitalWrite(2,LOW);
        analogWrite(5,(9 * 22.5));
        digitalWrite(4,LOW);
        analogWrite(6,(9 * 22.5));
        delay(150);

      }

    } else if (D_mix < Left_Distance && Left_Distance < D_max || D_mix < Right_Distance && Right_Distance < D_max) {
      if (D_mix < Left_Distance && Left_Distance < D_max) {
        digitalWrite(2,LOW);
        analogWrite(5,(7 * 22.5));
        digitalWrite(4,LOW);
        analogWrite(6,(7 * 22.5));
        delay(150);

      } else if (D_mix < Right_Distance && Right_Distance < D_max) {
        digitalWrite(2,HIGH);
        analogWrite(5,(7 * 22.5));
        digitalWrite(4,HIGH);
        analogWrite(6,(7 * 22.5));
        delay(150);
      }
    } else if (Right_Distance < D_mix && Left_Distance < D_mix) {
      digitalWrite(2,HIGH);
      analogWrite(5,0);
      digitalWrite(4,LOW);
      analogWrite(6,(9 * 22.5));
      delay(250);
      digitalWrite(2,LOW);
      analogWrite(5,0);
      digitalWrite(4,HIGH);
      analogWrite(6,0);
    }
    digitalWrite(2,LOW);
    analogWrite(5,0);
    digitalWrite(4,HIGH);
    analogWrite(6,0);

  }
}

void Obstacle_Avoidance_Main() {
  Left_IR_Value = digitalRead(A1);
  Right_IR_Value = digitalRead(A2);
  Front_Distance = checkdistance();
  Serial.println(Front_Distance);
  Infrared_Obstacle_Avoidance();
  Ultrasonic_obstacle_avoidance();
}

void Infrared_Obstacle_Avoidance() {
  if (Left_IR_Value == 0 && Right_IR_Value == 1) {
    digitalWrite(2,HIGH);
    analogWrite(5,90);
    digitalWrite(4,LOW);
    analogWrite(6,15);

  } else if (Left_IR_Value == 1 && Right_IR_Value == 0) {
    digitalWrite(2,HIGH);
    analogWrite(5,15);
    digitalWrite(4,LOW);
    analogWrite(6,90);
  } else {
    digitalWrite(2,HIGH);
    analogWrite(5,(4 * 22.5));
    digitalWrite(4,LOW);
    analogWrite(6,(4 * 22.5));

  }
}


void Infrared_Tracing() {
  int Left_Tra_Value = 1;
  int Center_Tra_Value = 1;
  int Right_Tra_Value = 1;
  int Black = 1;
  Left_Tra_Value = digitalRead(7);
  Right_Tra_Value = digitalRead(9);
  if (Left_Tra_Value != Black && Right_Tra_Value != Black) {
    digitalWrite(2,HIGH);
    analogWrite(5,50);
    digitalWrite(4,LOW);
    analogWrite(6,50);
  }
  else if (Left_Tra_Value == Black && Right_Tra_Value != Black) 
  {
    digitalWrite(2,LOW);
    analogWrite(5,60);
    digitalWrite(4,LOW);
    analogWrite(6,60);
  }
  else if (Left_Tra_Value != Black && Right_Tra_Value == Black) 
  {
    digitalWrite(2,HIGH);
    analogWrite(5,60);
    digitalWrite(4,HIGH);
    analogWrite(6,60);
  } 
  else if (Left_Tra_Value == Black && Right_Tra_Value == Black) 
  {
    digitalWrite(2,LOW);
    analogWrite(5,0);
    digitalWrite(4,HIGH);
    analogWrite(6,0);
  }
}

void stopMotors() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);

  digitalWrite(4, LOW);
  analogWrite(6, 0);
}


void sweep(){
  stopMotors();

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  
    sonicPulse();   
    nonBlockingBeep();            // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);     
    sonicPulse();     
    nonBlockingBeep();    // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  myservo.write(90);  
}


void sonicPulse() {
  digitalWrite(12, LOW);
  delayMicroseconds(2);
  digitalWrite(12, HIGH);
  delayMicroseconds(10);
  digitalWrite(12, LOW);

  duration = pulseIn(13, HIGH);
  cm = microsecondsToCentimeters(duration);

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  // Only beep if object is <= ALERT_DIST_CM
  if (cm <= ALERT_DIST_CM) {
    // Closer objects → faster beep
    beepInterval = map(cm, 0, ALERT_DIST_CM, 50, 100);
    beepInterval = constrain(beepInterval, 50, 100);
  } else {
    beepInterval = 0;  // no beep
    digitalWrite(A0, LOW);
    noTone(A3);
  }
}



long microsecondsToInches(long microseconds) { 
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) { 
  return microseconds / 29 / 2;
}




void setup(){
  Serial.begin(9600);

  myservo.attach(10);
  D_mix = 5;
  D_mid = 10;
  D_max = 400;
  Front_Distance = 0;
  Left_Distance = 0;
  Right_Distance = 0;
  Right_IR_Value = 1;
  Left_IR_Value = 1;
  myservo.write(90);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(9, INPUT);  
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);
  

  startTime = millis();


}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - startTime;

  if (elapsed < 10000) { 
    Infrared_Tracing();
    Obstacle_Avoidance_Main();
  }
  else if (elapsed < 20000) { 
    sweep();
  }
  else { 
    startTime = currentTime;
  }
}

// long microsecondsToCentimeters(long microseconds) {
//   return microseconds / 29 / 2;
// }

// Non-blocking LED + buzzer beep
void nonBlockingBeep() {
  if (beepInterval == 0) return; // no object in range

  unsigned long currentMillis = millis();
  if (currentMillis - lastBeepTime >= beepInterval) {
    lastBeepTime = currentMillis;

    // Toggle LED and buzzer
    static bool state = false;
    state = !state;
    digitalWrite(A0, state ? HIGH : LOW);

    if (state) {
      tone(A3, 2000);
    } else {
      noTone(A3);
    }
  }
}

// void loop(){
 
//    if (millis() - startTime < 10000) {
//     Infrared_Tracing();
//     Obstacle_Avoidance_Main();
//   } else if(millis() - startTime < 10000) {
//      sweep();
//   }

// }
