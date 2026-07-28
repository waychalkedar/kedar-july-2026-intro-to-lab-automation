#include <Servo.h>

Servo servo1;

const int ROTARY_PIN = A0;
const int SERVO_PIN = 7;
int value;
int angle;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(SERVO_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(ROTARY_PIN);          // an integer between 0 to 1023, need to convert to degrees
  angle = map(value, 0, 1023, 0, 180);
  // angle = (value/1023) * 180;           // doesn't work due to integer division, x/1023 = 0 for all int x
                                           // can make it work with float but slower
  servo1.write(angle); 
}
