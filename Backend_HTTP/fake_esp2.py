import tkinter as tk
from tkinter import ttk
import requests as rs

url = "http://localhost:8000/esp/"

def update_values():
    # Ottieni i valori attuali degli slider
    value1 = slider1.get()
    value2 = slider2.get()
    url = f"http://localhost:8000/esp/both/{value2}&{value1}"
    # Aggiorna l'etichetta con i valori selezionati
    label.config(text=f"Slider 1: {value1}, Slider 2: {value2}")
    response = rs.post(url)



def slider1_changed(val):
    # Funzione per slider 1: stampa il valore selezionato
    print(f"Slider 1 cambiato a: {val}")
    str = f"light/{val}"

def slider2_changed(val):
    # Funzione per slider 2: stampa il valore selezionato
    print(f"Slider 2 cambiato a: {val}")
    str = f"temp/{val}"

# Crea la finestra principale
root = tk.Tk()
root.title("Slider GUI")
root.geometry("300x300")

# Slider 1: con 5 valori (da 1 a 5), con callback quando cambia valore
slider1 = tk.Scale(root, from_=1, to=5, orient='horizontal', label="Slider 1 (5 valori) LUCE", length=200, command=slider1_changed)
slider1.pack(pady=10)

# Slider 2: con 8 valori (da 1 a 8), con callback quando cambia valore
slider2 = tk.Scale(root, from_=1, to=8, orient='horizontal', label="Slider 2 (8 valori) TEMPERATURA", length=200, command=slider2_changed)
slider2.pack(pady=10)

# Etichetta che mostra i valori selezionati
label = ttk.Label(root, text="Slider 1: , Slider 2: ")
label.pack(pady=10)

# Pulsante per aggiornare i valori visualizzati
button = ttk.Button(root, text="Aggiorna Valori", command=update_values)
button.pack(pady=10)

# Avvia la finestra principale
root.mainloop()
