import sys
import serial
import operator
from serial import Serial
from datetime import datetime
from datetime import date
from mysql.connector import (connection)


today = date.today()
now = datetime.now().time()
sensorData = {}
i = 1 # only used for testing
db_config = {
    'user': 'root',
    'password': 'password',
    'host': '127.0.0.1',
    'database': 'luft_quali',
    'raise_on_warnings': True
}
#prepare inserting data into table sensor_data
add_sensor_data = "INSERT INTO sensor_data (data_type, value, date, time) VALUES (%s, %s, %s, %s)"


# establish connection to device
while True:
    try:
        ser = serial.Serial('COM4', 38400, timeout=1)
        if ser.is_open:
            print(ser.name + ' is open..')
        break
    except (IOError):
        userinput = input('ERROR could not connect to COM port\n'
                'Please connect device and enter "ok" to continue or "q" to quit: ')
        if userinput != 'ok': sys.exit(1)


#establish connection to db
while True:
    try:
        cnx = connection.MySQLConnection(**db_config)
        cursor = cnx.cursor()

        if cnx.is_connected:
            print('conntected to db: ' + cnx._database)

        #check if table exists
        cursor.execute("SHOW TABLES LIKE 'sensor_data'")
        result = cursor.fetchone()
        if (operator.not_(result)): #table "sensor_data" exists not
            cursor.execute("CREATE TABLE sensor_data "
                            "(id INT AUTO_INCREMENT PRIMARY KEY, "
                            "data_type VARCHAR(45), "
                            "value INT, "
                            "date DATE, "
                            "time TIME)")

        cursor.execute("SHOW TABLES")
        for tb in cursor:
            print(tb)

        break
    except:
        userinput = input('ERROR DB connection failed\n'
                'Please make sure mySQL server is running and/or configuration is correct\n'
                'Enter "ok" to continue or "q" to quit: ')
        if userinput != 'ok': sys.exit(1)


#read data from serial port
#while True:
while i<5: #only for testing
    today = date.today()
    now = datetime.now().time()
    data = ser.readline(1000)

    if b'Zig' in data:
        i+=1 #only for testing
        sensor_data = (data, 1, today, now)
        cursor.execute(add_sensor_data, sensor_data)
        cnx.commit()

        #only for testing
        sensorData.update({'type': 'test', 'value': data, 'date': today, 'time': now})
        print(sensorData)



cnx.close() #close db connection