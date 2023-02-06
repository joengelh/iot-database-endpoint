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
os.environ["POSTGRES_NAME"] = "postgres"
os.environ["POSTGRES_USER"] = "postgres"
os.environ["POSTGRES_PASSWORD"] = "3910287467803549ASD318112312SADF"
os.environ["POSTGRES_HOST"] = "db.profitablestaking.com"
os.environ["POSTGRES_PORT"] = "80"

