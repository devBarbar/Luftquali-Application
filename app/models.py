from sqlalchemy import create_engine, Column,Integer,String,ForeignKey,JSON
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
from mysql.connector import (connection)
import time
Base = declarative_base()

class User(Base):
    __tablename__ = "PERSON"

    id = Column("id",Integer, primary_key=True)
    username = Column("USERNAME",String(90),unique=True)
    password = Column("PASSWORD",String(90))

class Rooms(Base):
    __tablename__ = "ROOMS"

    name = Column("NAME",String(90),unique=True,primary_key=True)
    data = Column("DATA",JSON)

class Settings(Base):
    __tablename__ = "SETTINGS"

    name = Column("ID",Integer,primary_key=True)
    rule = Column("RULE",String(90),unique=True)
    type = Column("TYPE",String(90))
    value = Column("VALUE",String(90))

engine = create_engine("mysql+mysqlconnector://user:password@mysql_database:3306/mysql_database", echo=True)

Base.metadata.create_all(bind=engine)
Session = sessionmaker(bind=engine)

session = Session()

user = User()


session.close()
