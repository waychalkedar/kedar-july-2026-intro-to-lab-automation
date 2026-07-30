// modified with Copilot

#include <Servo.h>
#include "Arduino_SensorKit.h"

#define BUZZER 5
const int SERVO_PIN = 7;
float valueX;
int angle;

Servo servo1;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600);
  servo1.attach(SERVO_PIN);
  pinMode(BUZZER, OUTPUT);
  while(!Serial);
  
  Accelerometer.begin();
}
 
void loop() {
  // Read accelerometer axis readings
  valueX = Accelerometer.readX();
  // Map the raw X value to a servo angle.
  // Adjust min/max values for your sensor orientation.
  float mappedAngle = mapFloat(valueX, -1.0, 1.0, 0.0, 180.0);
  // angle = constrain((int)mappedAngle, 0, 180);
  // servo1.write(angle);
  if ((valueX < -0.9) | (valueX > 0.9)){
    tone(BUZZER, 440);
  }
  else {
    noTone(BUZZER);
    angle = constrain((int)mappedAngle, 0, 180);
    servo1.write(angle);
  }
  delay(100);
}