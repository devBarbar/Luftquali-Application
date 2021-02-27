import time
import logging
import datetime
import sys
import sys

class Base():
    """
        Basis-Klasse zur Vererbung von ->ENVIRONMENT<- und ->LOGGING<-
    """

    def __init__(self, className=None HOST=None):
        """ 
            set logging and environment
            Param className: Skipt auto-detection of (inherited?) Class Name
            Param DEV: Skipt auto-detection of env DEV ("testing", "development", "feprd01"...)
        """
        self.initialize_root_handler()
        if className is None:
            className = self.__class__.__name__
        self.className = className 

        # logger:
        self.infoLogger = logging.getLogger(className)
        if self.infoLogger.hasHandlers():
            self.infoLogger.handlers = []
        self.logFormat = logging.Formatter(
            "----------------\n%(asctime)s %(name)s %(levelname)-8s  %(message)s\n"
        )
        self.logFileHandler = logging.FileHandler(f"/nginx/logs/{className}.log")
        self.infoLogger.setLevel(logging.DEBUG)
        self.logFileHandler.setFormatter(self.logFormat)
        self.infoLogger.addHandler(self.logFileHandler)
        self.ExceptionFileHandler = logging.StreamHandler(sys.stderr)
        self.ExceptionFileHandler.setFormatter(self.logFormat)
        self.ExceptionFileHandler.setLevel(logging.ERROR)
        self.infoLogger.addHandler(self.ExceptionFileHandler)
        self.infoLogger.propagate = False
        sys.excepthook = self.handle_exception

    def throwException(self, getaTraceback= True, customText=''):
        self.infoLogger.exception(f"{self.className}, {customText}", exc_info=getaTraceback)

    def handle_exception(self, exc_type, exc_value, exc_traceback):
        self.infoLogger.error("CUSTOM Uncaught exception", exc_info=(exc_type, exc_value, exc_traceback))
        sys.__excepthook__(exc_type, exc_value, exc_traceback)
        return
        
    def log(self, l, s, e=False):
        logstr = str(s)
        #print(logstr)
        if e==False:
            self.infoLogger.info(logstr)
        else:
            self.infoLogger.error(logstr)

    # give anything (list, tuple, string, int, object) to write to log file
    def vomit(self, what):
        self.log(1, what)

    # give anything (list, tuple, string, int, object) to write to error-log file
    def err(self, what):
        self.log(1, what, True)


    def initialize_root_handler(self):
        root = logging.getLogger()
        root.setLevel(logging.ERROR)