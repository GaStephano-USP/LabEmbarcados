from fastapi import FastAPI
from fastapi.param_functions import Body
from pydantic import BaseModel
from typing import List
from manage import insert, insert_fall

app = FastAPI()

class Post(BaseModel):
    temperature: float
    oximetry: int
    bpm: int

@app.post("/insertentry")
def insert_entry(new_post: Post):
    result = insert(new_post.temperature, new_post.oximetry, new_post.bpm)
    return(result)

@app.post("/insertentryfall")
def insertFall():
    result = insert_fall()
    return(result)