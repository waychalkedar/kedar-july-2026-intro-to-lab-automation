// Written with Copilot

/*
  userBlink_serial.ino

  Serial-based Arduino sketch (placed in the Firmata-approach folder per request).

  Same behavior as the Project8 serial example: reads newline-terminated timeout (ms)
  from Serial, uses an interrupt to detect a button on pin 6, turns LED on pin 4 on press,
  uses MsTimer2 to turn it off after the configured timeout, and sends simple state codes
  over Serial: 1 (pressed & LED ON), 2 (released), 0 (LED OFF).

  Wiring notes: button to pin 6 and GND, enable INPUT_PULLUP in code. Interrupt pin 3.
*/

#include <MsTimer2.h>

const int LED_PIN = 4;
const int BUTTON_PIN = 6;
const int INTERRUPT_PIN = 3;
const unsigned long DEBOUNCE_MS = 50UL;

volatile unsigned long last_button_time = 0;
volatile uint8_t pending_state = 255;
volatile bool state_changed = false;

unsigned long on_time_ms = 1000UL;

void turn_off();
void handleButtonChange();

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleButtonChange, CHANGE);
  Serial.println("Ready");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() > 0) {
      long v = s.toInt();
      if (v > 0) {
        on_time_ms = (unsigned long)v;
        Serial.print("I received: ");
        Serial.println(on_time_ms);
      } else {
        Serial.print("Invalid timeout: ");
        Serial.println(s);
      }
    }
  }

  if (state_changed) {
    noInterrupts();
    uint8_t st = pending_state;
    pending_state = 255;
    state_changed = false;
    interrupts();

    if (st <= 2) {
      Serial.println(st);
    }
  }

  delay(5);
}

void handleButtonChange() {
  unsigned long now = millis();
  if ((now - last_button_time) < DEBOUNCE_MS) return;
  last_button_time = now;

  bool pressed = digitalRead(BUTTON_PIN) == LOW;
  if (pressed) {
    digitalWrite(LED_PIN, HIGH);
    MsTimer2::set(on_time_ms + 1, turn_off);
    MsTimer2::start();
    pending_state = 1;
    state_changed = true;
  } else {
    pending_state = 2;
    state_changed = true;
  }
}

void turn_off() {
  digitalWrite(LED_PIN, LOW);
  MsTimer2::stop();
  pending_state = 0;
  state_changed = true;
}
