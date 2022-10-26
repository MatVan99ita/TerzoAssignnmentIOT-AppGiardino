from types import new_class
from typing import Union
from webbrowser import get

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Json


class Esp32(BaseModel):
    temperature: float = 0.0
    light: float = 0.0

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


@app.get("/esp/data/")
async def getStronzih():
    return {"temperatura": esp.get_temp(), "lux": esp.get_light()}
