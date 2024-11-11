import random
import threading
import time

# Variabile per il controllo del timer
timer_attivo = False

def avvia_timer():
    """Funzione che attiva il timer e lo disattiva dopo 90 secondi."""
    global timer_attivo
    timer_attivo = True
    print("Timer attivato per 90 secondi.")
    time.sleep(15)  # Attende 90 secondi
    timer_attivo = False
    print("Timer disattivato.")

def invia_messaggio(messaggio):
    """Simula l'invio di un messaggio al server."""
    print(f"Messaggio inviato al server: {messaggio}")

def elabora_numeri(numero1, numero2):
    """Funzione che elabora i numeri ricevuti e invia i messaggi in base alle condizioni."""
    global timer_attivo
    if numero2 < 5:
      # Invia il primo numero senza modificarlo
      if numero2 < 2:
        if not timer_attivo:
            # Invia il primo numero raddoppiato
            invia_messaggio(numero1 * 2)
            # Avvia il timer in un thread separato
            threading.Thread(target=avvia_timer).start()
        else:
            print("Timer attivo: invio speciale disabilitato.")
      else:
        invia_messaggio(numero1)


def ricevi_dal_server():
    """Simula la ricezione continua di numeri dal server."""
    while True:
        # Genera numeri casuali come simulazione di ricezione dal server
        numero1 = random.randint(0, 4)
        numero2 = random.randint(0, 7)
        print(f"Ricevuti dal server: numero1={numero1}, numero2={numero2}")
        
        # Elabora i numeri ricevuti
        elabora_numeri(numero1, numero2)
        
        # Pausa per simulare l'intervallo di ricezione
        time.sleep(1)

# Avvia il processo di ricezione dal server
ricevi_dal_server()
