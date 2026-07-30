#include <Servo.h>
#include "Arduino_SensorKit.h"
 
const int SERVO_PIN = 7;
int valueX;
int angle;

Servo servo1;

void setup() {
  Serial.begin(9600);
  servo1.attach(SERVO_PIN);
  while(!Serial);
  
  Accelerometer.begin();
}
 
void loop() {
  // Read X-axis reading
  
  valueX = Accelerometer.readX();
  Serial.print(valueX);
  Serial.println("");
  valueX = (valueX + 1) * 2.5;
  if ((valueX > 0) & (valueX < 5)){
    angle = analogRead(valueX);
    angle = map(valueX, -1, 1, 0, 180);
    Serial.print(angle);
    Serial.println("");
  }
  else {
    Serial.println("Out of range");
  }
  // valueY = Accelerometer.readY();
  // servo1.write(angle);
  delay(250);
}