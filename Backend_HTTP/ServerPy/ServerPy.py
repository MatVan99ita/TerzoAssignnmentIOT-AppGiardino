import serial
import time
import requests
import json
from time import sleep
import numpy as np

IRRIG_TRESHOLD = 2
LIGHT_TRESHOLD = 5

msg_sabbiato = False

arduino_port = "COM5"
pippoBaud_rate = 9600
time_out = .1

server = "http://localhost:8000"

status = ""
arduino = None

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
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
        arduino.write(bytes(x+'\n', "UTF-8"))
        sleep(0.5)
        msg_sabbiato = True
        print("sonic sez: " + arduino.readline().decode('utf-8').strip())
    except serial.SerialException as e:
        print(f"Errore di connessione: {e}")
    except Exception as e:
        print(f"Errore generico: {e}")

def readArduinoToTheEnd():
    start_time = time.time()  # Registra il tempo di inizio
    while (time.time() - start_time) < 10:  # Continua per 10 secondi
        try:
            # Legge la linea dall'Arduino
            print("sonic sez: " + arduino.readline().decode('utf-8').strip())
            
            # Aspetta 0.5 secondi prima di leggere nuovamente
            time.sleep(0.5)

        except serial.SerialException as e:
            print(f"Errore di connessione: {e}")
        except Exception as e:
            print(f"Errore generico: {e}")
    msg_sabbiato = False

def readServerStatus():
    request1 = server + "/arduino/status/"
    r1 = requests.get(request1)

    request2 = server + "/esp/data/"
    r2 = requests.get(request2)

    return r1.content.decode('utf-8'), r2.content.decode('utf-8')

def initializeSerial():
     global arduino
     while arduino is None:
        try:
            arduino = serial.Serial(
                port=arduino_port, 
                baudrate=pippoBaud_rate, 
                timeout=time_out, 
                writeTimeout = 2
            )
            print(f"Connesso a {arduino_port}")
            print(arduino.readline())
        except serial.SerialException:
            print(f"Porta {arduino_port} non disponibile, riprovo...")
            time.sleep(2)  # Aspetta 2 secondi prima di riprovare

#MAIN#
while True:

    initializeSerial()

    irriStatus, espData = readServerStatus()
    
    print("BANANA " + irriStatus + " " + espData + "\n") 
    jStatus = json.loads(irriStatus)
    jEsp = json.loads(espData)
 

    if(msg_sabbiato):
        readArduinoToTheEnd()
    
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
            msg1 = "LEDAUTO_1_"+str(jEsp["temperatura"])
            sendCommandToArduino(msg1)
        elif(jEsp["lux"] < IRRIG_TRESHOLD): # Fai giochi d'acqua
            msg = "IRRIAUTO_"+str(jEsp["temperatura"]) #<- posso calcolare la velocità anche qui e mnadarla già cacata
            sendCommandToArduino(msg)
    
    sleep(1)