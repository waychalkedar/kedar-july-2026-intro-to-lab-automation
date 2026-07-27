# Project 5 - Reacting to input part I: Interrupts

1. Make led light on button press
2. Learn how to debug code by establish communication with the computer using the Serial library
3. Understand the difference between polling and interrupt and benefits of using interrupts over polling
4. Learn how to use interrupts in Arduino

## Light led on button press (arduino code)
 - button pin is 6, grove led pin is 4
 - read value from button pin. If button is pressed (how can you tell?) turn grove led on. If button is not pressed, turn it off
 - test that this works.

## Debug statements
- use the Serial library to add debug statements to your code. View using Serial Monitor in Arduino IDE

## Simulate additional long process
- We wish to simulate a long process that happens in the loop. In your lab, this could be a calculation, data acquisition, etc. In order to simulate this, add to your loop the following code:

```
for (int i = 0; i< 10000; i++){
    Serial.println("calculating...");
}
```
- test if lighting led still works. Why or why not?
answer here: The LED only works if the button was pressed in the time after the loop, and the state is maintained. This can be seen by shortening the process (1000 iterations for example) and adding a delay after the loop ends to make sure you catch when the main `loop()` repeats itself.

The red LED is actually hardwired to the button, so even when you upload a blank sketch to the Arduino, the LED turns on with the button. The LED doesn't turn on when you have the long loop, so I guess when pin D6 (button) is assigned to do something in a program that takes priority(?)

## Use interrupt to light led
- Add code to create an interrupt pin (why can't it be the button pin?). Use a variable for this. Note: The interrupt pin is NOT a kill switch. You pause the process, do the check for the LED, and then resume the process.
- Short the interrupt pin with the button pin
- test. Does it work?

It's interesting to play around with the modes for the `attachInterrupt()` function. 
- `CHANGE` : the button turns on and off as expected
- `RISING` : the button turns on but stays on until the loop ends
- `LOW`    : the rest of the code only runs as long as the button is pressed

## Exercises
 - Comparison of AI changes if any:
 - commit and upload your code in this project folder.


