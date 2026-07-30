void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int angle = 179;
  bool buzzer = true;
  Serial.write(angle);
  Serial.write('A');
  Serial.write(buzzer);
}
