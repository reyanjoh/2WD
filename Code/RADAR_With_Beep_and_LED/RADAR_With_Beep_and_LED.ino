#include <Servo.h>

#define TRIG_PIN 12
#define ECHO_PIN 13
#define SERVO_PIN 10
#define LED_PIN A0
#define BUZZER_PIN A3

Servo myservo;
int pos = 0;
long duration, cm;

// Minimum and maximum distance thresholds (cm)
const int MIN_DIST = 5;
const int MAX_DIST = 50;

const int ALERT_DIST_CM = 15;

unsigned long lastBeepTime = 0;
long beepInterval = 500; // default interval

void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Sweep servo smoothly 0 -> 180
  for (pos = 0; pos <= 180; pos++) {
    myservo.write(pos);
    sonicPulse();
    nonBlockingBeep();
    delay(15); // smooth servo movement
  }

  // Sweep servo smoothly 180 -> 0
  for (pos = 180; pos >= 0; pos--) {
    myservo.write(pos);
    sonicPulse();
    nonBlockingBeep();
    delay(15);
  }
}

// Measure distance from ultrasonic sensor


void sonicPulse() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
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
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}
// void sonicPulse() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   duration = pulseIn(ECHO_PIN, HIGH);
//   cm = microsecondsToCentimeters(duration);

//   Serial.print("Distance: ");
//   Serial.print(cm);
//   Serial.println(" cm");

//   // Update beep interval based on distance
//   if (cm <= MAX_DIST) {
//     beepInterval = map(cm, MIN_DIST, MAX_DIST, 50, 500);
//     beepInterval = constrain(beepInterval, 50, 500);
//   } else {
//     beepInterval = 0; // no beep
//     digitalWrite(LED_PIN, LOW);
//     noTone(BUZZER_PIN);
//   }
// }

// Convert microseconds to centimeters
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

// Non-blocking LED + buzzer beep
void nonBlockingBeep() {
  if (beepInterval == 0) return; // no object in range

  unsigned long currentMillis = millis();
  if (currentMillis - lastBeepTime >= beepInterval) {
    lastBeepTime = currentMillis;

    // Toggle LED and buzzer
    static bool state = false;
    state = !state;
    digitalWrite(LED_PIN, state ? HIGH : LOW);

    if (state) {
      tone(BUZZER_PIN, 2000);
    } else {
      noTone(BUZZER_PIN);
    }
  }
}