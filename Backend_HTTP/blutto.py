import serial

def monitor_hc05_uart(port: str, baudrate: int = 9600):
    """
    Monitora lo stato di pairing dell'HC-05 analizzando i messaggi UART.

    Args:
        port (str): La porta seriale a cui Arduino è connesso (es. "COM3" o "/dev/ttyUSB0").
        baudrate (int): Il baudrate della comunicazione seriale (default: 9600).
    """
    try:
        with serial.Serial(port, baudrate, timeout=1) as ser:
            print("Connessione alla porta seriale avviata. Premere Ctrl+C per uscire.")
            while True:
                line = ser.readline().decode('utf-8').strip()
                print(line)
                if line:
                    if "+CONNECTED" in line:
                        print("Il modulo HC-05 è collegato a un dispositivo.")
                    elif "+DISCONNECTED" in line:
                        print("Il modulo HC-05 è disconnesso.")
    except serial.SerialException as e:
        print(f"Errore nella connessione seriale: {e}")
    except KeyboardInterrupt:
        print("Monitoraggio interrotto.")

# Esempio di utilizzo
monitor_hc05_uart(port="COM5")
