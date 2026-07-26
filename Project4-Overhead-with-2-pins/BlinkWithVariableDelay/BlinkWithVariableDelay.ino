/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

int LED_1 = 12; // channel 0 on the scope
int LED_2 = 13; // channel 1 on the scope
int SUM = 0;
int DELAY = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  // pinMode(LED_1, OUTPUT);
  // pinMode(LED_2, OUTPUT);
  DDRB |= (1 << DDB4) | (1 << DDB5); // using port registers, set pins 12 and 13 as outputs
}

// the loop function runs over and over again forever
void loop() {
  // digitalWrite(LED_1, HIGH);
  // digitalWrite(LED_2, HIGH);
  PORTB |= (1 << PB4) | (1 << PB5);  // using port registers, turn pins 12 and 13 HIGH together
  delay(DELAY);         
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  delay(DELAY);             
}
