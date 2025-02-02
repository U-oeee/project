# -*- coding: utf-8 -*-
import serial
import time
import sys

# Ensure UTF-8 encoding for proper text output
sys.stdout.reconfigure(encoding='utf-8')

# Serial port configuration
mega_port = "/dev/ttyACM0"   # Mega board
uno_port_1 = "/dev/ttyUSB0"  # First Uno board


# Serial port properties
baud_rate = 9600  # Default baud rate for Arduino
timeout = 1       # Response timeout (seconds)

# Establish serial communication with Mega and Uno boards
try:
    mega_serial = serial.Serial(mega_port, baud_rate, timeout=timeout)
    uno_serial_1 = serial.Serial(uno_port_1, baud_rate, timeout=timeout)
    print("Serial connections established successfully.")
except Exception as e:
    print(f"Error opening serial port: {e}")
    exit(1)

# Main loop: Read from Mega and forward data to Uno if needed
try:
    while True:
        if mega_serial.in_waiting > 0:  # Check if data is available
            received_data = mega_serial.readline().strip()  # Read raw bytes
            received_data_uno1 = uno_serial_1.readline().strip()
            print(f"Received from Mega: {received_data}")

            if received_data == b"1START":  # Ensure it's a byte, not a string
                uno_serial_1.write(b'1')  # Send raw byte (integer 1)
                print("Successfully sent integer 1 to Uno Board 1")
                
                if mega_serial.in_waiting > 0:
                    if received_data == b"PUSH":
                        uno_serial_1.write(b'2')
                        print("Successfully sent integer 2 to Uno Board 1")
                    
                        if mega_serial.in_waiting > 0:
                            if received_data_uno1 == b"3":
                                print(f"Received from Uno1: {received_data_uno1}")
                                uno_serial_1.write(b'4')
                                print("Successfully sent integer 4 to Uno Board 1")
                    
                    

        time.sleep(0.1)  # Reduce CPU usage

except KeyboardInterrupt:
    print("Program terminated by user.")

finally:
    # Close serial ports before exiting
    mega_serial.close()
    uno_serial_1.close()
    print("Serial ports closed.")
