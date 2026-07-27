const int BUTTON_PIN = 6;
const int LED_PIN = 4;
const int INTERRUPT_PIN = 2;
unsigned long ul_startTime = 0; // good practice to hint at the variable type in the name

// void led() {
//   if (digitalRead(BUTTON_PIN)) {
//     START_TIME = millis();
//     digitalWrite(LED_PIN, HIGH);
//   }
//   if (millis() - START_TIME > 5000) {
//     digitalWrite(LED_PIN, LOW);
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
    ul_startTime = millis();
    digitalWrite(LED_PIN, HIGH);
  }
  if (millis() - ul_startTime > 5000) {
    digitalWrite(LED_PIN, LOW);
  }

  // for (int i = 0; i< 10000; i++) {
  // Serial.println("calculating " + String(i) + " ...");
  // } 
}
