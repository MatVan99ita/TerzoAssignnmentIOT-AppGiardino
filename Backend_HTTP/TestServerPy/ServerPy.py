import serial
import time

arduino = serial.Serial(port='COM6', baudrate=115200, timeout=.1)

"""
Far partire prima l'arduino e poi questo

TODO:
    usare uvicorn per mandare e ricevere cose (credo)
    RILEGGERE STO PEZZO NELLE SLIDE

"""
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


while True:
    num = input("Enter a number: ")
    value = write_read(num)
    print(value)