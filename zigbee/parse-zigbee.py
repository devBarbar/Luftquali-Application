import sys
import serial
import operator
from serial import Serial
from datetime import datetime
from datetime import date
from mysql.connector import (connection)
import serial.tools.list_ports
import time
import json
import random
myports = [tuple(p) for p in list(serial.tools.list_ports.comports())]
print(myports)
testing = True

today = date.today()
now = datetime.now().time()
sensorData = {}
i = 1  # only used for testing
db_config = {
    'user': 'root',
    'password': 'password',
    'host': '127.0.0.1',
    'database': 'mysql_database',
    'raise_on_warnings': True
}
# prepare inserting data into table sensor_data


# establish connection to device
while True and not testing:
    try:
        ser = serial.Serial('COM4', 38400, timeout=1)
        if ser.is_open:
            print(ser.name + ' is open..')
        break
    except (IOError):
        userinput = input('ERROR could not connect to COM port\n'
                          'Please connect device and enter "ok" to continue or "q" to quit: ')
        if userinput != 'ok':
            sys.exit(1)


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
            cursor.execute("CREATE TABLE SENSOR_DATA "
                           "(ID INT(11) NOT NULL AUTO_INCREMENT, "
                           "csUID VARCHAR(40), "
                           "panID VARCHAR(40), "
                           "TMP INT(11), "
                           "HUM INT(11), "
                           "CO2 INT(11), "
                           "VOC INT(11), "
                           "BAT INT(11), "
                           "TIME TIMESTAMP, "
                           "DATE DATE "
                           "PRIMARY KEY (ID, csUID, panID) )")

        cursor.execute("SHOW TABLES")
        for tb in cursor:
            print(tb)

        break
    except Exception as e:
        print(e)
        retry_count += 1
        time.sleep(10)

        userinput = input('ERROR DB connection failed\n'
                          'Please make sure mySQL server is running and/or configuration is correct\n'
                          'Enter "ok" to continue or "q" to quit: ')
        if userinput != 'ok':
            sys.exit(1)


# read data from serial port

# default value für gemessene werte sollte null sein

# while True:
add_sensor_data = "INSERT INTO SENSOR_DATA (csUID, panID, TMP, HUM, CO2, VOC, BAT, TIME, DATE) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)"
while i < 5 and not testing:  # only for testing
    today = date.today()
    now = datetime.now().time()
    data = ser.readline(1000)

    if b'Zig' in data:
        i += 1  # only for testing
        sensor_data = ('0xA000F', '0x12345FFF', 10, 20, 30, 40, 50, now, today)
        cursor.execute(add_sensor_data, sensor_data)
        cnx.commit()

        # only for testing
        sensorData.update({
            'csUID': data,
            'panID': data,
            'TMP': data,
            'HUM': data,
            'CO2': data,
            'VOC': data,
            'BAT': data,
            'time': now,
            'date': today, })

        print(sensorData)


def fake_data():
    result = []
    csUIDs = ["0xA000D", "0xB000D", "0xC000D"]
    panIDs = ["0x12345AAA", "0x12345BBB", "0x12345CCC"]

    tmp = random.randint(0, 100)
    hum = random.randint(0, 80)
    co2 = random.randint(400, 1200)
    voc = random.randint(500, 2000)
    bat = random.randint(0, 100)

    for i in range(0, 3):
        result.append(json.dumps({"csUID": "%s", "panID": "%s", "TMP": "%s", "HUM": "%s", "CO2": "%s", "VOC": "%s", "BAT": "%s"}) % (
            csUIDs[0], panIDs[i], tmp, hum, co2, voc, bat))

    time.sleep(10)
    return json.dumps(result)


while True and testing:
    # today = date.today()
    # now = datetime.now().time()
    sensor_data = json.loads(fake_data())
    print(sensor_data)
    # print(type(sensor_data))

    for sensor in sensor_data:
        data = json.loads(sensor)
        # print(type(sensor))
        # print(type(data))
        print(data)
        params = (data["csUID"], data["panID"], data["TMP"], data["HUM"],
                  data["CO2"], data["VOC"], data["BAT"], now, today)
        cursor.execute(add_sensor_data, params)
        cnx.commit()

cnx.close()  # close db connection
