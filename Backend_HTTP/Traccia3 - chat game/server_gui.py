

#importiamo i moduli che utilizzeremo
import tkinter as tk
import socket
import threading
from time import sleep
import random as rnd

def server_getIp():# funzione usata per ottenere l'ip del server che servirà poi al client per connettersi
    global HOST_ADDR
    hostname=socket.gethostname()
    s=socket.gethostbyname(hostname)
    return(s)


server = None
HOST_ADDR = ""
HOST_PORT = 53001
client_name = " "
clients = []
clients_names = []
player_data = []

window = tk.Tk()
window.title("Server")




#Le domande sono solo 20 perchè è solo per testare il gioco con domande casuali
def genera_domande():
    questionID=rnd.randint(1, 20)
    questioner={
        1:  "1~Qual e' la radice quadrata di 2?~1)1.412 - 2)1.4141~1",
        2:  "2~Quante sono le classi nel manuale base del giocatore di D&D 5e?~1)10 - 2)12~2",
        3:  "3~Quanti sono i vangeli apocrifi secondo la chiesa cristiana?~1)8 - 2)10~1",
        4:  "4~Quanti sono i film di Star Wars usciti?~1)9 - 2)11~2",
        5:  "5~Qual e' la risposta fondamentale alla domanda fondamentale?~1)42 - 2)23~1",
        6:  "6~Quanti sono attualmente i Pokemon nel pokedex(aggiornato al 2021)?~1)890 - 2)898~2",
        7:  "7~In che periodo ci fu la Grande Guerra?~1)1914-'18 - 2)1915-'18~1",
        8:  "8~Quanti sono i Warcraft usciti?~1)3 - 2)8~2",
        9:  "9~Qual e' l'attuale serie di Dragon Ball in corso?~1)GT - 2)Super~2",
        10: "10~Manfred von Richthofen detto il Barone Rosso quando fu abbattuto?~1)21 Aprile 1918 - 2) 1 Ottobre 1918~1",
        11: "11~Qual e' l'ultima versione attualmente disponibile di python?~1)Python v-3.9.6 - 2)Pyhton v-3.9.7~1",
        12: "12~In che data si presentò nelle tv italiane il programma \"Striscia la Notizia\"?~1)11 Dicembre 1989 - 2)7 Novembre 1988~2",
        13: "13~Quando ci fu la prima comparsa del Gabibbo?~1)Paperissima Sprint(1995) - 2)Striscia la Notizia(1990)~2",
        14: "14~Quando fu pubblicato il primo manuale di D&D?~1)1981 2)1974~2",
        15: "15~In quale Resident Evil esordisce il tyrant conosciuto come Nemesis?~1)Resident Evil 2 - 2)Resident Evil 3~2",
        16: "16~In quale Silent Hill compare per la prima volta Piramid Head?~1)Silent Hill 2 - 2)Silent Hill 3~1",
        17: "17~Quando si formarono i Litfiba?~1)1978 - 2)1980~2",
        18: "18~Quando si formarono i Nomadi?~1)1963 - 2)1964~1",
        19: "19~Quando si formarono i Depeche Mode?~1)1980 - 2)1979~1",
        20: "20~Quando fu pubblicato lo standard IPv4?~1)Settembre 1980 - 2)Settembre 1981~2"
    }
    #la stringa è già formattata per l'invio al client con il formato
    #[ 'ID', 'corpo', 'risposte', 'risposta esatta' ] usando '~' come separatore per l'invio
    return questioner.get(questionID, "Invalid questionID")

def player_class_generator():
    classID=rnd.randrange(1, 12, 1)
    classer = {
        1:  "Barbaro",
        2:  "Bardo",
        3:  "Chierico",
        4:  "Druido",
        5:  "Guerriero",
        6:  "Ladro",
        7:  "Mago",
        8:  "Monaco",
        9:  "Paladino",
        10: "Ranger",
        11: "Stregone",
        12: "Warlock"
    }
    return classer.get(classID, "Invalid class")


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


# Funzione per ricevere messaggi dal client corrente E
# Invia quel messaggio agli altri client
def send_receive_client_message(client_connection, client_ip_addr):
    global server, client_name, clients, player_data, idx

    
    idx = get_client_index(clients, client_connection)
    client_name = client_connection.recv(4096)

    client_class=player_class_generator() #genera il ruolo per l'utente
    client_connection.send(bytes("z"+client_class, "utf8")) #il server invia il ruolo all'utente
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

        #controllo sulla richiesta del client se sceglie la domanda o la botola
        if(player_choice=="$quit"):
            client_connection.send(bytes("!Sei caduto nella mia trappola", "utf8"))
            # trova l'indice del client, quindi lo rimuove da entrambi gli elenchi (elenco dei nomi dei client e elenco delle connessioni)
            del clients_names[idx]
            del clients[idx]
            client_connection.close()
            update_client_names_display(clients_names)  # aggiorna la visualizzazione dei nomi dei client
        else:
            print("richiesta domanda\n")
            config_domanda="~"+genera_domande()
            print(config_domanda)
            client_connection.send(bytes(config_domanda, "utf8"))



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
















