#define LED_PIN A0
#define BUZZER_PIN A3


void buzzerLEDSyc(){
   // Turn on LED and buzzer
  digitalWrite(A0, HIGH);
  // digitalWrite(A3, HIGH);
  tone(A3, 2000);
  delay(500);

  // Turn off LED and buzzer
  digitalWrite(A0, LOW);
  // digitalWrite(A3, LOW);
  noTone(A3);
  delay(500);
}

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
 buzzerLEDSyc();
}