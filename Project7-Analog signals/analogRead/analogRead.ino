const int ROTARY_PIN = A0;
int rotaryValue = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(ROTARY_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  rotaryValue = analogRead(ROTARY_PIN);
  Serial.println(String(rotaryValue));
}
