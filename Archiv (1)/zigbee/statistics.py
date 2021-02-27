# from C:\Users\boost\Documents\uni\informatikProjekt\luftQuali_0.5\app\models.py import func_name
# app\models.py
# from .app.models import SensorData, SensorDataWeek, SensorDataBiWeek, SensorDataMonth
from sqlalchemy import create_engine, Column, Integer, String, ForeignKey, JSON, Time, Date, TIMESTAMP
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
from mysql.connector import (connection)
from datetime import datetime
import time
import json
import random
Base = declarative_base()


testing = False
today = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
# today = datetime.today().date()
# now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
count_sunday = 0  # counter for the bi-weekly stats

# preparing feed for database
engine = create_engine(
    "mysql+mysqlconnector://root:password@mysql_database:3306/mysql_database", echo=True)
Session = sessionmaker(bind=engine)
session = Session()
add_sensor_data_daily = ("csUID, panID, DATA_TYPE, VALUE, TIMESTAMP)"
                         "VALUES (%s, %s, %s, %s, %s)")
add_sensor_data_weekly = ("csUID, panID, DATA_TYPE, VALUE, WEEK_NUMBER)"
                          "VALUES (%s, %s, %s, %s, %s)")
add_sensor_data_biweekly = ("csUID, panID, DATA_TYPE, VALUE, WEEK_NUMBER)"
                            "VALUES (%s, %s, %s, %s, %s)")
add_sensor_data_monthly = ("csUID, panID, DATA_TYPE, VALUE, MONTH)"
                           "VALUES (%s, %s, %s, %s, %s)")
add_sensor_data_yearly = ("csUID, panID, DATA_TYPE, VALUE, YEAR)"
                          "VALUES (%s, %s, %s, %s, %s)")


# for testing
# def fake_data():
#     result = []
#     dataTypes = ["tmp", "hum", "co2", "voc", "bat"]
#     csUIDs = ["0xA000D", "0xB000D", "0xC000D", "0xD000D", "0xE000D"]
#     panIDs = ["0x12345AAA", "0x12345BBB", "0x12345CCC"]

#     for i in range(0, 5):
#         if i == 0:
#             value = random.randint(0, 100)
#         elif i == 1:
#             value = random.randint(0, 80)
#         elif i == 2:
#             value = random.randint(400, 1200)
#         elif i == 3:
#             value = random.randint(500, 2000)
#         elif i == 4:
#             value = random.randint(0, 100)

#         result.append(json.dumps(
#             {"csUID": "%s", "panID": "%s", "DATA_TYPE": "%s", "VALUE": "%s"})
#             % (csUIDs[i], panIDs[0], dataTypes[i], value))

#     time.sleep(10)
#     return json.dumps(result)


# while True and testing:
#     sensor_data = json.loads(fake_data())
#     # now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
#     print(sensor_data)
#     print(type(sensor_data))

#     for sensor in sensor_data:
#         data = json.loads(sensor)
#         print(type(sensor))
#         print(type(data))
#         print(data)
#         params = (data["csUID"], data["panID"],
#                   data["DATA_TYPE"], data["VALUE"], 1)
#         cursor.execute(add_sensor_data_week, params)
#         cnx.commit()


# daily - triggers at 00:00:01
# TODO fällt weg, wenn script eh um midnight gestartet wird
# today = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
midnight = datetime.now().strftime("%Y-%m-%d 00:00:01")
if today == midnight:
    yesterday = datetime.today() - timedelta(days=1)

    result = session.query(SensorData).filter(
        SensorData.timestamp.between(yesterday, today))

    tmps = []
    hums = []
    co2s = []
    vocs = []
    bats = []
    for sensor_data in result:
        if sensor_data.dataType == 'tmp':
            tmps.append(int(sensor_data.value))
        if sensor_data.dataType == 'hum':
            hums.append(int(sensor_data.value))
        if sensor_data.dataType == 'co2':
            co2s.append(int(sensor_data.value))
        if sensor_data.dataType == 'voc':
            vocs.append(int(sensor_data.value))
        if sensor_data.dataType == 'bat':
            bats.append(int(sensor_data.value))

    # calculate mean of every data_type
    mean_tmps = statistics.mean(tmps)
    mean_hums = statistics.mean(hums)
    mean_co2s = statistics.mean(co2s)
    mean_vocs = statistics.mean(vocs)
    mean_bats = statistics.mean(bats)

    # save to db


# weekly
if datetime.today().weekday() == 6:  # if(sunday)
    count_sunday += 1
    sunnday = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    last_monday = datetime.today() - timedelta(days=6)

    result = session.query(SensorData).filter(
        SensorData.timestamp.between(last_monday, sunnday))

    tmps = []
    hums = []
    co2s = []
    vocs = []
    bats = []
    for sensor_data in result:
        if sensor_data.dataType == 'tmp':
            tmps.append(int(sensor_data.value))
        if sensor_data.dataType == 'hum':
            hums.append(int(sensor_data.value))
        if sensor_data.dataType == 'co2':
            co2s.append(int(sensor_data.value))
        if sensor_data.dataType == 'voc':
            vocs.append(int(sensor_data.value))
        if sensor_data.dataType == 'bat':
            bats.append(int(sensor_data.value))

    # calculate mean of every data_type
    mean_tmps = statistics.mean(tmps)
    mean_hums = statistics.mean(hums)
    mean_co2s = statistics.mean(co2s)
    mean_vocs = statistics.mean(vocs)
    mean_bats = statistics.mean(bats)

    # save to db


# bi-weekly - triggered every second sunday
if count_sunday == 2:
    count_sunday = 0
    # today = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    monday_fortnight_ago = datetime.today() - timedelta(days=12)

    result = session.query(SensorData).filter(
        SensorData.timestamp.between(monday_fortnight_ago, today))

    tmps = []
    hums = []
    co2s = []
    vocs = []
    bats = []
    for sensor_data in result:
        if sensor_data.dataType == 'tmp':
            tmps.append(int(sensor_data.value))
        if sensor_data.dataType == 'hum':
            hums.append(int(sensor_data.value))
        if sensor_data.dataType == 'co2':
            co2s.append(int(sensor_data.value))
        if sensor_data.dataType == 'voc':
            vocs.append(int(sensor_data.value))
        if sensor_data.dataType == 'bat':
            bats.append(int(sensor_data.value))

    # calculate mean of every data_type
    mean_tmps = statistics.mean(tmps)
    mean_hums = statistics.mean(hums)
    mean_co2s = statistics.mean(co2s)
    mean_vocs = statistics.mean(vocs)
    mean_bats = statistics.mean(bats)

    # save to db


# monthly - every 1st
# today = datetime.today().date()
first_day_of_month = datetime.today().replace(day=1)

if today == first_day_of_month:
    # get first and last day of previous month
    last_day_of_prev_month = datetime.today().replace(day=1) - timedelta(days=1)
    start_day_of_prev_month = datetime.today().replace(
        day=1) - timedelta(days=last_day_of_prev_month.day)

    result = session.query(SensorData).filter(
        SensorData.timestamp.between(start_day_of_prev_month, last_day_of_prev_month))

    tmps = []
    hums = []
    co2s = []
    vocs = []
    bats = []
    for sensor_data in result:
        if sensor_data.dataType == 'tmp':
            tmps.append(int(sensor_data.value))
        if sensor_data.dataType == 'hum':
            hums.append(int(sensor_data.value))
        if sensor_data.dataType == 'co2':
            co2s.append(int(sensor_data.value))
        if sensor_data.dataType == 'voc':
            vocs.append(int(sensor_data.value))
        if sensor_data.dataType == 'bat':
            bats.append(int(sensor_data.value))

    # calculate mean of every data_type
    mean_tmps = statistics.mean(tmps)
    mean_hums = statistics.mean(hums)
    mean_co2s = statistics.mean(co2s)
    mean_vocs = statistics.mean(vocs)
    mean_bats = statistics.mean(bats)

    # save to db


# yearly - every 1st January
# today = datetime.today().date()
last_day_of_year = datetime.today().year, 12, 31
first_day_of_year = datetime.today().year, 1, 1

if today == last_day_of_year:
    result = session.query(SensorData).filter(
        SensorData.timestamp.between(first_day_of_year, last_day_of_year))

    tmps = []
    hums = []
    co2s = []
    vocs = []
    bats = []
    for sensor_data in result:
        if sensor_data.dataType == 'tmp':
            tmps.append(int(sensor_data.value))
        if sensor_data.dataType == 'hum':
            hums.append(int(sensor_data.value))
        if sensor_data.dataType == 'co2':
            co2s.append(int(sensor_data.value))
        if sensor_data.dataType == 'voc':
            vocs.append(int(sensor_data.value))
        if sensor_data.dataType == 'bat':
            bats.append(int(sensor_data.value))

    # calculate mean of every data_type
    mean_tmps = statistics.mean(tmps)
    mean_hums = statistics.mean(hums)
    mean_co2s = statistics.mean(co2s)
    mean_vocs = statistics.mean(vocs)
    mean_bats = statistics.mean(bats)

    # save to db


session.close()
