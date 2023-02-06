#!/usr/local/bin/python3
 
from fastapi import Depends, APIRouter
from sqlalchemy.orm import Session
import uuid

import crud, schemas
from dependencies import get_db

router = APIRouter()

@router.post("/api/v1/data")
async def signup(data: schemas.Data, db: Session = Depends(get_db)) -> schemas.Uuid:
    dataUuid = str(uuid.uuid4())
    crud.saveDataToDb(db, data, dataUuid)
    return {"uuid": dataUuid}
