from sqlalchemy import create_engine, Column, Integer, String, ForeignKey, JSON, Time, TIMESTAMP
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
from mysql.connector import (connection)
import time
Base = declarative_base()


class User(Base):
    __tablename__ = "PERSON"

    id = Column("id", Integer, primary_key=True)
    username = Column("USERNAME", String(90), unique=True)
    password = Column("PASSWORD", String(90))


class Rooms(Base):
    __tablename__ = "ROOMS"

    name = Column("NAME", String(90), unique=True, primary_key=True)
    data = Column("DATA", JSON)


class Settings(Base):
    __tablename__ = "SETTINGS"

    name = Column("ID", Integer, primary_key=True)
    rule = Column("RULE", String(90), unique=True)
    type = Column("TYPE", String(90))
    value = Column("VALUE", String(90))


class User(Base):
    __tablename__ = "USER"

    email = Column("EMAIL", String(128), primary_key=True)
    password = Column("PASSWORD", String(128))
    username = Column("USERNAME", String(90))
    last_login = Column("LAST_LOGIN", Time)


class SensorData(Base):
    __tablename__ = "SENSOR_DATA"

    id = Column("ID", Integer, primary_key=True, autoincrement=True)
    csUID = Column("csUID", String(45), primary_key=True)
    panID = Column("panID", String(45), primary_key=True)
    dataType = Column("DATA_TYPE", String(45))
    value = Column("VALUE", String(45))
    timestamp = Column("TIMESTAMP", TIMESTAMP)


class SensorDataWeek(Base):
    __tablename__ = "SENSOR_DATA_WEEK"

    id = Column("ID", Integer, primary_key=True, autoincrement=True)
    csUID = Column("csUID", String(45), primary_key=True)
    panID = Column("panID", String(45), primary_key=True)
    dataType = Column("DATA_TYPE", String(45))
    value = Column("VALUE", String(45))
    weekNumber = Column("WEEK_NUMBER", Integer)


class SensorDataBiWeek(Base):
    __tablename__ = "SENSOR_DATA_BIWEEK"

    id = Column("ID", Integer, primary_key=True, autoincrement=True)
    csUID = Column("csUID", String(45), primary_key=True)
    panID = Column("panID", String(45), primary_key=True)
    dataType = Column("DATA_TYPE", String(45))
    value = Column("VALUE", String(45))
    weekNumber = Column("WEEK_NUMBER", Integer)


class SensorDataMonth(Base):
    __tablename__ = "SENSOR_DATA_MONTH"

    id = Column("ID", Integer, primary_key=True, autoincrement=True)
    csUID = Column("csUID", String(45), primary_key=True)
    panID = Column("panID", String(45), primary_key=True)
    dataType = Column("DATA_TYPE", String(45))
    value = Column("VALUE", String(45))
    month = Column("MONTH", Integer)


time.sleep(10)

engine = create_engine(
    "mysql+mysqlconnector://user:password@mysql_database:3306/mysql_database", echo=True)

Base.metadata.create_all(bind=engine)
Session = sessionmaker(bind=engine)

session = Session()

user = User()

session.close()
