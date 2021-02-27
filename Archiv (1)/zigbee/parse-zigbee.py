import sys
import serial
import operator
from serial import Serial
from datetime import datetime,timedelta
from datetime import date
from mysql.connector import (connection)
import serial.tools.list_ports
import time
import json
import random
from mailer import Mailer
myports = [tuple(p) for p in list(serial.tools.list_ports.comports())]
print(myports)
testing = True
email = Mailer(server="localhost:1025")
# today = date.today()
# now = datetime.now().time()
sensorData = {}
i = 1  # TODO only used for testing
db_config = {
    'user': 'root',
    'password': 'password',
    'host': '127.0.0.1',
    'database': 'mysql_database',
    'raise_on_warnings': True
}


# establish connection to device
x = 0
while True and not testing:
    port = 'COM' + str(x)
    try:
        ser = serial.Serial(port, 38400, timeout=1)
        if ser.is_open:
            print(ser.name + ' is open..')
            break
        if x == 100:
            print('connection to com port failed')
            sys.exit(1)
    except (IOError):
        print(f"ERROR could not connect to {port} port\n")
        if x == 100:
            print('connection to com port failed')
            sys.exit(1)
    x += 1


# establish connection to db
retry_count = 0
while True:
    try:
        time.sleep(10)  # wait for mysql database bootup
        cnx = connection.MySQLConnection(**db_config)
        cursor = cnx.cursor()

        if cnx.is_connected:
            print('conntected to db: ' + cnx._database)

        # check if table exists
        cursor.execute("SHOW TABLES LIKE 'SENSOR_DATA'")
        result = cursor.fetchone()
        if (operator.not_(result)):  # table "sensor_data" exists not
            cursor.execute("CREATE TABLE SENSOR_DATA"
                           "(ID INT AUTO_INCREMENT,"
                           "csUID VARCHAR(45),"
                           "panID VARCHAR(45),"
                           "DATA_TYPE VARCHAR(45),"
                           "VALUE VARCHAR(45),"
                           "TIMESTAMP TIMESTAMP)"
                           "PRIMARY KEY (ID, csUID, panID) )")
        cursor.execute("SHOW TABLES")
        for tb in cursor:
            print(tb)
        break
    except Exception as e:
        print(e)
        retry_count += 1
        time.sleep(10)
        print('ERROR DB connection failed\n'
              'Please make sure mySQL server is running and/or configuration is correct')
        sys.exit(1)


add_sensor_data = ("INSERT INTO SENSOR_DATA ("
                   "csUID, panID, DATA_TYPE, VALUE, TIMESTAMP)"
                   "VALUES (%s, %s, %s, %s, %s)")
add_alert = ("INSERT INTO ALERTS ("
                   "CS_UID, TYPE, MESSAGE, VALUE, TIMESTAMP)"
                   "VALUES (%s, %s, %s, %s, %s)")


# read data from serial port
# {"csUID":"%s","panID":"%s","DATA_TYPE":"%s","VALUE":"%s"}
alert_dict = {}
first_run = datetime.now()
while True and not testing:
    print("---------------------------------")
    print("LIVEMODE")
    print("---------------------------------")
    serial_data = ser.readline(1000)
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    now_date = datetime.now()
    try:
        for key in alert_dict:
            if alert_dict[key] < (now_date-timedelta(hours=1)):
                cursor.execute("SELECT EMAIL FROM USER")
                result = cursor.fetchall()
                try:
                    for mail in result:
                        email.mailer(mail[0],f"Sensor Ueberprufen {key}",mail[0],"Bitte Sensor ueberpruefen")
                except:
                    pass
                params = (key, "AUSFALL","SENSOR "+key+" AUSGEFALLEN", "SENSOR "+key+" AUSGEFALLEN", now)
                cursor.execute(add_alert, params)
                
                print("SENDING MAIL")
            else:
                print("DELETE")
                params = (key,)
                cursor.execute("DELETE FROM ALERTS WHERE CS_UID = %s AND TYPE = 'AUSFALL'",(params))
    except:
        pass
    if b'panID' in serial_data:
        data = json.loads(serial_data)
        print(data)
        print(type(data))
        alert_dict[data["csUID"]] = now_date
        params = (data["csUID"], data["panID"],
                  data["DATA_TYPE"], data["VALUE"], now)
        print(params)
        cursor.execute(add_sensor_data, params)
        cnx.commit()


def fake_data():
    result = []
    dataTypes = ["tmp", "hum", "co2", "voc", "bat"]
    csUIDs = ["0xA000D", "0xB000D", "0xC000D", "0xD000D", "0xE000D"]
    panIDs = ["0x12345AAA", "0x12345BBB", "0x12345CCC"]

    for i in range(0, 5):
        if i == 0:
            value = random.randint(0, 100)
        elif i == 1:
            value = random.randint(0, 80)
        elif i == 2:
            value = random.randint(400, 1200)
        elif i == 3:
            value = random.randint(500, 2000)
        elif i == 4:
            value = random.randint(0, 100)

        result.append(json.dumps(
            {"csUID": "%s", "panID": "%s", "DATA_TYPE": "%s", "VALUE": "%s"})
            % (csUIDs[i], panIDs[0], dataTypes[i], value))

    time.sleep(5)
    return json.dumps(result)

showed_testing = False


add_sensor_data = ("INSERT INTO SENSOR_DATA ("
                   "csUID, panID, DATA_TYPE, VALUE, TIMESTAMP)"
                   "VALUES (%s, %s, %s, %s, %s)")
while True and testing:
    if not showed_testing:
        print("---------------------------------")
        print("TESTMODUS")
        print("---------------------------------")
        showed_testing = True
    sensor_data = json.loads(fake_data())
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    now_date = datetime.now()

    for key in alert_dict:
        cursor.execute("SELECT EMAIL FROM USER")
        result = cursor.fetchall()
        if alert_dict[key] < (now_date-timedelta(hours=1)):
            for mail in result:
                email.mailer(mail[0],f"Sensor Ueberprufen {key}",mail[0],"Bitte Sensor ueberpruefen")
            params = (key, "AUSFALL","SENSOR "+key+" AUSGEFALLEN", "SENSOR "+key+" AUSGEFALLEN", now)
            cursor.execute(add_alert, params)
            
            print("SENDING MAIL")
        else:
            print("DELETE")
            params = (key,)
            cursor.execute("DELETE FROM ALERTS WHERE CS_UID = %s AND TYPE = 'AUSFALL'",(params))
    
    for sensor in sensor_data:
        data = json.loads(sensor)
        print(data)
        alert_dict[data["csUID"]] = now_date
        params = (data["csUID"], data["panID"],
                  data["DATA_TYPE"], data["VALUE"], now)
        cursor.execute(add_sensor_data, params)
        cnx.commit()

cnx.close()  # close db connection
