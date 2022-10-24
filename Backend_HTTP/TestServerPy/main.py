from typing import Union
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

class esp32(BaseModel):
    temperature: float
    light: float

app = FastAPI()
origins=["*"]
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

@app.post("/api/data/")
def send_data():
    return {"value":"666", "place":"satana"}

@app.get("/api/data/{item_val}")
def read_esp(item_val: int, q: Union[str, None] = None):
    return {"item_id": item_val, "q": q}


@app.post("/esp32/{tmp_val}&{light_val}")
def getEspData(tmp_val: float, light_val: float):
    return {"temperatura": str(tmp_val), "luce": str(light_val)}

@app.post("/esp32/{id}")
def getEspJSon(id: int, pl: Union[str, None] = None):
    return {pl}

@app.post("/items/")
async def create_item(item: esp32):
    return item

@app.get("/api/data")
def getStronzih():
    return {"scemo":"cretino"}