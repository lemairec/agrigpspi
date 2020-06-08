import serial

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200
)

while True:
    response = ser.readline()
    print(response)

