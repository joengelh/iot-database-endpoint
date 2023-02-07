#!/usr/local/bin/python3
 
from fastapi import Depends, APIRouter
from sqlalchemy.orm import Session

import crud, schemas
from dependencies import get_db, get_current_username

router = APIRouter()

@router.post("/api/v1/data")
async def signup(
        data: schemas.Data, 
        db: Session = Depends(get_db), 
        username: str = Depends(get_current_username)
    ):
    assert(username != "")
    crud.saveDataToDb(db, data)
