unsigned long startTime = 0;

void Infrared_Tracing() {
  int Left_Tra_Value = 1;
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

void pirDetection(){
  int motionState = digitalRead(11);

  if (motionState == HIGH) {
    digitalWrite(A0, HIGH);
    tone(A3, 2000);
    Serial.println("Motion Detected!");
  } else {
    digitalWrite(A0, LOW);
    noTone(A3);
  }

  // delay(100);
}

void setup(){

  Serial.begin(9600);

  startTime = millis();
  pinMode(7, INPUT);
  pinMode(9, INPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);
}

void stopMotors() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);

  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

void loop(){
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - startTime;

  
  if (elapsed < 10000) { 
    Infrared_Tracing();
  }
  else { 
    stopMotors();
    pirDetection();
  }
  
}
