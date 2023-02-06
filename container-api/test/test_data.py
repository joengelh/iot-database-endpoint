#!/usr/local/bin/python3

from fastapi.testclient import TestClient

from main import app, engine
from models import Base

client = TestClient(app)

#test signup
def test_auth():
    Base.metadata.drop_all(bind=engine)
    Base.metadata.create_all(bind=engine)
    response = client.post("/api/v1/data", json={
        "deviceId": 1231231,
        "float1": 0.0,
        "float2": 0.0,
        "float3": 0.0,
        "float4": 0.0
    })
    assert response.status_code == 200
