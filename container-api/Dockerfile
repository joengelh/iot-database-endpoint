FROM python:3.9

RUN mkdir /src

WORKDIR /src

COPY ./requirements.txt requirements.txt

RUN pip install --no-cache-dir --upgrade -r requirements.txt

ADD ./src/ /src/

CMD ["uvicorn", "main:app", "--port", "80", "--host", "0.0.0.0", "--log-level", "error"]