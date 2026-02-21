#include <Servo.h>

Servo myservo;

// -------- Pins --------
#define TRIG 12
#define ECHO 13

#define LEFT_LINE 7
#define RIGHT_LINE 9

#define ENA 5
#define ENB 6
#define IN1 2
#define IN2 4

// -------- Constants --------
#define ONE_FOOT 30      // 30 cm
#define THREE_INCH 8     // 8 cm

// -------- Ultrasonic --------
float checkdistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) / 58.0;
}

// -------- Motor Controls --------
void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void forward(int spd) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void turnLeft(int spd) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void turnRight(int spd) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

// -------- Line Detection --------
bool scanForLine() {
  unsigned long t = millis();
  while (millis() - t < 5000) {
    if (digitalRead(LEFT_LINE) == 1 || digitalRead(RIGHT_LINE) == 1) {
      return true;
    }
  }
  return false;
}

// -------- Line Tracing --------
void lineTraceWithObstacle() {
  unsigned long t = millis();

  while (millis() - t < 20000) {

    float dist = checkdistance();

    if (dist <= ONE_FOOT) {
      stopMotors(); // stop if obstacle within 1 foot
      continue;
    }

    int left = digitalRead(LEFT_LINE);
    int right = digitalRead(RIGHT_LINE);
    int black = 1;

    if (left != black && right != black) {
      forward(70);
    }
    else if (left == black && right != black) {
      turnLeft(80);
    }
    else if (left != black && right == black) {
      turnRight(80);
    }
    else {
      stopMotors();
    }
  }
}

// -------- Roaming Mode (1 min) --------
void roamAvoidObstacle() {
  unsigned long t = millis();

  while (millis() - t < 60000) {

    float dist = checkdistance();

    if (dist <= THREE_INCH) {
      stopMotors();
      delay(200);

      // quick scan left & right
      myservo.write(160);
      delay(300);
      float leftD = checkdistance();

      myservo.write(20);
      delay(300);
      float rightD = checkdistance();

      myservo.write(90);

      if (leftD > rightD) {
        turnLeft(90);
      } else {
        turnRight(90);
      }
      delay(400);
    }
    else {
      forward(70);
    }
  }
}

// -------- Object Tracking --------
void trackObject() {

  while (true) {

    float closest = 1000;
    int bestAngle = 90;

    for (int angle = 0; angle <= 180; angle += 10) {
      myservo.write(angle);
      delay(150);
      float d = checkdistance();

      if (d <= ONE_FOOT && d < closest) {
        closest = d;
        bestAngle = angle;
      }
    }

    if (closest <= ONE_FOOT) {
      myservo.write(bestAngle);
    } else {
      myservo.write(90);
    }
  }
}

// -------- Setup --------
void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LEFT_LINE, INPUT);
  pinMode(RIGHT_LINE, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  myservo.attach(10);
  myservo.write(90);

  stopMotors();
}

// -------- Main --------
void loop() {

  bool lineFound = scanForLine();

  if (lineFound) {
    lineTraceWithObstacle();
    stopMotors();
    trackObject(); // final stage
  }
  else {
    roamAvoidObstacle();
    stopMotors();
  }

  while (1); // end program
}
