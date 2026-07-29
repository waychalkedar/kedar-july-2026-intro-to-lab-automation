# Written with Copilot

"""Simple serial GUI to work with userBlink_serial.ino

Features:
- List and connect to serial ports
- Set LED on-time (ms) and send to Arduino (adds newline)
- Read device state codes and display LED state
Requires: pyserial, freesimplegui
"""
"""Simple serial GUI to work with `userBlink_serial.ino`.

This module provides a small FreeSimpleGUI interface to:
- list and select serial ports
- configure the LED on-time in milliseconds and send it to the Arduino
- display simple device state messages sent by the Arduino (codes 0/1/2)

Dependencies:
- `pyserial` (serial communication)
- `freesimplegui` (lightweight GUI)

Run with: `python userBlink_serial_gui.py`
"""

import queue
import threading
import time

import FreeSimpleGUI as sg
import serial
import serial.tools.list_ports

# Default serial settings (matches the sketch)
DEFAULT_BAUD = 9600
READ_TIMEOUT = 0.1


def list_ports():
    """Return available serial port device names as a list.

    Uses pyserial's list_ports convenience function and returns only the device
    path (e.g. 'COM3' on Windows or '/dev/ttyUSB0' on Linux).
    """
    return [p.device for p in serial.tools.list_ports.comports()]


class SerialReader(threading.Thread):
    """Background thread that reads lines from a serial connection.

    - Reads lines using `serial.readline()` so the Arduino should send newline-terminated
      messages (the provided sketch sends short codes and occasional text lines).
    - Puts decoded text lines into the GUI queue for the main thread to display.
    - Stops when `stop_event` is set.
    """

    def __init__(self, ser, q, stop_event):
        super().__init__(daemon=True)
        self.ser = ser
        self.q = q
        self.stop_event = stop_event

    def run(self):
        while not self.stop_event.is_set():
            try:
                raw = self.ser.readline()
            except Exception as e:
                # Communicate errors back to the GUI and exit the reader thread
                self.q.put(f"Serial read error: {e}")
                break

            if not raw:
                # Timeout with no data; loop again
                continue

            # Decode bytes to text safely and strip whitespace/newlines
            try:
                text = raw.decode('utf-8', errors='replace').strip()
            except Exception:
                text = '<decode error>'

            if text:
                self.q.put(text)


def build_window():
    """Construct and return the FreeSimpleGUI window object.

    The layout includes controls for selecting a port, connecting, setting the
    LED on-time (ms), and a log area that shows incoming device messages.
    """
    layout = [
        [sg.Text('Port'), sg.Combo(list_ports(), key='-PORT-', size=(20, 1)), sg.Button('Refresh', key='-REFRESH-')],
        [sg.Text('Baud'), sg.InputText(str(DEFAULT_BAUD), key='-BAUD-', size=(8, 1)),
         sg.Button('Connect', key='-CONNECT-'), sg.Button('Disconnect', key='-DISCONNECT-')],
        [sg.Text('LED on time (ms)'), sg.InputText('1000', key='-TIME-'), sg.Button('Send', key='-SEND-')],
        [sg.Text('LED State:'), sg.Text('Unknown', key='-STATE-')],
        [sg.Multiline('', size=(60, 10), key='-LOG-', disabled=True)],
        [sg.Button('Exit')]
    ]

    return sg.Window('Serial LED Controller', layout, finalize=True)


def append_log(win, msg):
    """Append a timestamped message to the GUI log box.

    Reads the existing content from the `-LOG-` element, appends the new line,
    and updates the widget. Kept simple for clarity.
    """
    if not msg:
        return
    now = time.strftime('%H:%M:%S')
    prev = win['-LOG-'].get()
    win['-LOG-'].update(prev + f"[{now}] {msg}\n")


def main():
    """Main application routine: GUI loop plus serial reader management.

    The GUI thread is the main thread here; a `SerialReader` background thread
    handles incoming serial lines and places them on a `queue.Queue` for safe
    communication back to the GUI.
    """
    window = build_window()

    ser = None               # pyserial Serial instance
    reader = None            # background SerialReader thread
    stop_event = threading.Event()  # used to stop the reader thread
    q = queue.Queue()        # queue for messages from reader -> GUI

    while True:
        # Non-blocking read with a timeout so the GUI remains responsive
        event, values = window.read(timeout=100)
        if event in (sg.WINDOW_CLOSED, 'Exit'):
            break

        # Refresh the list of available ports
        if event == '-REFRESH-':
            window['-PORT-'].update(values=list_ports())

        # Connect to selected serial port
        if event == '-CONNECT-':
            port = values.get('-PORT-')
            try:
                baud = int(values.get('-BAUD-'))
            except Exception:
                append_log(window, 'Invalid baud')
                continue

            if not port:
                append_log(window, 'Select a port')
                continue

            try:
                ser = serial.Serial(port, baud, timeout=READ_TIMEOUT, write_timeout=READ_TIMEOUT)
                stop_event.clear()
                reader = SerialReader(ser, q, stop_event)
                reader.start()
                append_log(window, f'Connected to {port} @ {baud}')
            except Exception as e:
                append_log(window, f'Open error: {e}')

        # Disconnect from serial port and stop reader
        if event == '-DISCONNECT-':
            if ser and ser.is_open:
                stop_event.set()
                ser.close()
                append_log(window, 'Disconnected')

        # Send the configured LED on-time (ms) to the Arduino over serial
        if event == '-SEND-':
            if not ser or not ser.is_open:
                append_log(window, 'Not connected')
                continue
            t = values.get('-TIME-', '').strip()
            if not t.isdigit():
                append_log(window, 'Enter positive integer ms')
                continue
            payload = t + '\n'  # sketch expects newline-terminated integer
            try:
                ser.write(payload.encode('utf-8'))
                append_log(window, f'Sent timeout: {t} ms')
            except Exception as e:
                append_log(window, f'Write error: {e}')

        # Drain any messages from the serial reader and update GUI state/log
        try:
            while True:
                msg = q.get_nowait()

                # Interpret simple state codes sent by the Arduino sketch
                if msg == '0':
                    window['-STATE-'].update('LED OFF')
                    append_log(window, 'Device: LED OFF')
                elif msg == '1':
                    window['-STATE-'].update('LED ON')
                    append_log(window, 'Device: LED ON')
                elif msg == '2':
                    window['-STATE-'].update('Button released')
                    append_log(window, 'Device: Button released')
                else:
                    # Print any informational lines from the device (e.g. "I received: 5000")
                    append_log(window, f'Received: {msg}')
        except queue.Empty:
            pass

    # Cleanup on exit: stop reader and close serial port if open
    stop_event.set()
    if ser and ser.is_open:
        ser.close()
    window.close()


if __name__ == '__main__':
    main()
