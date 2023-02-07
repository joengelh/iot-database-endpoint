#!/usr/local/bin/python3

from sqlalchemy import DateTime, Column, Float, BigInteger, String
from sqlalchemy.sql import func

from database import Base

class Data(Base):
    __tablename__ = 'data'
    id = Column(BigInteger, primary_key=True)
    device = Column(String)
    float1 = Column(Float)
    float2 = Column(Float)
    float3 = Column(Float)
    float4 = Column(Float)
    date = Column(DateTime, default=func.now())
