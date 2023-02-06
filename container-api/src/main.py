#!/usr/local/bin/python3

from fastapi import FastAPI

import models
from database import engine
from routers.data import post

models.Base.metadata.create_all(bind=engine)

app = FastAPI()
app.include_router(post.router)

