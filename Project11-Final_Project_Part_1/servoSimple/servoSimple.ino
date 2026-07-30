#include <Servo.h>

Servo servo1;

const int SERVO_PIN = 7;
const int KNOB_PIN = A0;

int value;

void setup() {
  pinMode(KNOB_PIN, INPUT);
  servo1.attach(SERVO_PIN);  
}

void loop() {
  value = analogRead(KNOB_PIN);
  value = map(value, 0, 1023, 0, 180);
  servo1.write(value);
  delay(15);
}
