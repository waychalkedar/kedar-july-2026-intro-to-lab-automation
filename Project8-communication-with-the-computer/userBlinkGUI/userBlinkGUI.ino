#include <MsTimer2.h>

const int LED_PIN = 4;
const int BUTTON_PIN = 6;
const int INTERRUPT_PIN = 3;
int valueRead;
int onTime;
byte state;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  // configuring interrupt pin for the button
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), button, CHANGE);
}

void button() {
  if (digitalRead(BUTTON_PIN)) {
    // configuring and starting timer using onTime read from Serial
    MsTimer2::set(onTime + 1, turn_off); // bug seen previously where the stop() would end early
    MsTimer2::start();               
    // turning on the LED
    digitalWrite(LED_PIN, HIGH);
    state = 1;
  }
}

void turn_off() {
  // turns off the LED
  digitalWrite(LED_PIN, LOW);
  // stops the timer and prevents looping
  MsTimer2::stop();
}

void loop() {
  // polls repeatedly to check Serial
  while (!Serial.available()) { 
    valueRead = Serial.readString().toInt();
    if (valueRead != 0) {
      // storing the read value in onTime
      // doing it this way stores the last inputted value and keeps the button responsive
      onTime = valueRead; 
      Serial.println("Received " + String(onTime));
    } 
  }
}
