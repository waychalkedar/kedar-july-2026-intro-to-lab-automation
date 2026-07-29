# written with Copilot

import sys
import time

from telemetrix import telemetrix

board = telemetrix.Telemetrix()

LED_PIN = 4
BUTTON_PIN = 6

# Callback data indices for telemetrix digital reporting
CB_PIN_MODE = 0
CB_PIN = 1
CB_VALUE = 2
CB_TIME = 3


def button_callback(data):
    """Handle button state changes and update the LED."""
    pin = data[CB_PIN]
    value = data[CB_VALUE]

    if pin != BUTTON_PIN:
        return

    if value == 1:
        board.digital_write(LED_PIN, 1)
        print("Button pressed: LED on")
    else:
        board.digital_write(LED_PIN, 0)
        print("Button released: LED off")


board.set_pin_mode_digital_output(LED_PIN)
board.set_pin_mode_digital_input(BUTTON_PIN, button_callback)
board.enable_digital_reporting(BUTTON_PIN)

try:
    while True:
        time.sleep(0.1)
except KeyboardInterrupt:
    board.shutdown()
    sys.exit(0)
