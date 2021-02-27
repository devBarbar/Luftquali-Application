import mimetypes
import smtplib
import email.utils
from email import encoders
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email.mime.image import MIMEImage
from email.utils import make_msgid
from email.mime.multipart import MIMEMultipart

class Mailer():

    def __init__(self,base=None,server= "localhost"):
        self.base = base
        self.smtp_server = server

    def mailer(self,sender_email_addr:str,subject:str,to_email_addr:str,msgbody):
        msg = MIMEMultipart('mixed')
        msg.attach(MIMEText(msgbody.encode('utf-8'), 'plain', 'utf-8'))
        msg.set_unixfrom('AirQuality')
        msg['Content-Type'] = "text/html; charset=utf8"
        msg['Subject'] = subject
        msg['From'] = email.utils.formataddr(('AirQuality: ', sender_email_addr))
        msg['To'] = to_email_addr
        msg['Date'] = email.utils.formatdate(localtime=True)
        msg['Message-ID'] = make_msgid()

        bcc = []
        bcc = [sender_email_addr]
        server = smtplib.SMTP(self.smtp_server)
        server.ehlo()
        try:
            # identify ourselves, prompting server for supported features
            server.ehlo()

            # If we can encrypt this session, do it
            if server.has_extn('STARTTLS'):
                server.starttls()
                server.ehlo() # re-identify ourselves over TLS connection
            server.sendmail(sender_email_addr, [to_email_addr]+bcc, msg.as_string())
        except:
            pass
        finally:
            server.quit()
    
        return