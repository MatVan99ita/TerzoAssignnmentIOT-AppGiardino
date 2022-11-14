import serial
import time
import requests
import json

import numpy as np

arduino = serial.Serial(port='COM6', baudrate=115200, timeout=.1)
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
    try:
        irrigazione = str.split(',')[0].split(':')[1]
        mode = str.split(',')[1].split(':')[1].replace('\r\n', '')
    except:
        print("Error occured")


    return irrigazione, mode


def sendCommandToArduino(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)

while True:
    ir, m = readArduinoStatus()
    print(ir, m)
    request = server + "/arduino/status/" + str(ir)
    r = requests.post(str(request))
    print("ok" if r == 200 else "no")
    request = server + "/arduino/irrigation/" + str(ir)
    r = requests.post(str(request))
    print("ok" if r == 200 else "no")

    # print(r.content)
    # print(requests.get(str(server + "/py/get")))
