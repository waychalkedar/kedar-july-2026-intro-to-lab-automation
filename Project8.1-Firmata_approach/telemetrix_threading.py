# Written with Copilot

# Imports
import sys
import time
import threading
import FreeSimpleGUI as sg
import serial

from telemetrix import telemetrix


# set up the GUI window
sg.theme('DefaultNoMoreNagging')

valueRead = 0

layout = [
    [sg.Text('Welcome to the firmata GUI exercise!')],
    [sg.Text('Enter the on time for the LED (in milliseconds)'), 
    sg.Input(key = '-INPUT-')],
    [sg.Text(key = '-OUTPUT1-')],
    [sg.Text(key = '-OUTPUT2-')],
    [sg.Button('OK'), sg.Button('Cancel')]
]

window = sg.Window('Firmata GUI', layout)


# set up the firmata
board = telemetrix.Telemetrix()

LED_PIN = 4
BUTTON_PIN = 6

board.set_pin_mode_digital_output(LED_PIN)
board.set_pin_mode_digital_input(BUTTON_PIN)

# Callback data indices for telemetrix digital reporting
CB_PIN_MODE = 0
CB_PIN = 1
CB_VALUE = 2
CB_TIME = 3

def led_off(pin):
    board.digital_write(pin, 0)  # Turn off the LED
    return "LED off"

timer = threading.Timer(valueRead, led_off, args=[LED_PIN])

def button_callback(data):
    """Handle button state changes and update the LED."""
    pin = data[CB_PIN]
    value = data[CB_VALUE]
    # state = True
    if pin != BUTTON_PIN:
        return

    state = True if value == 1 else False
    
    if state:
        # turn on the LED for the specified duration
        board.digital_write(LED_PIN, 1)
        print("Button pressed: LED on")
        timer.start()
    else:
        board.digital_write(LED_PIN, 0)
        print("Button released")


board.set_pin_mode_digital_output(LED_PIN)
board.set_pin_mode_digital_input(BUTTON_PIN, button_callback)
board.enable_digital_reporting(BUTTON_PIN)


# main loop
try:
    while True:
        time.sleep(0.1)
    #     event, values = window.read()
    #     if event == sg.WIN_CLOSED or event == 'Cancel':
    #         break
    #     if values['-INPUT-']:
    #         valueRead = int(values['-INPUT-'])
    #     window['-OUTPUT1-'].update("The LED is now configured to stay ON for " 
    #     + values['-INPUT-'] + " milliseconds")
    #     window['-OUTPUT2-'].update("Go ahead and press the button!")

    # window.close()

except KeyboardInterrupt:
    board.shutdown()
    sys.exit(0)
