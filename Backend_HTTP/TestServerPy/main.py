import string
from types import new_class
from typing import Union
from webbrowser import get

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Json

class Esp32(BaseModel):
    temperature: float = 0.0
    light: float = 0.0
    arduino_status = "AUTO"         # AUTO       / MANUAL / ERROR
    irrigation_state = "ATTIVABILE" # ATTIVABILE / PAUSA  / MOVIMENTO

    # GETTER
    def get_temp(self):
        return self.temperature

    def get_light(self):
        return self.light

    def get_status(self):
        return self.arduino_status

    def get_irrigation_state(self):
        return self.irrigation_state


    # SETTER
    def set_temp(self, x):
        self.temperature = x

    def set_light(self, x):
        self.light = x

    def set_status(self, x):
        self.arduino_status = x

    def set_irrigation_state(self, x):
        self.irrigation_state = x



esp = Esp32()

# esp = Esp32()

app = FastAPI()
origins = ["*"]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
"""
La luce è mappata su 8 valori di cui se è sotto 5 accende i led e sotto 2 attiva l'irrigazione
La temperatura è mappata su 5 valori di cui in base al valore calcolato si determina la velocità di rotazione del servo per Y secondi e poi non potrà essere riattivato per X minuti

Se la temperatura è uguale a 5 e il servo è in pausa si attiva l'alarm mode -> led dell'esp va spento
"""

@app.get("/")
async def read_root():
    return {"Hello": "World"}


@app.get("/items/{item_id}")
async def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}


@app.get("/api/data/{item_val}")
async def read_esp(item_val: int, q: Union[str, None] = None):
    return {"item_id": item_val, "q": q}


@app.put("/items/")
async def create_item(item: Esp32):
    esp.set_light(item.get_temp())
    esp.set_temp(item.get_light())
    return item

@app.post("/esp/temp/{val}")
async def setTemp(val: float):
    esp.set_temp(val)
    #return {"temperatura": esp.get_temp()}

@app.post("/esp/light/{val}")
async def setLight(val: float):
    esp.set_light(val)
    return checkLight(val)

@app.post("/esp/both/{temp}&{light}")
async def update_item(temp: float, light: float):
    esp.set_temp(temp)
    esp.set_light(light)
    checktmp = checkTemp(temp)
    checklgt = checkLight(light)
    return {"irrigazione": checktmp, "illuminazione": checklgt}

@app.get("/esp/data/")
async def getSensorData():
    return {"temperatura": esp.get_temp(), "lux": esp.get_light()}

@app.get("/arduino/status/")
async def getStatus():
    return {"status": esp.get_status()}

@app.post("/arduino/status/{status}")
async def setStatus(status):
    esp.set_status(status)

@app.get("/arduino/irrigation/")
async def getIrrigationStatus():
    return {"irrigazione": esp.get_irrigation_state()}

@app.post("/arduino/irrigation/{status}")
async def setIrrigationStatus(status):
    esp.set_irrigation_state(status)

def checkLight(val):
    if(val < 2):
        return {"irrigazione": "on", "illuminazione": "on"}
    if(val < 5):
        return {"irrigazione": "off", "illuminazione": "off"}
    return {"ok": "ok"}

def checkTemp(val):
    return {"rotation": 7}

def checkIrrigation():
    temp = esp.get_temp()
    irr = esp.get_irrigation_state()
    if temp < 5 and irr == "PAUSA":
        esp.set_status("ERROR")
    pass

