class AlertHandler():


    def __init__(self,basis=None,DB=None,emailHandler=None):
        self.basis = basis
        self.DB = DB
        self.emailHandler = emailHandler
        self.rate_alert_limit = 3600


    def email_alert (self, errormsg, errortype='unknown', to_emails=None, testmodus=None):
        """ 
            Send an alert emeil to inform somethg is wrong 
            Remember that email has been sent. Send only once an hour
            params:
                errormsg = errortext
                errortype = type of errer remembered in Tabel EMAIL_ALERTS
                to_emails = comma (,) deperated list of receipients
        """
        self.basis.err (errortype+': '+errormsg)
        nowTime = datetime.fromtimestamp(time.time())
        # Already in DB?
        oldError = self.DB.query('select * from EMAIL_ALERTS where ERROR_TYPE="%s" order by TIMESTRING DESC LIMIT 1;' % errortype )
        oldEnough = True
        if oldError:
            oldTime = datetime.strptime(oldError[0]['TIMESTRING'], '%Y-%m-%d %H:%M:%S')
            if (nowTime-oldTime).total_seconds() <= self.rate_alert_limit: oldEnough = False
                
        if oldEnough==True:
            # todo: send email....
            errormessage = errormsg.replace('"','')
            qs = 'insert into EMAIL_ALERTS (ERROR_TYPE, ERROR_MESSAGE, TIMESTRING) values (\'%s\',\'%s\',\'%s\')' % (errortype, errormsg, nowTime.strftime('%Y-%m-%d %H:%M:%S') )
            resInsert = self.DB.execute(qs)
            if to_emails is not None:
                self.basis.vomit (errortype+': Send emails to: '+to_emails)
                list_emails = to_emails.split(',')
                for em in list_emails:
                    param = {'ERRORMESSAGE':errormsg, 'ERRORTYPE':errortype} 
                    self.email_handler('ALERT_EMAIL_TEMPLATE', em, 'en', param, testmodus=testmodus)