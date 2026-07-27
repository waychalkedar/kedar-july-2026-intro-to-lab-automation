int BUTTON_PIN = 6;
int LED_PIN = 4;
int INTERRUPT_PIN = 2;
long START_TIME = 0;

// void led() {
//   if (digitalRead(BUTTON_PIN)) {
//     MsTimer2::start();
//     MsTimer2::set(5000, flash);
//   } 
// }


void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), led, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BUTTON_PIN)) {
    START_TIME = millis();
    digitalWrite(LED_PIN, HIGH);
  }
  if (millis() - START_TIME > 5000) {
    digitalWrite(LED_PIN, LOW);
  }
}
