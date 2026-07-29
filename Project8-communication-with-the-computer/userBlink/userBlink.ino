#include <MsTimer2.h>

const int LED_PIN = 4;
const int BUTTON_PIN = 6;
const int INTERRUPT_PIN = 3;
int valueRead;
int onTime;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), button, CHANGE);
}

void button() {
  if (digitalRead(BUTTON_PIN)) {
    MsTimer2::set(onTime + 1, turn_off); // bug seen previously where the stop() would end early
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
    valueRead = Serial.parseInt();
    if (valueRead != 0) {
      onTime = valueRead; // doing it this way stores the last inputted value and leaves the button responsive
      Serial.println("Received " + String(onTime));
    } 
  }
}
