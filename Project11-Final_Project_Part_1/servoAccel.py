import serial
import csv
import sys
import time

start_time = time.time()

CSV_FILE = 'log.csv'

# open the CSV file in write mode and write the header row
with open(CSV_FILE, mode="w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow(["Seconds", "Angle", "Buzzer"])

# define function to log new lines
def log_to_csv(angle, buzzer):
    """Calculates elapsed time and appends a row to the CSV file."""
    elapsed = time.perf_counter() - start_time

    # Open in 'a' (append) mode so we don't erase previous rows
    with open(CSV_FILE, mode="a", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        # Format elapsed time to 4 decimal places
        writer.writerow([f"{elapsed:.4f}", angle, buzzer])

def serial_read(arduino):
    """Reads a line from the Arduino and returns the angle and buzzer values."""
    inByte = arduino.readline()
    if inByte == 'A':
        arduino.clear_input_buffer()
    data = arduino.readline().decode("utf-8").strip()
    if data:
        try:
            angle_str, buzzer_str = data.split(",")
            angle = int(angle_str)
            buzzer = int(buzzer_str)
            return angle, buzzer
        except ValueError:
            print(f"Invalid data received: {data}")
            return None, None
    return None, None

# communicate with the Arduino over the serial port
arduino = serial.Serial('COM4', 9600, timeout=1)

data = arduino.readline()




