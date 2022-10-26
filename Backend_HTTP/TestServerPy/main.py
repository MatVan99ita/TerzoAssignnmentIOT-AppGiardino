from types import new_class
from typing import Union
from webbrowser import get
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel


class Esp32:
    temperature: float
    light: float

    def __init__(self, **data: BaseModel):
        super().__init__(**data)
        self.temperature = 0.0
        self.light = 0.0

    # getter method
    def get_temp(self):
        return self.temperature
    def get_light(self):
        return self.light

    # setter method
    def set_age(self, x):
        self._age = x

        # setter method
    def set_temp(self, x):
        self.temperature = x
    def set_light(self, x):
        self.light = x





esp = Esp32()

app = FastAPI()
origins = ["*"]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/items/{item_id}")
def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}

@app.get("/api/data/{item_val}")
def read_esp(item_val: int, q: Union[str, None] = None):
    return {"item_id": item_val, "q": q}

@app.post("/items/")
async def create_item(item: BaseModel):
    esp.set_light(2.0)
    esp.set_temp(2.0)
    print(item)
    return item


@app.get("/esp/data/")
def getStronzih():
    return {"temperatura": esp.get_temp(), "lux": esp.get_light()}
