#!/usr/local/bin/python3

from fastapi import HTTPException
from sqlalchemy.orm import Session
import schemas, models

def saveDataToDb(db: Session, data: schemas.Data, dataUuid: str):
    db.add(models.Data(
        uuid=dataUuid,
        deviceId=data.deviceId,
        float1=data.float1,
        float2=data.float2,
        float3=data.float3,
        float4=data.float4
    ))
    try:
        db.commit()
    except:
        db.rollback()
        raise HTTPException(status_code=500, detail="Database Error")
    return dataUuid
