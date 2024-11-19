import serial
import time
import requests
import json
import threading
from time import sleep
import numpy as np

LIGHT_TRESHOLD = 5
IRRIG_TRESHOLD = 2
IRRIGATION_WAIT_TIME = 1 #Wait time per usare il servo che deve essere di qualche minuto

msg_sabbiato = False
timer_attivo = False

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
    mode = ""
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

def start_irrigation_timer():
    global timer_attivo

    timer_attivo = True
    request = server + "/arduino/irrigation/PAUSA"
    r = requests.post(request)
    print(timer_attivo)


    time.sleep(IRRIGATION_WAIT_TIME * 60)
    timer_attivo = False
    print(timer_attivo)
    request = server + "/arduino/irrigation/ATTIVABILE"
    r = requests.post(request)


def sendCommand(x):
    print("arduino cummand: " + str(x))
    if "IRRI" in str(x) and not timer_attivo:
        print("ENTERO, ", timer_attivo)
        sendCommandToArduino(x)
        print("ALURA")
        request = server + "/arduino/irrigation/MOVIMENTO"
        r = requests.post(request)
        threading.Thread(target=start_irrigation_timer).start()
    elif "IRRI" not in str(x):# LED or ERROR
        sendCommandToArduino(x)


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
    global status
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

    request3 = server + "/arduino/irrigation/"
    r3 = requests.get(request3)

    return r1.content.decode('utf-8'), r2.content.decode('utf-8'), r3.content.decode('utf-8')

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

    irriStatus, espData, status = readServerStatus()
    
    print("BANANA " + irriStatus + " " + espData + " " + status + "\n") 
    jStatus = json.loads(irriStatus)
    jEsp = json.loads(espData) 
 

    if(msg_sabbiato):
        readArduinoToTheEnd()
    
    """
    N.B.: 
        Luce -> 0-7
        Temperatura -> 0-4
        Se temperatura > 5 e Irrigazione in pausa -> ERRORE
        Se luce < 5(=Buio) -> Accendi luci, i fade vanno messi in base alla temperatura
        Se luce < 2(=BUISSIMO) -> Accendi irrigatore con vel equivalente alla temperatura
    """
    print(jStatus)


    if(status != "MANUAL"):
        if(jEsp["temperatura"] == 5 and irriStatus == "PAUSA"): # || PAUSE
            print("ERROR")
            sendCommand("ERROR")
            status = "ERROR"
        else:
            if(jEsp["lux"] < LIGHT_TRESHOLD): # Fai giochi di luce
                msg1 = "LEDAUTO_"+str(jEsp["temperatura"])
                sendCommand(msg1)
            elif(jEsp["lux"] >= LIGHT_TRESHOLD):
                msg2 = "LEDAUTO_0" #Ci stanno tracciando STACCA STACCA
                sendCommand(msg2)
            
            if(jEsp["lux"] < IRRIG_TRESHOLD): # Fai giochi d'acqua
                msg = "IRRI_"+str(jEsp["temperatura"])
                sendCommand(msg)
        
    
    sleep(1)