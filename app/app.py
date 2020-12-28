import tornado.web
import tornado.ioloop
import tornado.websocket
import tornado
import re
import json
import logging
from sqlalchemy import create_engine,select,insert
from libs.helper import helper
from models import Rooms,session,Settings

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
            log( fh, 1,  'Out: '+'malformed JSON' )
            return
        data = replace_dict(data)  # replace ' and "
        obj = msg_type = sid = None
        if "MSG_TYPE" not in data:
            answer = helper.build_json_answer(data["ID"],"INVALID",json.dumps("PLEASE ENTER MESSAGE TYPE"))
            self.write_message(answer)
            return
        MSG_TYPE = data["MSG_TYPE"]

        if MSG_TYPE == "SAVE_ROOM_TO_DATABASE":
            room = Rooms()
            room.name = data["ROOM_NAME"]
            room.data = data["ROOM_DATA"]
            try:
                session.add(room)
                session.commit()
            except:
                session.rollback()
            answer = helper.build_json_answer(data["ID"],"SUCCESS",json.dumps(room.name +" erfolgreich hinzugefügt"))
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
        else:
            answer = helper.build_json_answer(data["ID"],"INVALID",json.dumps("INVALID MESSAGE TYPE"))

        if answer:
            logger.warning(answer)
            self.write_message(answer)

    def on_close(self):
        print("WebSocket closed")




if __name__ == "__main__":
    application = tornado.web.Application([
        (r"/ws", MainHandler),
    ],debug=True)
    application.listen(8888)
    tornado.ioloop.IOLoop.current().start()