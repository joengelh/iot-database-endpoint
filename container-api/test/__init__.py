#!/usr/local/bin/python3

import os
import sys

#add project path
PROJECT_PATH = os.getcwd()
SOURCE_PATH = os.path.join(
    PROJECT_PATH,"src/"
)
sys.path.append(SOURCE_PATH)

#overwrite env vars to use the test db
os.environ["POSTGRES_HOST"] = "20.251.9.84"
os.environ["POSTGRES_PORT"] = "9999"

