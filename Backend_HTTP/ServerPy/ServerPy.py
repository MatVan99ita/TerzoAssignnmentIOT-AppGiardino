import serial
import time
import requests
import json
from time import sleep
import numpy as np

TEMP_TRESHOLD = 2
LIGHT_TRESHOLD = 5

""" N.B.: Far partire prima l'arduino e poi questo """
#arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)
server = "http://localhost:8000"

status = ""


def write_read(x):
    #arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    #data = arduino.readline()
    return data


def readArduinoStatus():
    #var = arduino.readline()
    #str = np.compat.unicode(var, errors='replace')
    irrigazione = ""
    mode = "" """TODO: cambiare il messaggio invito secondo il nuovo pattern"""
    """ PATTERN MESSAGGIO
        <DEVICE>_<ID>_<VALUE>
        
        TODO: fare meglio sto backend seriale
    """
    #print("arduino statuuuus: " + str)
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
        #arduino.write(bytes(x, 'utf-8'))
        time.sleep(5000)
    except:
        print("Error occured: can't write properly")


def readServerStatus():
    request1 = server + "/arduino/status/"
    r1 = requests.get(request1)

    request2 = server + "/esp/data/"
    r2 = requests.get(request2)

    print(r1.content)
    print(r2.content)

    return r1.content.decode('utf-8'), r2.content.decode('utf-8')


while True:
    """statuuus = readArduinoStatus()
    if(statuuus == "AUTO"):
        #controllo dal server
        print("Auto")
    elif (statuuus == "MANUAL"):
        #controllo manuale da android
        print("Manual")"""
    irriStatus, espData = readServerStatus()
    #Creazione comando per arduino
    #<DEVICE>_<ID>_<VALUE>
    
    print("BANANA " + irriStatus + " " + espData + "\n") 
    jStatus = json.loads(irriStatus)
    jEsp = json.loads(espData)

    if(jStatus["temperatura"] < TEMP_TRESHOLD):
        msg = "IRRI_ON"
        #if irrigazione = PAUSA -> ERROR
        #Send irrigazione se non è disattivato altrimenti manda ERROR
        print("BANANA")

    if(jStatus["lux"] < LIGHT_TRESHOLD):
        #Fai giochi di luce
        msg1 = "LEDB_3_ON"
        msg2 = "LEDF_3"
        print("BANANA")

    #print("BANANA " + jStat + " " + jEsp + "\n") 

    """
    if(x != "ERROR"):
        print("BANANA " + str(x) + " " + str(y) + "\n") 
        sendCommandToArduino(x)
    else:
        sendCommandToArduino("ERROR")
    """
    #Piccola pausa di riflessione
    #sleep(1000)