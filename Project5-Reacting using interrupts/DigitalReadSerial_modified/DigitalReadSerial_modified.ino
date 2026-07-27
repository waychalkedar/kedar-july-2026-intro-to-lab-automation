
// digital pin 6 has a pushbutton attached to it. Give it a name:
int pushButton = 6;
int LEDPin = 4;
int interruptPin = 2; // interrupt pin, will act as an event
                      // for the UNO, pins 2 and 3 can be used
bool SWITCH = true;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), button, CHANGE);
}

// the loop routine runs over and over again forever:
void loop() {
  // digitalWrite(LEDPin, HIGH);
  // byte buttonState = digitalRead(pushButton);
  // (buttonState) ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
  for (int i = 0; i< 1000; i++){
    Serial.println("calculating " + String(i) + " ...");
  } 
  delay(1000); 
}

void button() {
  // read the input pin: 
  byte buttonState = digitalRead(pushButton);
  digitalWrite(LEDPin, buttonState);
  // if(!buttonState) {
  //   digitalWrite(LED_PIN, HIGH);
    // not recommended to put serial prints
    // if (SWITCH) {
    //   Serial.println("Button released");
    //   SWITCH = !SWITCH;
    // }
  // }
  // else {
    // digitalWrite(LED_PIN, LOW);
    // if (!SWITCH) {
    //   Serial.println("Button pressed");
    //   SWITCH = !SWITCH;
    // }
  // }
  // delay(5000); // doesn't work inside the interrupt service routine
}