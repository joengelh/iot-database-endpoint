#!/usr/local/bin/python3

from fastapi import FastAPI

from app import models
from app.database import engine
from app.routers.data import post

models.Base.metadata.create_all(bind=engine)

app = FastAPI()
app.include_router(post.router)

