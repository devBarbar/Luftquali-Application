# Luftquali-Application s

# Zur Nutzung der Application ist Docker benötigt
https://www.docker.com/get-started

Nach dem Installieren die repository clonen und dann im der repository:

`docker-compose -f docker-compose.yml up --build`
`docker-compose up --build`

Starten des React Frontends:

`cd gatsby-admin-template`

`npm install -g gatsby-cli`

`npm install`

`gatsby develop`

`http://localhost:8000` - Dashboard Template

## app
Python Server für die Serverseitige Handhabung
    - Datenbank auslesen
    - Senden der Daten an die Website
    - Empfangen von Nachrichten von der Website
    - basierend auf dem Websocket Protocol


## gatsby-admin-template
React Frontend
    - Nginx 


## zigbee
Microservice zum auslesen der Zigbee Daten:
    - Schreiben in die Datenbank
    - Zur Verfügungstellen der Daten über eine API 


## Database
Mysql Datenbank
