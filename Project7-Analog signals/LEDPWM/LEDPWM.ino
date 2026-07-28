#include <MD_PWM.h>

const int ROTARY_PIN = A0;
const int LED_PIN = 4;

const int PWM_PINS[] = { LED_PIN };
const int PIN_COUNT = sizeof(PWM_PINS) / sizeof(PWM_PINS[0]);

const int PWM_FREQ = 100;

// MD_PWM LED_PWM(PWM_FREQ, PWM_PINS, PIN_COUNT);
MD_PWM LED_PWM = MD_PWM(LED_PIN); // creating a specific instance for the LED_PIN of the MD_PWM class
                                  // using a constructor called MD_PWM()


void setup() {
  // put your setup code here, to run once:
  pinMode(ROTARY_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  LED_PWM.begin();                // calling the begin() method on the specific instance
}

void loop() {
  // put your main code here, to run repeatedly:
  LED_PWM.write(analogRead(ROTARY_PIN) / 4);
}
