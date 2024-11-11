import asyncio
import random

# Variabile per tracciare se il timer è attivo
timer_attivo = False

async def avvia_timer():
    global timer_attivo
    timer_attivo = True
    await asyncio.sleep(5)  # Imposta qui la durata del timer
    timer_attivo = False

async def genera_numeri():
    global timer_attivo
    while True:
        numero = random.randint(0, 1)
        print(f"Generato: {numero}")
        
        if numero == 1:
            if not timer_attivo:
                print("È uscito 2! Avvio timer...")
                asyncio.create_task(avvia_timer())
            else:
                print("È uscito 2, ma il timer è ancora attivo.")

        await asyncio.sleep(1) # secondi

# Avvia il ciclo principale
asyncio.run(genera_numeri())