#include <MsTimer2.h>

const int LED_PIN = 4;
const int BUTTON_PIN = 6;
const int INTERRUPT_PIN = 3;
int onTime;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), button, CHANGE);
}

void button() {
  if (digitalRead(BUTTON_PIN)) {               
    digitalWrite(LED_PIN, HIGH);
  }
}

void turn_off() {
  digitalWrite(LED_PIN, LOW);
  MsTimer2::stop();
}

void loop() {
  // polls repeatedly to check Serial
  if (Serial.available()) { 
    onTime = Serial.parseInt();
    Serial.println("Received " + String(onTime));
    MsTimer2::set(onTime, turn_off);

  }
}
