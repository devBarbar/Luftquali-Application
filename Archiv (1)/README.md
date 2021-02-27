# Luftquali-Application

# Zur Nutzung der Application ist Docker benötigt (Für die MYSQL Verbindung)
https://www.docker.com/get-started

Nach dem Installieren von Docker in in dem Ordner, indem die Docker docker-compose Datei ist, folgende Befehle ausführen.

`docker-compose -f docker-compose.yml up --build`

`http://localhost:8000` - Link zum aufrufen des Dashboards


Für das auslesen der Sensordaten, bitte folgende Befehle ausführen.
"`python -m smtpd -n -c DebuggingServer localhost:1025`
`python3 zigbee/parse-zigbee.py`
