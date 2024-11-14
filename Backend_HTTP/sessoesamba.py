import bluetooth

def find_hc05():
    print("Cercando dispositivi Bluetooth...")

    # Esegue la scansione dei dispositivi vicini
    nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True, flush_cache=True)

    hc05_address = None
    for addr, name in nearby_devices:
        print(f"Trovato dispositivo: {name} con MAC Address: {addr}")
        # Verifica se il nome del dispositivo contiene "HC-05"
        if "HC-05" in name:
            hc05_address = addr
            print(f"Trovato dispositivo HC-05! MAC Address: {hc05_address}")
            break

    if hc05_address is None:
        print("Nessun dispositivo HC-05 trovato.")
    else:
        print(f"MAC Address del dispositivo HC-05: {hc05_address}")

    return hc05_address

# Esegui la funzione
find_hc05()