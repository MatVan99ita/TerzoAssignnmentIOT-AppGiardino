
import tkinter as tk
from tkinter import PhotoImage
import socket
from time import sleep
from tkinter import messagebox as msgb
import threading
import random as rnd

# FINESTRA DI GIOCO PRINCIPALE
window_main = tk.Tk()
window_main.title("Chat game")
your_name = ""
your_class = ""
game_round = 180 #3 minuti di gioco
game_timer = 3 
your_choice = ""
#TOTAL_NO_OF_ROUNDS = 3
your_score = 0
# client di rete
client = None
HOST_ADDR = ''
HOST_PORT = 53001
door_sample=rnd.sample(range(3), 3)
domanda_scompattata=[]
risposta_esatta=""
txtCountDown="" #variabile usata per il controllo sul game_timer


def game_logic(arg):
    global your_choice, client, game_round, your_score, risposta_esatta
    enable_disable_answers("disable")
    if arg==risposta_esatta:
        lbl_answer_choice["text"]="Risposta esatta\n~ +1 Punto"
        your_score+=1
    else:
        lbl_answer_choice["text"]="Mi spiace era la " + str(risposta_esatta) + " quella corretta\n~ -1 Punto"
        your_score-=1
    sleep(1)
    lbl_result["text"]=your_score # aggiorno lo score del player
    
    generaPorte()                 # rigenero il sample delle porte
    enable_disable_doors("enable")# e le riattivo per la scelta del giocatore


def enable_disable_doors(todo):
    if todo == "disable":
        btn_door1.config(state=tk.DISABLED)
        btn_door2.config(state=tk.DISABLED)
        btn_door3.config(state=tk.DISABLED)
    else:
        btn_door1.config(state=tk.NORMAL)
        btn_door2.config(state=tk.NORMAL)
        btn_door3.config(state=tk.NORMAL)

def enable_disable_answers(todo):
    if todo == "disable":
        btn_Scelta1.config(state=tk.DISABLED)
        btn_Scelta2.config(state=tk.DISABLED)
    else:
        btn_Scelta1.config(state=tk.NORMAL)
        btn_Scelta2.config(state=tk.NORMAL)

def enable_question_frame():
    lbl_questioID.pack()
    lbl_question.pack()
    lbl_answers.pack()


def connect():
    global your_name
    if len(ent_name.get()) < 1:
        tk.messagebox.showerror(title="ERROR!!!", message="You MUST enter your first name <e.g. John>")
    elif len(ent_IPAdress.get()) < 8:
        tk.messagebox.showerror(title="ERROR!!!", message="You MUST enter a valid ip <e.g. 255.255.255.255>")
    else:
        print("prova a connettersi\n")
        your_name = ent_name.get()
        connect_to_server(your_name)


def count_down(my_timer, nothing):#timer usato per far partire il gioco
    lbl_game_round["text"] = "Game starts in"
    while my_timer > 0:
        my_timer = my_timer - 1
        print("game timer is: " + str(my_timer))
        lbl_timer["text"] = my_timer
        sleep(1)
    enable_disable_doors("enable")

def game_count_down(self, nothing):#simile a count_down() ma è il timer di gioco
    global game_round
    lbl_game_round["text"] = "Tempo rimanente:"
    self=game_round
    while self > 0:
        self = self - 1
        lbl_timer["text"] = self
        sleep(1)
    lbl_timer["Text"] = "Tempo scaduto!"
    enable_disable_doors("disable")
    enable_disable_answers("disable")
    exit_trapdoor()

def door_choice(arg):
    global your_choice, client, your_score, your_name, your_class, domanda_scompattata
    enable_disable_doors("enable")
    switcher={
        0: "Domanda",
        1: "Domanda",
        2: "Trabocchetto"
    }
    if client:
        lbl_door_choice["text"] = "La porta scelta è la numero " + str(arg+1) + " che corrisponde a: \n" + switcher.get(arg, "Invalid choice")
        if(arg==2):
            exit_trapdoor()#chiamo la funzione per far uscire l'utente
        else:
            #invio al server una richiesta di domanda e poi mi redirigo verso il game logic
            client.send(bytes("$domanda", "utf-8"))
        client.send(bytes(your_choice, "utf8")) #il server invia la scelta della porta al server
        print(domanda_scompattata)
        enable_disable_doors("disable")
        enable_disable_answers("enable")
    generaPorte()

#funzione usata per rigenerare il sample delle porte una volta che sono scelte per cambiarle
def generaPorte():
    door_sample=[]
    door_sample=rnd.sample(range(3), 3)


def connect_to_server(name):
    global client, HOST_PORT, HOST_ADDR, your_name
    try:
        HOST_ADDR=str(ent_IPAdress.get())
        ent_IPAdress.destroy()
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((HOST_ADDR, HOST_PORT))
        client.send(name.encode()) # Invia il nome al server dopo la connessione
        # disable widgets
        btn_connect.config(state=tk.DISABLED)
        btn_quit.config(state=tk.NORMAL)
        ent_name.config(state=tk.DISABLED)
        lbl_name.config(state=tk.DISABLED)
        enable_disable_doors("disable")
        enable_disable_answers("disable")

        # avvia un thread per continuare a ricevere messaggi dal server
        # non bloccare il thread principale
        threading._start_new_thread(receive_message_from_server, (client, "m"))
    except Exception as e:
        tk.messagebox.showerror(title="ERROR!!!", message="Cannot connect to host: " + HOST_ADDR + " on port: " + str(HOST_PORT) + " Server may be Unavailable. Try again later")
        print(e)


def receive_message_from_server(sck, m):
    global your_name, your_choice, your_score, your_class, risposta_esatta, txtCountDown

    while True:
        from_server = sck.recv(4096)
        if not from_server: break

        if txtCountDown=="":
            threading._start_new_thread(count_down, (game_timer, ""))

        if from_server.startswith("z".encode()):#inizio del gioco una volta loggato
            your_class=from_server.decode("utf-8")#ottiene il ruolo dal server
            your_class=your_class[1:]#genero il ruolo senza carattere di riconoscimento
            
            #attivo una prte di grafica
            top_frame.pack()
            middle_frame.pack()
            lbl_welcome["text"] = "Salve " + your_name + "! Il tuo ruolo sara' " + your_class + ". Ora scegli una porta"
            lbl_your_name["text"] = "PC:" + your_name + "\n~" + your_class

            lbl_welcome.config(state=tk.DISABLED)
            lbl_line_server.config(state=tk.DISABLED)

            #faccio partire il thread con il timer di gioco una volta che il conto alla rovescia ha fatto
            sleep(game_timer)
            txtCountDown="#La usiamo come stringa di controllo fermando il count down per l'inzio del gioco"
            
            #generazione timer di gioco
            th_timer=threading.Thread(target=game_count_down, args=(game_round, ""))#creo il thread che farà da timer
            th_timer.daemon = True  #il thread viene fatto girare in background in modo da lasciare che il gioco prosegua
            th_timer.start()

        elif from_server.startswith("~".encode()):#inizio per le domande
            print("~domanda ricevuta\n")
            global risposta_esatta
            config_domanda_server=from_server.decode("utf-8")#ricevo la stringa con la domanda formattata
            domanda_scompattata=config_domanda_server.split('~')#scompatto la domanda
                                                                    #la stringa scompattata risulterà
                                                                    #['id', 'domanda', 'risposte', 'risposta esatta']
            print(config_domanda_server)
            print(domanda_scompattata)
            risposta_esatta=domanda_scompattata[4]#salvo la risposta esatta per poter fare poi il confronto con la risposta dell'utente

            #genero la grafica con le domande
            lbl_questioID["text"] = "Domanda n°" + domanda_scompattata[1]   #id della domanda
            lbl_question["text"]  = domanda_scompattata[2]                  #corpo della domanda
            lbl_answers["text"]   = domanda_scompattata[3]                  #risposte per l'utente
            enable_disable_answers("enable")    #attivo le risposte
            enable_question_frame()             #attivo la grafica delle domande
    sck.close()

#funzione che verrà usata per uscire dal gioco
def exit_trapdoor():
    global client
    msgb.showerror(title="Botola della morte per te " + your_name + " - " + your_class, message="Alla fine hai ottenuto: " + str(your_score) + " punti\nMa ora è il tempo di botolarti, alla prossima... se soppravvivrai")
    client.send(bytes("$quit", "utf-8"))
    client.close()
    window_main.destroy()


################# GRAFICA #################################################################################################################################################

top_welcome_frame= tk.Frame(window_main)


new_host_frame=tk.Frame(top_welcome_frame)
lbl_name = tk.Label(new_host_frame, text = "Name:")
lbl_name.pack(side=tk.LEFT)
ent_name = tk.Entry(new_host_frame)
ent_name.pack(side=tk.LEFT)
new_host_frame.pack(side=tk.TOP)


new_ip_frame=tk.Frame(top_welcome_frame)
lbl_IPAdress = tk.Label(new_ip_frame, text = "Insert server ip:")
lbl_IPAdress.pack(side=tk.LEFT)
ent_IPAdress = tk.Entry(new_ip_frame)
ent_IPAdress.pack(side=tk.LEFT)
new_ip_frame.pack(side=tk.BOTTOM)


button_connection_frame=tk.Frame(top_welcome_frame)
btn_connect = tk.Button(button_connection_frame, text="Connect", command=lambda : connect())
btn_connect.pack(side=tk.LEFT)
btn_quit = tk.Button(button_connection_frame, text="Quit", command=lambda : exit_trapdoor(), state=tk.DISABLED)
btn_quit.pack(side=tk.LEFT)
button_connection_frame.pack(side=tk.BOTTOM)

top_welcome_frame.pack(side=tk.TOP)


top_message_frame = tk.Frame(window_main)
lbl_line = tk.Label(top_message_frame, text="***********************************************************").pack()
lbl_welcome = tk.Label(top_message_frame, text="")
lbl_welcome.pack()
lbl_line_server = tk.Label(top_message_frame, text="***********************************************************")
lbl_line_server.pack_forget()
top_message_frame.pack(side=tk.TOP)


top_frame = tk.Frame(window_main)
top_left_frame = tk.Frame(top_frame, highlightbackground="green", highlightcolor="green", highlightthickness=1)
lbl_your_name = tk.Label(top_left_frame, text="Your name: " + your_name, font = "Helvetica 13 bold")
lbl_your_name.grid(row=0, column=0, padx=5, pady=8)
top_left_frame.pack(side=tk.LEFT, padx=(10, 10))


top_right_frame = tk.Frame(top_frame, highlightbackground="green", highlightcolor="green", highlightthickness=1)
lbl_game_round = tk.Label(top_right_frame, text="Game round (x) starts in", foreground="blue", font = "Helvetica 14 bold")
lbl_timer = tk.Label(top_right_frame, text=" ", font = "Helvetica 24 bold", foreground="blue")
lbl_game_round.grid(row=0, column=0, padx=5, pady=5)
lbl_timer.grid(row=1, column=0, padx=5, pady=5)
top_right_frame.pack(side=tk.RIGHT, padx=(10, 10))

top_frame.pack_forget()

middle_frame = tk.Frame(window_main)

lbl_line = tk.Label(middle_frame, text="***********************************************************").pack()
lbl_line = tk.Label(middle_frame, text="**** Quale porta scegli, la 1, la 2 o la THREH? ****", font = "Helvetica 13 bold", foreground="blue").pack()
lbl_line = tk.Label(middle_frame, text="***********************************************************").pack()

round_frame = tk.Frame(middle_frame)
lbl_round = tk.Label(round_frame, text="Round")
lbl_round.pack()

#door
lbl_door_choice = tk.Label(round_frame, text="Your choice: " + your_choice, font = "Helvetica 13 bold")
lbl_door_choice.pack()
button_frame2 = tk.Frame(middle_frame)
btn_Scelta1 = tk.Button(button_frame2, text="Risposta 1", command=lambda : game_logic("1"), state=tk.DISABLED)
btn_Scelta2 = tk.Button(button_frame2, text="Risposta 2", command=lambda : game_logic("2"), state=tk.DISABLED)
btn_Scelta1.grid(row=0, column=0)
btn_Scelta2.grid(row=0, column=2)
button_frame2.pack(side=tk.BOTTOM)
lbl_answer_choice = tk.Label(round_frame, text="Risposta data" + "corretto/Sbaglito", font = "Helvetica 13 bold")
lbl_answer_choice.pack()

lbl_result_text = tk.Label(round_frame, text="Punti ottenuti: ", foreground="blue", font = "Helvetica 14 bold")
lbl_result_text.pack()
lbl_result = tk.Label(round_frame, text=" 0", foreground="blue", font = "Helvetica 14 bold")
lbl_result.pack()
round_frame.pack(side=tk.TOP)

#frame per le generazione della domanda
final_frame = tk.Frame(middle_frame)
lbl_line = tk.Label(final_frame, text="***********************************************************").pack()
lbl_questioID = tk.Label(final_frame, text="_", font = "Helvetica 10 bold")
lbl_questioID.pack()
lbl_question = tk.Label(final_frame, text="_", font = "Helvetica 10 bold")
lbl_question.pack()
lbl_answers = tk.Label(final_frame, text="_", font = "Helvetica 10 bold")
lbl_answers.pack()
lbl_line = tk.Label(final_frame, text="***********************************************************").pack()
final_frame.pack(side=tk.TOP)       #mostro il frame completo

middle_frame.pack_forget()

#frame per i bottoni con le porte
button_frame = tk.Frame(window_main)
door_img = PhotoImage(file="door.png")
btn_door1 = tk.Button(button_frame, text="Door 1", command=lambda: door_choice(door_sample[0]), state=tk.DISABLED, image=door_img)
btn_door2 = tk.Button(button_frame, text="Door 2", command=lambda: door_choice(door_sample[1]), state=tk.DISABLED, image=door_img)
btn_door3 = tk.Button(button_frame, text="Door 3", command=lambda: door_choice(door_sample[2]), state=tk.DISABLED, image=door_img)

btn_door1.grid(row=0, column=0)
btn_door2.grid(row=0, column=1)
btn_door3.grid(row=0, column=2)
button_frame.pack(side=tk.BOTTOM)

####################################################################################################################################################################################################################################################################################################################################

window_main.mainloop()
