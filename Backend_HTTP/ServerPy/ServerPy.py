import serial
import time
import requests
import json

import numpy as np

arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)
server = "http://localhost:8000"
"""
Far partire prima l'arduino e poi questo

TODO:
    usare uvicorn per mandare e ricevere cose (credo)
    RILEGGERE STO PEZZO NELLE SLIDE

"""

status = ""


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


def readArduinoStatus():
    var = arduino.readline()
    str = np.compat.unicode(var, errors='replace')
    irrigazione = ""
    mode = ""
    """ PATTERN MESSAGGIO
        irrigazione:<val>,mode:<val>
    """
    print("arduino statuuuus: " + str)
    try:
        irrigazione = str.split(',')[0].split(':')[1]
        mode = str.split(',')[1].split(':')[1].replace('\r\n', '')
        request = server + "/arduino/status/" + mode
        r = requests.post(request)
        print(r)
        request = server + "/arduino/irrigation/" + irrigazione
        r = requests.post(request)
        print(r)
    except:
        print("Error occured: cannot read/send properly")


def sendCommandToArduino(x):
    print("arduino cummand: " + str(x))
    try:
        arduino.write(bytes(x, 'utf-8'))
        time.sleep(5000)
    except:
        print("Error occured: can't write properly")


def readServerStatus():
    request1 = server + "/arduino/status/"
    r1 = requests.get(request1)

    request2 = server + "/arduino/irrigation/"
    r2 = requests.get(request2)

    print(r1.content)
    print(r2.content)

    return r1.content, r2.content


while True:
    readArduinoStatus()
    x, y = readServerStatus()

    if(x != "ERROR"):
        print("BANANA " + str(x) + " " + str(y) + "\n") 
        sendCommandToArduino(x)
    else:
        sendCommandToArduino("ERROR")
