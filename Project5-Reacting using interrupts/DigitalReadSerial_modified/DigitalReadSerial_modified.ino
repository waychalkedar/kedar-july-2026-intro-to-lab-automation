
// digital pin 6 has a pushbutton attached to it. Give it a name:
int pushButton = 6;
int LED = 4;
int interrupt = 8; // interrupt pin, will kill the for loop
bool SWITCH = true;

// the setup routine runs once when you press reset:
void setup() {
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.begin(230400);
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // (buttonState) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
  if(buttonState) {
    digitalWrite(LED, HIGH);
    if (SWITCH) {
      Serial.println("Button pressed");
      SWITCH = !SWITCH;
    }
  }
  else {
    digitalWrite(LED, LOW);
    if (!SWITCH) {
      Serial.println("Button released");
      SWITCH = !SWITCH;
    }
  }
  for (int i = 0; i< 10000; i++){
    Serial.println("calculating...");
    if (digitalRead(interrupt) == HIGH) {
      break;
    }
  } 
  delay(1);  // delay in between reads for stability
}
