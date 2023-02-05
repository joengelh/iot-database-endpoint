#!/usr/local/bin/python3
 
from fastapi import Depends, APIRouter
from sqlalchemy.orm import Session

from src import crud, schemas
from src.dependencies import get_db

router = APIRouter()

@router.post("/api/v1/data")
async def signup(data: schemas.Data, db: Session = Depends(get_db)) -> str:
    return {"customer_uuid": crud.saveDataToDb(db, data)}
