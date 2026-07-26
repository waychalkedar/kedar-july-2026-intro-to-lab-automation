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
  DDRB |= B00110000; // using port registers, set pins 12 and 13 as outputs
  // we don't want to change the configuration of other pins
  // DDRB = DDRB | B00110000, so any pin except 12 and 13 will preserve their state, since x | 0 = x for x = 0 or 1
}

// the loop function runs over and over again forever
void loop() {
  PORTB |=  B00110000;  // using port registers, turn pins 12 and 13 HIGH together
  // SUM = SUM + 1;
  delay(DELAY);
  PORTB &= ~B00110000; // simultaneously turning 12 and 13 LOW together
  // we don't want to turn OFF other pins that might be required to stay on
  // PORTB = PORTB & B11001111, so any pin except 12 and 13 will preserve their state, since x & 1 = x for x = 0 or 1         
  delay(DELAY);             
}
