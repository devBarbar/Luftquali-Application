import dkim
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

    def __init__(self,base=None,DB=None,server= "127.0.0.1"):
        self.DB = DB
        self.base = base
        self.smtp_server = server

    
    def email_handler(
            self, email_template, to_email_addr, lang=None, params=None
        ):
            """ Email Handler nimmt %s Parameter in einem Array entgegen und kümmert sich um den Bau der Email in der gesetzten Sprache.
            Macht Email in HTML und Text.
            
            params: Str email_template: Identifiziert in Tabelle EMAILS die Email Template 
                    Str to_email_addr: Empfänger Email
                    Str lang: Sprache "de" oder "en" (default "de")
                    Dict params: key=>value zum Einsetzen in den Subject und Body bzw BODY_HTML (ALLES STRINGS!)
                    Str file, file2: optionaler Pfad zu einem Bild falls benötigt in HTML Version
                    testmodus != None: Schreibe file anstatt zu senden
        """
            if lang is None: lang = "de"
            elif len(lang) != 2: lang = "de"
            qs = 'SELECT * FROM EMAIL_TEMPLATES where TEMPLATE = "%s" and SPRACHE = "%s"' % (email_template, lang)
            tmpl = self.DB.query( qs )
            qs = 'SELECT * FROM EMAIL_TEMPLATES where TEMPLATE = "FOOTER" and SPRACHE = "%s"' % (lang)
            tmpFooter = self.DB.query( qs )

            subject = tmpl[0]["SUBJECT"]
            body_txt = tmpl[0]["BODY"]
            body_htm = tmpl[0]["BODY_HTML"]  
            from_email = tmpl[0]["ABSENDER_EMAIL"]
            footer = tmpFooter[0]["BODY"]
            footer_html = tmpFooter[0]["BODY_HTML"]

            if from_email is None: from_email = 'bahmad@stud.fra-uas.de'
            
            for key in list(params.keys()):
                ident = "{{" + key + "}}"

                if body_txt:
                    try:
                        body_txt = body_txt.replace(ident, params[key])
                    except:
                        body_txt = body_txt.replace(ident, params[key].decode("utf-8"))
                    body_txt = body_txt.replace("\\n", "\n")

                try:
                    subject = subject.replace(ident, params[key])
                except:
                    subject = subject.replace(ident, params[key].decode("utf-8"))

                if body_htm:
                    try:
                        body_htm = body_htm.replace(ident, params[key])
                    except:
                        body_htm = body_htm.replace(ident, params[key].decode("utf-8"))
                    
            if footer:
                footer = footer.replace("{{ABSENDER_EMAIL}}", from_email)
                footer = footer.replace("\\n", "\n")
                body_txt = body_txt + footer
        
              
                    
            if body_htm and footer_html:
                footer_html = footer_html.replace("{{ABSENDER_EMAIL}}",from_email)
                body_htm = body_htm + footer_html
              
                    
            self.mailer(from_email, subject, to_email_addr, body_txt, body_htm, file, file2, testmodus=testmodus)





    def mailer(self,sender_email_addr:str,subject:str,to_email_addr:str,msgbody):
        msg = MIMEMultipart('mixed')

        msg.attach(MIMEText(msgbody.encode('utf-8'), 'plain', 'utf-8'))

        msg.set_unixfrom('AirQuality')
        msg['Content-Type'] = "text/html; charset=utf8"
        msg['Subject'] = sender_subject
        msg['From'] = email.utils.formataddr(('AirQuality: ', sender_email_addr))
        msg['To'] = to_email_addr
        msg['Date'] = email.utils.formatdate(localtime=True)
        msg['Message-ID'] = make_msgid()

        bcc = []
        bcc = [sender_email_addr]
        

        signature = dkim.sign( msg.as_bytes(), b'AirQuality',
                        b'Dashboard', dkim_private.encode(), 
                        include_headers=[b'from', b'to', b'subject', b'message-id'])
        msg['DKIM-Signature'] = str(signature[len("DKIM-Signature: "):])

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
            self.basis.throwException(True,"ERROR GLOBAL MAIL HANDLER")
        finally:
            server.quit()
    
        return