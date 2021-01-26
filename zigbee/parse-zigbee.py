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

# test

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
                           "(ID INT AUTO_INCREMENT PRIMARY KEY, "
                           "SENSOR VARCHAR(90) ,"
                           "DATA_TYPE VARCHAR(45), "
                           "VALUE VARCHAR(128), "
                           "TIMESTAMP TIMESTAMP)")

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
# while True:
add_sensor_data = "INSERT INTO SENSOR_DATA (DATA_TYPE, SENSOR, VALUE, TIMESTAMP) VALUES (%s,%s, %s, %s)"
while i < 5 and not testing:  # only for testing
    today = date.today()
    now = datetime.now().time()
    data = ser.readline(1000)

    if b'Zig' in data:
        i += 1  # only for testing
        sensor_data = (data, 1, today, now)
        cursor.execute(add_sensor_data, sensor_data)
        cnx.commit()

        # only for testing
        sensorData.update({'type': 'test', 'value': data,
                           'date': today, 'time': now})
        print(sensorData)


def fake_data():
    result = []
    types = ["H", "T", "Q"]
    sensorID = ["P0000001", "P0000002", "P00000003"]
    for i in range(0, 3):
        if i == 0:
            values = [random.randint(0, 100) for _ in range(20)]
        elif i == 1:
            values = [random.randint(0, 40) for _ in range(20)]
        elif i == 2:
            values = values = [random.randint(400, 1200) for _ in range(20)]
        result.append(json.dumps({"SENSOR": "%s", "TYPE": "%s", "VALUES": "%s"}) % (
            sensorID[i], types[i], values))
    time.sleep(10)
    return json.dumps(result)


while True and testing:
    sensor_data = json.loads(fake_data())
    print(sensor_data)
    print(type(sensor_data))
    for sensor in sensor_data:

        data = json.loads(sensor)
        print(type(sensor))
        print(type(data))
        print(data)
        params = (data["TYPE"], data["SENSOR"], data["VALUES"], datetime.now())
        cursor.execute(add_sensor_data, params)
        cnx.commit()

cnx.close()  # close db connection
