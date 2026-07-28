const int ROTARY_PIN = A0;
const int PWM_PIN = 10;   // PWM-capable pin that will be shorted to the LED
const int LED_PIN = 4;    // LED pin that will receive the PWM pulse

// int rotaryValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ROTARY_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(LED_PIN, INPUT);    // because the LED pin receives the PWM wave, it should be an input
                              // bad things happen if you have two outputs shorted together
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(PWM_PIN, analogRead(ROTARY_PIN) / 4); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}
