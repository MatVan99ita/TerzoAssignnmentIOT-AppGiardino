
import serial
import time
import requests
import threading
from time import sleep
import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext

IRRIG_TRESHOLD = 2
LIGHT_TRESHOLD = 5

msg_sabbiato = False
reading_thread = reading_thread = threading.Thread(daemon=True)  # Variabile globale per il thread di lettura

arduino_port = "COM5"
pippoBaud_rate = 9600
time_out = .1


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


def sendCommandToArduino():
    x = "LEDAUTO_3_" + str(slider.get()) + '\n'

    print("SENDATO: " + x)
    try:
        arduino.write(bytes(x, "UTF-8"))
        sleep(0.5)
        readArduinoToTheEnd()
    except serial.SerialException as e:
        print(f"Errore di connessione: {e}")
    except Exception as e:
        print(f"Errore generico: {e}")
    

def readArduinoToTheEnd():
    conta = 1
    start_time = time.time()  # Registra il tempo di inizio
    while (time.time() - start_time) < 5: # Ogni 5 secondi
        try:
            # Legge la linea dall'Arduino
            print(str(conta) + ") sonic sez: " + arduino.readline().decode('utf-8').strip())
            conta += 1
            # Aspetta 0.5 secondi prima di leggere nuovamente
            time.sleep(0.1)

        except serial.SerialException as e:
            print(f"Errore di connessione: {e}")
        except Exception as e:
            print(f"Errore generico: {e}")

def initializeSerial():
     global arduino
     while arduino is None:
        try:
            arduino = serial.Serial(
                port=arduino_port, 
                baudrate=pippoBaud_rate, 
                timeout=time_out
            )
            print(f"Connesso a {arduino_port}")
        except serial.SerialException:
            print(f"Porta {arduino_port} non disponibile, riprovo...")
            time.sleep(2)  # Aspetta 2 secondi prima di riprovare

#MAIN#
# Crea la finestra principale
initializeSerial()
root = tk.Tk()
root.title("Slider GUI")
root.geometry("500x300")

# Slider 2: con 8 valori (da 1 a 8), con callback quando cambia valore
slider = tk.Scale(root, from_=0, to=7, orient='horizontal', label="Slider 2 (8 valori) TEMPERATURA", length=200)
slider.pack(pady=10)

# Pulsante per aggiornare i valori visualizzati
button = ttk.Button(root, text="Aggiorna Valori", command=sendCommandToArduino)
button.pack(pady=10)

# Avvia la finestra principale
root.mainloop()

