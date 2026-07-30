import serial
import csv
import sys
import time

CSV_FILE = 'log.csv'

start_time = time.perf_counter()

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

def extract(data):
    """Reads from the Arduino and returns the angle and buzzer values."""
    if not data:
        return None

    data = data.rstrip(b'\r\n')
    parts = data.split()  # split on whitespace, ignores extra spaces
    if len(parts) < 2:
        return None

    try:
        angle = parts[0][0]
        buzzer = bool(parts[1][0])
    except IndexError:
        return None
    return angle, buzzer

# communicate with the Arduino over the serial port
arduino = serial.Serial('COM4', 9600, timeout=1)

try:
    while True:
        data = arduino.readline()
        result = extract(data)
        if result is None:
            continue
        angle, buzzer = result
        log_to_csv(angle, buzzer)
except KeyboardInterrupt:
    pass
except Exception:
    import traceback
    traceback.print_exc()
finally:
    arduino.close()
    sys.exit()






