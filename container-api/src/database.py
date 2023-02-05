#!/usr/local/bin/python3

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from envs import env

SQLALCHAMY_DATABASE_URL = (
    "postgresql://" + 
    env("POSTGRES_USER") + 
    ":" + 
    env("POSTGRES_PASSWORD") + 
    "@" + 
    env("POSTGRES_HOST") +
    ":" +
    env("POSTGRES_PORT") +
    "/" + 
    env("POSTGRES_NAME")
)

engine = create_engine(SQLALCHAMY_DATABASE_URL, pool_size=50, echo=False)

SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

Base = declarative_base()