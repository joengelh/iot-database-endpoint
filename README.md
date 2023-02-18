# iot-database-endpoint

This is a general repository capable of spinning up a highly flexible endpoint for IoT devices.

## Routes

| Table  | Route          | Methods |
| ------ | -------------- | ------- |
| `data` | `/api/v1/data` | `POST`  |

## Swagger Documentation

Once deployed, FastApi automatically renders a Swagger API documentation at **https://<your_domain>/docs** for developers to read.

## Configuration

For the email Templates to render successfully, copy `logo/logo_green.png` to `/dat`so it can be loaded from the web.
All configurations are done using the `.env` file. It can be copied from `sampledotenv`.

```bash
POSTGRES_NAME=postgres
POSTGRES_USER=postgres
POSTGRES_PASSWORD=
POSTGRES_HOST=db
POSTGRES_PORT=5432
BASEAUTH_USERNAME=root
BASEAUTH_PASSWORD=
```

## SSL

This project contains a pair of microservices that enable automated SSL encryption.
The NGINX reverse proxy labelded **webserver** can be fed with configuration files that can be genenerated with the **nginx/apps.json** and the **nginx/config_generator.py** files.

Whenever a new certificate file has to be generated due to a domain migration, the nginx config files have to modified accordingly and the script **init-letsencrypt.sh** has to be run.
Only one service can be encrypted at a time. Thus, only one config file can be modified at a time aswell.

## Security

The API is protected with http BaseAuth, using the username **root** and a password.
This ensures that only authorized IoT devices can post to the database.

## Deployment

All components are deployed using docker-compose.

```bash
docker-compose up -d --build
```

If you want to run the application without docker-compose, the .env vars have to be set manually using the following command:

```bash
export $(echo $(cat ../.env | sed 's/#.*//g' | sed 's/\r//g' | xargs) | envsubst)
```

## Tests

:warning: **Environment Variables**: Make sure to use diffrent variables for the test databases, as located in `test/__init__.py`.

All have been developed in a test-driven development (TDD) style.
This makes the entire application testable and easy to maintain.
In order to run the tests, you must first install the dependencies:

```bash
pip install -r requirements.txt
```

Just like when deploying the app manually, the .env vars have to be set manually using the following command:

```bash
export $(echo $(cat ../.env.test | sed 's/#.*//g' | sed 's/\r//g' | xargs) | envsubst)
```

The tests are located in the `test` directory.
Run them using:

```bash
python3 -m pytest
```

## Backup & restore

### Backup

To get a backup of the Postgres database, run the command below after inserting the correct password.

```bash
sudo docker exec -i iot-database-endpoint_db_1 /bin/bash -c "PGPASSWORD=password pg_dump --username postgres postgres" > dump.sql
```

SSL certificates are stored in `./certbot` folder, they are automatically renewed from LetsEncrypt every month.

### Restore

In order to load a backup run the command below after inserting the right password

```bash
sudo docker-compose up -d db
sudo docker exec -i iot-database-endpoint_db_1 /bin/bash -c "PGPASSWORD=password psql --username postgres postgres" < dump.sql
sudo docker-compose up -d--build
```
