#!/usr/local/bin/python3

from fastapi import FastAPI

from src import models
from src.database import engine
from src.routers.data import post

models.Base.metadata.create_all(bind=engine)

app = FastAPI()
app.include_router(post.router)

