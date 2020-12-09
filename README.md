# Luftquali-Application s

# Zur Nutzung der Application ist Docker benötigt
https://www.docker.com/get-started

Nach dem Installieren die repository clonen und dann im der repository:
`docker-compose -f docker-compose.yml up`


## app
Python Server für die Serverseitige Handhabung
    - Datenbank auslesen
    - Senden der Daten an die Website
    - Empfangen von Nachrichten von der Website
    - basierend auf dem Websocket Protocol

## static
Html Website mit einer Verbindung zum Python Server
    - Nginx 

## zigbee
Microservice zum auslesen der Zigbee Daten:
    - Schreiben in die Datenbank
    - Zur Verfügungstellen der Daten über eine API 

## Database
Mysql Datenbank