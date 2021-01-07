import socket
import time
import smtplib, ssl, getpass
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

MAX = 65535
ARDUINO = ('192.168.178.29', 7000)
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.settimeout(30) #Чекај 30 секунди за одговор

#Клиентот бара email извештај, серверот му ги праќа податоците. Клиентот ја креира пораката и ја праќа. 
port = 465 #За SSL
smtp_server = "smtp.gmail.com"

while True:
    command = input("Please enter your command: ")
    client.sendto(command.encode(), ARDUINO)
    try:
        if command == "EMAIL REPORT":
            data, addr = client.recvfrom(MAX)
            arduino_message = data.decode().split("|")

            sender_email = "nadezhdastestemail@gmail.com"
            password = "Testiranje12"
            context = ssl.create_default_context()
            with smtplib.SMTP_SSL(smtp_server, port, context=context) as session:
                    session.login(sender_email, password)
                    msg = MIMEMultipart()
                    recepient_email = input("Where would you like to receive the report? Enter a valid email adress: ")
                    msg['To'] = recepient_email
                    msg['From'] = sender_email
                    msg['Subject'] = "Email Report from your SmartHouse"
                    part = MIMEText('text', 'plain')
                    message = "Hello.\nThe temperature in your house is " + arduino_message[0] + " Celsius degrees.\nThe humidity in your house is " + arduino_message[1] + "%."
                    part.set_payload(message)
                    msg.attach(part)
                    session.sendmail(sender_email, recepient_email, msg.as_string())
                    session.quit()
            print("EMAIL SENT!")
        else:
            data, addr = client.recvfrom(MAX)
            print(data.decode())
    except:
        pass
    time.sleep(2) #Delay пред праќање на друга команда
