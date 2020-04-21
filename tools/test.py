import serial

ser = serial.Serial(
    port='/dev/cu.usbmodem1452201',
    baudrate=9600
)

while True:
    response = ser.readline()
    print("read data: " + response)

