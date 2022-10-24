#importiamo i moduli che utilizzeremo
import tkinter as tk
import socket
import threading
from time import sleep
import random as rnd
import json
#import urllib2
#import urllib3 as url
import requests

def server_getIp():# funzione usata per ottenere l'ip del server che servirà poi al client per connettersi
    global HOST_ADDR
    hostname=socket.gethostname()
    s=socket.gethostbyname(hostname)
    return(s)

server = None
HOST_ADDR = ""
HOST_PORT = 53000
client_name = " "
clients = []
clients_names = []
player_data = []

window = tk.Tk()
window.title("Server")

def get_ip():
    """estrae l'ip per mostrarlo a video """
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

# Avvia la funzione server
def start_server():
    global server, HOST_ADDR, HOST_PORT
    btnStart.config(state=tk.DISABLED)
    btnStop.config(state=tk.NORMAL)

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print (socket.AF_INET)
    print (socket.SOCK_STREAM)

    server.bind((HOST_ADDR, HOST_PORT))
    server.listen(5)  # il server è in ascolto per la connessione del client

    threading._start_new_thread(accept_clients, (server, " "))

    print("Server started")

# Arresta la funzione server
def stop_server():
    global server
    btnStart.config(state=tk.NORMAL)
    btnStop.config(state=tk.DISABLED)

def accept_clients(the_server, y):
    while True:
        client, addr = the_server.accept()
        clients.append(client)
        # utilizza un thread in modo da non intasare il thread della gui
        threading._start_new_thread(send_receive_client_message, (client, addr))


def send_receive_client_message(client_connection, client_ip_addr):
    global server, client_name, clients, player_data, idx
    
    server = "http://" + str(get_ip()) + ":" + str(HOST_PORT)
    idx = get_client_index(clients, client_connection)
    client_name = client_connection.recv(4096)
    val = rnd.randint(20, 100)
    json_string = {'value': str(val), 'place': 'scemo'}
    json_obj = json.dumps(json_string)

    payload = {'json_payload': json_obj}
    r = requests.post(server, data=payload)

    client_connection.send() #il server invia il json all'esp
    clients_names.append( client_name )
    update_client_names_display( clients_names )  # aggiornare la visualizzazione dei nomi dei client
    sleep(1)
    while True:
        data = client_connection.recv(4096)
        if not data: break

        # estrae la scelta del giocatore dai dati ricevuti
        player_choice = data[11:len(data)]

        msg = {
            "choice": player_choice,
            "socket": client_connection
        }
        player_data.append(msg)



# Restituisce l'indice del client corrente nell'elenco dei client
def get_client_index(client_list, curr_client):
    idx = 0
    for conn in client_list:
        if conn == curr_client:
            break
        idx = idx + 1

    return idx


# Aggiorna la visualizzazione del nome del client quando un nuovo client si connette O
# Quando un client connesso si disconnette
def update_client_names_display(name_list):
    tkDisplay.config(state=tk.NORMAL)
    tkDisplay.delete('1.0', tk.END)

    for c in name_list:
        tkDisplay.insert(tk.END, c.decode() + "\n")
    tkDisplay.config(state=tk.DISABLED)

# Cornice superiore composta da due pulsanti (i.e. btnStart, btnStop)
topFrame = tk.Frame(window)
btnStart = tk.Button(topFrame, text="Start", command=lambda : start_server())
btnStart.pack(side=tk.LEFT)
btnStop = tk.Button(topFrame, text="Stop", command=lambda : stop_server(), state=tk.DISABLED)
btnStop.pack(side=tk.LEFT)
topFrame.pack(side=tk.TOP, pady=(5, 0))

# Cornice centrale composta da due etichette per la visualizzazione delle informazioni sull'host e sulla porta
middleFrame = tk.Frame(window)
lblHost = tk.Label(middleFrame, text = "Address: " + str(get_ip()))
lblHost.pack(side=tk.LEFT)
lblPort = tk.Label(middleFrame, text = "Port:"+str(HOST_PORT))
lblPort.pack(side=tk.LEFT)
middleFrame.pack(side=tk.TOP, pady=(5, 0))

# Il frame client mostra l'area dove sono elencati i clients che partecipano al gioco
clientFrame = tk.Frame(window)
lblLine = tk.Label(clientFrame, text="**********Client List**********").pack()
scrollBar = tk.Scrollbar(clientFrame)
scrollBar.pack(side=tk.RIGHT, fill=tk.Y)
tkDisplay = tk.Text(clientFrame, height=10, width=30)
tkDisplay.pack(side=tk.LEFT, fill=tk.Y, padx=(5, 0))
scrollBar.config(command=tkDisplay.yview)
tkDisplay.config(yscrollcommand=scrollBar.set, background="#F4F6F7", highlightbackground="grey", state="disabled")
clientFrame.pack(side=tk.BOTTOM, pady=(5, 10))

window.mainloop()
