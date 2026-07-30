const int FAN_PIN = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
  // Ramp up speed
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(FAN_PIN, speed);
    delay(15);
  }
  
  delay(1000); // Wait at max speed
  
  // Ramp down speed
  for (int speed = 255; speed >= 0; speed--) {
    analogWrite(FAN_PIN, speed);
    delay(15);
  }
  
  delay(1000); // Wait at off state
}
