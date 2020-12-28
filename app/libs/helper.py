
#!/usr/bin/python3
# -*- coding: utf-8 -*-
###########################################
# IMPORT SECTION
###########################################
import os
import sys
import traceback
import json
import uuid
import base64
import time
from datetime import datetime, timedelta
from decimal import Decimal
import string
import re
import codecs


"""
Python Helperklasse mit häufiggenutzen (einfachen) Funktionen,

"""
"""
    from class_helper import helper
    Stellt Helper Funktionen zur Verfügung (ohne self)
    z.B.
    dummy = helper.is_json('bla')

"""


class helper:
    
    class DecimalEncoder(json.JSONEncoder):
        def default(self, obj):
            if isinstance(obj, Decimal):
                return str(obj)
            # Let the base class default method raise the TypeError
            return json.JSONEncoder.default(self, obj)
            
    @staticmethod
    def rechte():
        return {
            'RECHTE_ADMIN':1,
            'RECHTE_ADMIN_BEREICH':2,
            'RECHTE_EURO':4,
            'RECHTE_TERMIN':8,
            'RECHTE_ZAHLUNGEN':16,
            'RECHTE_ZAHLUNGEN_WRITE':32,
            'RECHTE_ADRESSEN':64,
            'RECHTE_ADRESSEN_WRITE':128,
        }

    @staticmethod
    def hatRecht(r, recht):
        checkrechte=helper.rechte()
        return checkrechte[r] & recht

    @staticmethod
    def is_json(the_message):
        try:
            json_object = json.loads(the_message)
        except ValueError as e:
            return False
        return json_object

    @staticmethod
    def get_data(data, param):
        if param in data:
            return data[param]
        else:
            return ""

    @staticmethod
    def format_german(number):
        str = "{:,.2f}".format(number)
        a = str.replace(",", ";")
        b = a.replace(".", ",")
        c = b.replace(";", ".")
        return c

    @staticmethod
    def convert_german(value):
        result = value
        if type(value).__name__ == "bytes" or type(value).__name__ == "str":
            if type(value).__name__ == "bytes":
                result = str(value)
            else:
                result = value
            if result.find(".") != -1 or result.find(",") != -1:
                result = result.replace(".", "")
            result = result.replace(",", ".")
            if result == "":
                result = 0
            else:
                result = float(result)
        return result

    @staticmethod
    def build_json_answer(sid, msg_type, json_data):
        return '{ "MSG_TYPE": "%s", "ID": "%s", "DATA": %s }' % (
            msg_type,
            sid,
            json_data,
        )

    @staticmethod
    def anrede_helper(language, herrfrau, nachname, vorname = None):
        """ Build a salutation using the information provided in language 
        params: 
                Str language: "de" or "en" (default "de")
                Str herrfrau: "Herr" or "Frau" (Mr. or Mrs.)
                Str nachname (Lastname)
    """
        language = language.lower()
        if language is None:
            language = "de"
        elif len(language) != 2:
            language = "de"

        if language == "de":
            if herrfrau == "Herr":
                anrede = "Sehr geehrter Herr %s" % nachname
            else:
                anrede = "Sehr geehrte Frau %s" % nachname
        elif language == "en":
            if herrfrau == "Herr":
                anrede = "Dear Mr. %s" % nachname
            else:
                anrede = "Dear Mrs. %s" % nachname
        elif language == "br":
            if herrfrau == "Herr":
                anrede = "Prezado Sr. %s" % nachname
            else:
                anrede = "Prezada Sra. %s" % nachname
        elif language == "nl":
            if herrfrau == "Herr":
                anrede = "Geachte heer %s" % nachname
            else:
                anrede = "Geachte mevrouw %s" % nachname

        return anrede

    @staticmethod
    def createToken():
        token = uuid.uuid4().hex
        token = base64.b64encode(bytes(token, 'utf8'))
        token = token.replace( b'=', b'').decode()
        return token

    @staticmethod
    def build_json_answer(sid, msg_type, json_data, action: string = ''):
        return f'{{ "MSG_TYPE": "{msg_type}", "ID": "{sid}", "DATA": {json_data}, "ACTION": "{action}" }}'

    @staticmethod
    def build_response(sid, msg_type, json_data, action=""):
        return '{"MSG_TYPE": "%s", "ID": "%s", "DATA": %s, "ACTION":"%s"}' % (msg_type, sid, json.dumps(json_data), action)

        
    @staticmethod
    def stringIsNullOrWhitespace(toCheck):
        return toCheck != None and not toCheck.strip()

    @staticmethod
    def get_current_timestamp():
        timestamp = str(time.mktime(datetime.now().timetuple()))
        ts = timestamp.split(".")
        timestamp = int(ts[0])
        return timestamp
    @staticmethod
    def build_json_error(sid, msg_type, json_data):
        return f'{{ "MSG_TYPE": "{msg_type}", "ID": "{sid}", "DATA": {json_data}, "STATUS": "2" }}' 