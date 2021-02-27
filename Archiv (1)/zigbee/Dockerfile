FROM python:3.8.6-buster

WORKDIR /zigbee
ENV VIRTUAL_ENV=/venv/
ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN python3 -m venv $VIRTUAL_ENV
# make directory for application

#install dependencies
COPY req.txt .

RUN pip install -r req.txt

COPY /parse-zigbee.py .


CMD ["python", "parse-zigbee.py"]
