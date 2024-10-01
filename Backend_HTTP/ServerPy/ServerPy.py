import serial
import time
import requests
import json
from time import sleep
import numpy as np

IRRIG_TRESHOLD = 2
LIGHT_TRESHOLD = 5

arduino_port = "COM5"
pippoBaud_rate = 9600
time_out = .1

server = "http://localhost:8000"

status = ""
arduino = None

def write_read(x):
    #arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    #data = arduino.readline()
    return data


def readArduinoStatus():
    var = arduino.readline()
    str = np.compat.unicode(var, errors='replace')
    irrigazione = ""
    mode = "" """TODO: cambiare il messaggio invito secondo il nuovo pattern"""
    """ PATTERN MESSAGGIO
        <DEVICE>_<ID>_<VALUE>
        
        TODO: fare meglio sto backend seriale
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
        print("send goku")
        print(arduino.readline())
        #time.sleep(1)
    except:
        print("Error occured: can't write properly")


def readServerStatus():
    request1 = server + "/arduino/status/"
    r1 = requests.get(request1)

    request2 = server + "/esp/data/"
    r2 = requests.get(request2)
    #print(r1.content)
    #print(r2.content)

    return r1.content.decode('utf-8'), r2.content.decode('utf-8')

def initializeSerial():
     global arduino
     while arduino is None:
        try:
            arduino = serial.Serial(port=arduino_port, baudrate=pippoBaud_rate, timeout=time_out)
            print(f"Connesso a {arduino_port}")
            print(arduino.readline())
        except serial.SerialException:
            print(f"Porta {arduino_port} non disponibile, riprovo...")
            time.sleep(2)  # Aspetta 2 secondi prima di riprovare


while True:

    initializeSerial()

    print("bio sbrolly")
    print(arduino.readline())
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
 

    """
    N.B.: 
        Luce -> 0-8
        Temperatura -> 0-5
        Se temperatura > 5 e Irrigazione in pausa -> ERRORE
        Se luce < 5(=Buio) -> Accendi luci, i fade vanno messi in base alla temperatura
        Se temp < 2 -> Accendi irrigatore con vel equivalente alla temperatura
    """
    print(jStatus)
    if(jEsp["temperatura"] == 5 and irriStatus == "RELOAD"): # || PAUSE
        print("ERROR")
        sendCommandToArduino("ERROR")
    else:
        if(jEsp["lux"] < LIGHT_TRESHOLD): # Fai giochi di luce
            msg1 = "LEDAUTO_"+str(jEsp["temperatura"])
            sendCommandToArduino(msg1)
        elif(jEsp["lux"] < IRRIG_TRESHOLD): # Fai giochi d'acqua
            msg = "IRRIAUTO_"+str(jEsp["temperatura"]) #<- posso calcolare la velocità anche qui e mnadarla già cacata
            sendCommandToArduino(msg)

    #print("BANANA " + jStat + " " + jEsp + "\n") 

    """
    if(x != "ERROR"):
        print("BANANA " + str(x) + " " + str(y) + "\n") 
        sendCommandToArduino(x)
    else:
        sendCommandToArduino("ERROR")
    """
    #Piccola pausa di riflessione
    
    print(arduino.readline())
    sleep(1)