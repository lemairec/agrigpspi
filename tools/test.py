import serial

ser = serial.Serial(
    port='/dev/cu.wchusbserial1420',
    baudrate=9600
)

while True:
    response = ser.readline()
    print("read data: " + response)

