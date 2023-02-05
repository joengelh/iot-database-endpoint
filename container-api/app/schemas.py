#!/usr/local/bin/python3

from pydantic import BaseModel

class Data(BaseModel):
    float1: float
    float2: float
    float3: float
    float4: float