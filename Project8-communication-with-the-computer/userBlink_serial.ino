/*
  userBlink_serial.ino

  Arduino sketch for Project 8 (communication) — serial version.

  Functionality:
  - LED on pin 4 is turned ON when the button is pressed (button pin 6)
  - The host (Python GUI) sends a timeout in milliseconds over Serial (newline-terminated)
    which sets how long the LED remains ON after a button press.
  - The sketch uses an interrupt (pin 3) to detect button edge changes and a MsTimer2
    timer to turn the LED off after the configured timeout.
  - The sketch prints simple state codes to Serial so the GUI can update:
      "1" => button pressed and LED ON
      "2" => button released
      "0" => LED OFF (timer expired)
  - Uses simple debounce in the ISR and defers Serial output to the main loop.

  Wiring recommendation:
  - LED_PIN (4): connect LED (with resistor) to pin 4 and GND
  - BUTTON_PIN (6): connect one side of the button to pin 6 and the other side to GND
    (the sketch enables INTERNAL_PULLUP for stable input; pressed reads LOW)
  - INTERRUPT_PIN (3): must be an interrupt-capable pin and connected so that
    the button change triggers an interrupt (using the same button wiring as above)

  Requires: MsTimer2 library (install from Library Manager)
*/

#include <MsTimer2.h>

const int LED_PIN = 4;
const int BUTTON_PIN = 6;      // physical input pin for button
const int INTERRUPT_PIN = 3;  // interrupt pin number (use digitalPinToInterrupt(INTERRUPT_PIN))

const unsigned long DEBOUNCE_MS = 50UL;

volatile unsigned long last_button_time = 0;
volatile uint8_t pending_state = 255; // 0/1/2 values to send; 255 = none
volatile bool state_changed = false;

unsigned long on_time_ms = 1000UL; // default 1 second

// Forward declarations
void turn_off();
void handleButtonChange();

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Use internal pull-up for stable button reading. Button should pull pin to GND when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Attach interrupt to the physical interrupt pin; trigger on CHANGE to catch both press/release.
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleButtonChange, CHANGE);

  // Announce ready
  Serial.println("Ready");
}

void loop() {
  // Process any serial input: expecting newline-terminated integer representing milliseconds
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

  // Flush pending state changes (Serial cannot safely be used inside ISR)
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

  // Small idle delay to reduce CPU usage
  delay(5);
}

// ISR for button edge changes (must be short and fast)
void handleButtonChange() {
  unsigned long now = millis();
  if ((now - last_button_time) < DEBOUNCE_MS) {
    return; // ignore bounces
  }
  last_button_time = now;

  // Read the button state (pressed = LOW because of INPUT_PULLUP)
  bool pressed = digitalRead(BUTTON_PIN) == LOW;

  if (pressed) {
    // Turn on LED and start (or restart) timer to turn it off after on_time_ms
    digitalWrite(LED_PIN, HIGH);
    MsTimer2::set(on_time_ms + 1, turn_off); // add +1ms as precaution
    MsTimer2::start();

    // Notify main loop to send '1'
    pending_state = 1;
    state_changed = true;
  } else {
    // Button released: we notify '2' but do not turn LED off here (timer controls off)
    pending_state = 2;
    state_changed = true;
  }
}

// Called by MsTimer2 when the timeout expires
void turn_off() {
  digitalWrite(LED_PIN, LOW);
  MsTimer2::stop();

  // Notify main loop to send '0' (LED off)
  pending_state = 0;
  state_changed = true;
}
