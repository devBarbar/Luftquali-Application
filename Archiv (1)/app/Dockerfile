FROM python:3.8.6-buster

WORKDIR /app
ENV VIRTUAL_ENV=/venv/
ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN python3 -m venv $VIRTUAL_ENV
# make directory for application

#install dependencies
COPY requirenments.txt .

RUN pip install -r requirenments.txt

COPY /app.py .


CMD ["python", "app.py"]
