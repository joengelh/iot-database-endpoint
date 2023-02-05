#!/usr/local/bin/python3
 
from fastapi import Depends
from sqlalchemy.orm import Session

router = APIRouter()

from app import crud, schemas
from app.dependencies import get_db

@router.post("/api/v1/data")
async def signup(data: schemas.Data, db: Session = Depends(get_db)) -> str:
    return {"customer_uuid": crud.saveDataToDb(db, data)}
