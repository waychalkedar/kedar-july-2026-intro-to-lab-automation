# written with Copilot

# Imports for threading, GUI, and Firmata communication
import sys
import time
import threading
import queue
import FreeSimpleGUI as sg

from telemetrix import telemetrix

# Arduino pin assignments
LED_PIN = 4
BUTTON_PIN = 6
DEFAULT_ON_TIME_MS = 5000

# Shared state between the GUI thread and the Firmata thread
on_time_ms = DEFAULT_ON_TIME_MS
on_time_lock = threading.Lock()
state_queue = queue.Queue()
stop_event = threading.Event()

# Callback data indices used by telemetrix digital reporting
CB_PIN_MODE = 0
CB_PIN = 1
CB_VALUE = 2
CB_TIME = 3


def get_on_time_ms():
    """Return the current LED on-time in milliseconds safely."""
    with on_time_lock:
        return on_time_ms


def set_on_time_ms(value):
    """Update the LED on-time in milliseconds safely."""
    global on_time_ms
    with on_time_lock:
        on_time_ms = value


def gui_thread():
    """Run the FreeSimpleGUI event loop in a separate thread."""
    sg.theme('DefaultNoMoreNagging')

    layout = [
        [sg.Text('Welcome to the Firmata GUI exercise!')],
        [sg.Text('Enter the LED on-time in milliseconds:'), sg.InputText(str(DEFAULT_ON_TIME_MS), key='-INPUT-', size=(12, 1))],
        [sg.Button('Set'), sg.Button('Exit')],
        [sg.Text('Configured timeout:'), sg.Text(f'{DEFAULT_ON_TIME_MS} ms', key='-TIMEOUT-')],
        [sg.Text('LED state:'), sg.Text('Unknown', key='-STATE-')],
        [sg.Text('', key='-MESSAGE-', size=(50, 1))],
    ]

    window = sg.Window('Telemetrix Firmata GUI', layout, finalize=True)

    while True:
        event, values = window.read(timeout=100)

        # Close the app when the user exits
        if event in (sg.WIN_CLOSED, 'Exit'):
            stop_event.set()
            break

        # Update the shared timeout value when the user clicks Set
        if event == 'Set':
            input_text = values.get('-INPUT-', '').strip()
            try:
                new_value = int(input_text)
                if new_value <= 0:
                    raise ValueError('Must be positive')
                set_on_time_ms(new_value)
                window['-TIMEOUT-'].update(f'{new_value} ms')
                window['-MESSAGE-'].update('Updated timeout successfully.')
            except ValueError:
                window['-MESSAGE-'].update('Please enter a valid positive integer.')

        # Read LED state updates from the Firmata thread and display them
        try:
            while True:
                state_text = state_queue.get_nowait()
                window['-STATE-'].update(state_text)
        except queue.Empty:
            pass

    window.close()


def firmata_thread():
    """Run the Firmata board interaction in a separate thread."""
    board = telemetrix.Telemetrix()
    turn_off_timer = None

    def led_off():
        """Turn off the LED and notify the GUI thread."""
        board.digital_write(LED_PIN, 0)
        state_queue.put('LED OFF')

    def button_callback(data):
        """Handle button events reported by telemetrix."""
        nonlocal turn_off_timer
        pin = data[CB_PIN]
        value = data[CB_VALUE]

        if pin != BUTTON_PIN:
            return

        if value == 1:
            # Button pressed: turn LED on and restart the timeout timer
            board.digital_write(LED_PIN, 1)
            state_queue.put('LED ON')

            if turn_off_timer is not None and turn_off_timer.is_alive():
                turn_off_timer.cancel()

            turn_off_timer = threading.Timer(get_on_time_ms() / 1000.0, led_off)
            turn_off_timer.start()
        else:
            # Button released events are ignored for the timeout.
            # LED remains on until the timer expires.
            pass

    # Configure the Firmata board pins and start digital reporting for the button
    board.set_pin_mode_digital_output(LED_PIN)
    board.set_pin_mode_digital_input(BUTTON_PIN, button_callback)
    board.enable_digital_reporting(BUTTON_PIN)

    try:
        while not stop_event.is_set():
            time.sleep(0.1)
    finally:
        if turn_off_timer is not None and turn_off_timer.is_alive():
            turn_off_timer.cancel()
        board.shutdown()


def main():
    """Start the GUI and Firmata threads and keep the program alive."""
    gui = threading.Thread(target=gui_thread, daemon=True)
    firmata = threading.Thread(target=firmata_thread, daemon=True)

    gui.start()
    firmata.start()

    while gui.is_alive() and firmata.is_alive():
        time.sleep(0.1)

    stop_event.set()
    gui.join(timeout=1)
    firmata.join(timeout=1)


if __name__ == '__main__':
    main()
