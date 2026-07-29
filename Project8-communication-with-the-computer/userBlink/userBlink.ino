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
    MsTimer2::set(onTime, turn_off);
    MsTimer2::start();               
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
    if (onTime == 0) {
    Serial.println("Received 0");
    } 
    else {
      Serial.println("Received " + String(onTime));
      Serial.println("Waiting 5 s for a button press...");
      delay(5000);
    }
  }
  // else {
  //   Serial.println("No input");
  // }
}
