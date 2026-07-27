#include <MsTimer2.h>

const int BUTTON_PIN = 6; // using const puts the variables into static memory, 
                          // freeing up the Arduino's precious 2kB SRAM
const int LED_PIN = 4;
const int INTERRUPT_PIN = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), led, CHANGE);
  Serial.begin(9600);
}

void led() {
  if (digitalRead(BUTTON_PIN)) {
    MsTimer2::set(30, dim);       // configure timer first
    MsTimer2::start();              // then start
    digitalWrite(LED_PIN, HIGH);
    // now after 30 ms, dim will be called.
    // without stop, dim would be called every 30 ms
  }
  // you only want to turn the LED off when the timer expires, so no else block
}

void dim() {
  digitalWrite(LED_PIN, LOW);
  MsTimer2::stop();               // stop instead of looping
}

void loop() {
  for (int i = 0; i< 10000; i++) {
  Serial.println("calculating " + String(i) + " ...");
  } 
}