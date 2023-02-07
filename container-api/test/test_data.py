#!/usr/local/bin/python3

import base64
from fastapi.testclient import TestClient
from envs import env

from main import app, engine
from models import Base

client = TestClient(app)

#test signup
def test_auth():
    Base.metadata.drop_all(bind=engine)
    Base.metadata.create_all(bind=engine)

    username = env("BASEAUTH_USERNAME")
    password = env("BASEAUTH_PASSWORD")

    auth = f"{username}:{password}"
    encoded_auth = base64.b64encode(auth.encode()).decode()
    headers = {
    "Authorization": "Basic " + encoded_auth
    }
    response = client.post("/api/v1/data", json={
        "device": "test",
        "float1": 0.0,
        "float2": 0.0,
        "float3": 0.0,
        "float4": 0.0
    }, headers=headers)
    assert response.status_code == 200
