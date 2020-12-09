# Luftquali-Application s

# Zur Nutzung der Application ist Docker benötigt

docker-compose -f docker-compose.yml up


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
Microservice zum auslesen der Zigbee Daten und schreiben in die Datenbank

## Database
Mysql Datenbank