import serial
import time

# Initialize serial port
ser = serial.Serial('/dev/ttyS0', 57600)  # Assuming your sensor communicates at 57600 baud rate

try:
    while True:
        # Read data from the sensor
        data = ser.readline().decode().strip()  # Decode the received bytes and remove any whitespace
        
        # Process and print the received data
        print("Received data:", data)

        time.sleep(0.01)

except KeyboardInterrupt:
    print("\nExiting...")
    ser.close()
