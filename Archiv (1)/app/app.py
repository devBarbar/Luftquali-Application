import tornado.web
import tornado.ioloop
import tornado.websocket
import tornado
import re
import subprocess
import sys
import json
import logging
from sqlalchemy import create_engine,select,insert
from datetime import time,datetime
from libs.helper import helper
from models import Rooms,session,Settings,User,SensorData,engine,SensorToRoom,Alerts
from passlib.hash import pbkdf2_sha256
sql_inject_re_byt = re.compile(b'\'|"')
sql_inject_re_str = re.compile('\'|"')

def build_json_msg( sid, msg_type, status, text ):
    """
        Baue Antwort aus sid, msg_type (s), status (i), text (s)
    """

    return '{ "MSG_TYPE": "%s", "ID": "%s", "STATUS": %i, "TEXT": "%s" }' % ( msg_type, sid, status, text )

def replace_dict(the_dict):
    for k, v in the_dict.items():
        the_dict[k] = do_replace(the_dict, k)
    return the_dict

def replace_list(the_list):
    nr = 0
    for v in the_list:
        the_list[nr] = do_replace(the_list, nr)
        nr += 1
    return the_list
    
def do_replace(the_thing, k):
    v = the_thing[k]
    if (type(v).__name__=='str'):
        return sql_inject_re_str.sub('',v)
    elif (type(the_thing[k]).__name__=='bytes'):
        return sql_inject_re_byt.sub(b'',v)
    elif (type(the_thing[k]).__name__=='list'):
        return replace_list(v)
    elif (type(the_thing[k]).__name__=='dict'):
        return replace_dict(v)
    else:
        return v # leave unchanged int, float, ...
class MainHandler(tornado.websocket.WebSocketHandler):


    def check_origin(self, origin):
        print(origin)
        return True

    def open(self):
        print("WebSocket opened")

    def on_message(self, message):
        logger = logging.getLogger()
        answer = None
        data = helper.is_json( message )
        logger.warning(data)
        if data == False:
            self.write_message( 'malformed JSON' )
            logger.log('Out: '+'malformed JSON' )
            return
  
        if "MSG_TYPE" not in data:
            answer = helper.build_json_answer(data["ID"],"INVALID",json.dumps("PLEASE ENTER MESSAGE TYPE"))
            self.write_message(answer)
            return

        MSG_TYPE = data["MSG_TYPE"]

        if MSG_TYPE == "GET_AVAILABLE_SENSORS":
            result = session.query(SensorData.csUID).distinct()
            sensors = []
            for sensor in result:
                sensors.append(sensor)
            
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(sensors))
            logger.warning(result)
        elif MSG_TYPE == "GET_ALERTS":
            result = []
            with engine.connect() as conn:
                rs = conn.execute('SELECT MESSAGE FROM ALERTS')
                for res in rs:
                    result.append(res[0])
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(result))
        elif MSG_TYPE == "CREATE_ROOM_TO_SENSOR_MAPPING":
            logger.error(data)
            with engine.connect() as conn:
                rs = conn.execute('SELECT * FROM SENSOR_TO_ROOM WHERE CS_UID = "%s" AND ROOM_ID = "%s"' % (data["SENSOR"],data["ROOM"]))
                for res in rs:
                    rs = conn.execute('DELETE FROM SENSOR_TO_ROOM WHERE CS_UID = "%s" AND ROOM_ID = "%s"' % (data["SENSOR"],data["ROOM"]))
           
            sToR = SensorToRoom()
            sToR.room_id = data["ROOM"]
            sToR.sensor_id = data["SENSOR"]
            logger.warning(data["SENSOR"])
            try:
                session.add(sToR)
                session.commit()
            except:
                session.rollback()
                raise

            answer = helper.build_json_answer(data["ID"],"SUCCESS",json.dumps("erfolgreich hinzugefügt"))
        elif MSG_TYPE == "GET_STATISTICS_FOR_SPECIFIC_SENSOR":
            values = []
            stats = []
            from_date = datetime.strptime(data["FROM"], '%Y-%m-%dT%H:%M:%S.%fZ')
            till_date = ''
            if "TILL" in data:
                till_date = "AND TIMESTAMP < '%s'" %time.strptime(data["TILL"], '%Y-%m-%dT%H:%M:%S.%fZ') 
            logger.warning("_--------------------")
            logger.warning(from_date)
            logger.warning("_--------------------")
            with engine.connect() as conn:
                data_types = conn.execute('SELECT distinct(DATA_TYPE) FROM mysql_database.SENSOR_DATA WHERE csUID = "%s" AND TIMESTAMP >= "%s" %s' % (data["SENSOR"],from_date,till_date))
                for datatype in data_types:
                    formatted_stats = {"id":datatype[0],"DATA":{"MIN":0,"MAX":0,"AVG":0}}
                    rs = conn.execute('SELECT * FROM SENSOR_DATA WHERE csUID = "%s" AND DATA_TYPE = "%s" AND TIMESTAMP >= "%s" %s' % (data["SENSOR"],datatype[0],from_date,till_date))
                    formatted_result = {
                            "id":datatype[0],
                            "color":"red",
                            "data": [] 
                        }
                    for row in rs:
                        formatted_result["data"].append({"x":row[5].strftime("%Y-%m-%d, %H:%M:%S"),"y":row[4]})
                    min_max_avg = conn.execute('SELECT MIN(CAST(VALUE AS DOUBLE)),MAX(CAST(VALUE AS DOUBLE)),AVG(CAST(VALUE AS DOUBLE)) FROM SENSOR_DATA WHERE csUID = "%s" AND DATA_TYPE = "%s"' % (data["SENSOR"],datatype[0]))
                    for row in min_max_avg:
                        formatted_stats["DATA"]["MIN"] = row[0]
                        formatted_stats["DATA"]["MAX"] = row[1]
                        formatted_stats["DATA"]["AVG"] = row[2]
                    values.append(formatted_result)
                    stats.append(formatted_stats)

            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps([values,stats]))

        elif MSG_TYPE == "SAVE_ROOM_TO_DATABASE":
            room = Rooms()
            room.name = data["ROOM_NAME"]
            room.data = data["ROOM_DATA"]
            logger.warning(data["ROOM_DATA"])
            try:
                session.add(room)
                session.commit()
            except:
                session.rollback()
            answer = helper.build_json_answer(data["ID"],"SUCCESS",json.dumps(room.name +" erfolgreich hinzugefügt"))

        elif MSG_TYPE == "GET_ROOM_FROM_DATABASE":
            room = session.query(Rooms).get(data["ROOM_NAME"])
            logger.warning(room)
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(room.data))
        
        elif MSG_TYPE == "SHOW_ALL_ROOMS":
            result = session.query(Rooms).all()
            room_names = []
            for room in result:
                logger.warning(room)
                room_names.append(room.name)
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(room_names))


        elif MSG_TYPE == "GET_ALL_SETTINGS":
            result = session.query(Settings).all()
            settings = []
            for setting in result:
                settings.append({RULE:setting.rule,VALUE:setting.value})
            
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(settings))
            logger.warning(result)

        elif MSG_TYPE == "LOGIN":
            result = {"STATUS":"FAIL"}
            user = session.query(User).get(data["EMAIL"])
            if user:
                if (pbkdf2_sha256.verify(data["PASSWORD"], user.password) == True):
                    result = {
                        "STATUS":"SUCCESS",
                        "EMAIL": user.email,
                        "USERNAME": user.username,
                        "LAST_LOGIN":user.last_login 
                    }
                    
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(result,default=str))
        elif MSG_TYPE == "REGISTER":
            hashed_password = pbkdf2_sha256.encrypt(data["PASSWORD"], rounds=12000, salt_size=16)
            user = User()
            user.email = data["EMAIL"]
            user.password = hashed_password
            user.username = data["USERNAME"]
            user.last_login = time()
            try:
                session.add(user)
                session.commit()
            except Exception as e:
                logger.error('Error at %s', 'division', exc_info=e)
                session.rollback()
                self.write_message(helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps({"STATUS":"DUPLICATE"})))
                return 

            result = session.query(User).get(data["EMAIL"])
            result = {
                "EMAIL": result.email,
                "PASSWORD": result.password
            }
            answer = helper.build_json_answer(data["ID"],MSG_TYPE,json.dumps(result))
        else:
            answer = helper.build_json_answer(data["ID"],"INVALID",json.dumps("INVALID MESSAGE TYPE"))

        if answer:
            #logger.warning(answer)
            self.write_message(answer)

    def on_close(self):
        print("WebSocket closed")




if __name__ == "__main__":
    application = tornado.web.Application([
        (r"/ws", MainHandler),
    ],debug=True)
    application.listen(8888)
    tornado.ioloop.IOLoop.current().start()

    