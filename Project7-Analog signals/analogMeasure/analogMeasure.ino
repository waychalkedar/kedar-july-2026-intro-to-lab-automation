const int PIN_1 = 7;
const int PIN_2 = A0;
const int PIN_3 = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_1, HIGH);
  analogRead(PIN_2);
  // analogWrite(PIN_3, 500);
  digitalWrite(PIN_1, LOW);
  // digitalWrite(PIN_1, HIGH);
  // digitalWrite(PIN_1, LOW);
}
