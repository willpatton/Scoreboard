'''

pyScoreboard


'''
# !/usr/bin/python3

import threading
import serial
import time
import serial_list  # wcp
#import header  # wcp

#GLOBALS
cmd = ""        #cmd
msg = ""        #message
ver = ""        #version
temp = 0.0      #temperature
loop_counter = 0




#DATE TIME
from datetime import datetime
now = datetime.now()
current_time = now.strftime("%H:%M:%S")
print("Current Time =", current_time)
#ZULU- 20191201T220400Z 
current_zulu = now.strftime("%Y%m%dT%H%M%SZ")
print("Current Zulu =", current_zulu)


#SERIAL OVER USB PORTS
ports = serial_list.serial_ports()
ports_num = len(ports)
print('Found ', end=' ')
print(ports_num, end=' ports.\n')
if (ports_num) == 0:
    print('No USB serial ports found.\n')
if ports_num >= 1:
    # port0 = '/dev/ttyACM0'
    port0 = ports[0]
    print('Found USB serial port: ' + ports[0])
if ports_num >= 2:
    # port1 = '/dev/ttyACM0'
    port1 = ports[1]
    print('Found USB serial port: ' + ports[1])  
print("Connecting to USB serial port: ", end='')
print(port0)
scoreboard = serial.Serial(port0, 19200, timeout=.1)  #39.4K and up NOT RELIABLE!!
#print(scoreboard.name)


# WAIT FOR Arduino RESET here
#time.sleep(2)
#while(scoreboard.readline()):    
#    print("Waiting\n")



def sendCmd(myStr): 
    print("RASPI>: " + myStr)
    myStr = myStr + "\n"
    scoreboard.write(myStr.encode())  #string to bytes
    #time.sleep(.3)

def setZulu():
    myStr = "$RASPI>?&cmd=setzulu&data=" + current_zulu + "\n"
    scoreboard.write(myStr.encode())  #string to bytes
#    time.sleep(.3)


def setTemp():
    scoreboard.write(b'$RASPI>?&cmd=settemp&data=75&units=F\n')
#    time.sleep(.3)


'''
def myTimer():
    print("MyTimer!!!")

timer = threading.Timer(7.0,myTimer)
timer.start()
'''
loop_timer = time.perf_counter()

#LOOP
while(1):

    #READ SERIAL AVAILABLE
    dataScoreboard = scoreboard.readline()[:-2] #.decode('ascii')  # the last bit gets rid of the new-line chars
    #dataScoreboard = scoreboard.read()
    if dataScoreboard:
        
        #strip bad chars
        #if(\xf0\xf0 in incoming):
        #    print(dataScoreboard)
        
        #ECHO incoming bytes
        incoming = dataScoreboard.decode()
        print(incoming)
        
        #PARSE SETUP
        if("Begin setup()" in incoming):
            print("FOUND: Begin setup")
            #SEND INIT COMMANDs
            setZulu()
            time.sleep(.3)
            setTemp()
            time.sleep(.3)
      
        if("$SB>" in incoming):
            print("FOUND: $SB>")
            #print([params[i:i+3] for i in range (0,len(params), 3)])
            params = incoming.split('&')
            #print(len(params))
            #print(params)
            for i in range (0,len(params)):
                if("=" in params[i]):
                    print(params[i])
                    param = params[i].split('=')
                    print(param[0])
                    print(param[1])
                    
                    #COMMANDS INCOMING
                    if("cmd" in param[0]):
                        cmd = param[1]
                        
                    if("msg" in param[0]):
                        msg = param[1]

                    if("ver" in param[0]):
                        ver = param[1]

                    print("cmd:" + cmd + ", msg: " + msg + ", ver: " + ver)            
          

    #TIMER
    if(time.perf_counter() - loop_timer > 3.0):
        loop_timer = time.perf_counter()
        #print("RASPI LOOP TIMER")
        
        #MODE CAROUSEL
        #S=scoreboard
        #s=clock
        #d=date
        #t=test
        remainder = loop_counter % 2  #2 for time and date, 4 for all
        if(remainder == 0):
            sendCmd("$RASPI>?&cmd=s")  #clock  
        if(remainder == 1):
            sendCmd("$RASPI>?&cmd=d")  #d date
        if(remainder == 2):
            sendCmd("$RASPI>?&cmd=S")
            time.sleep(.3)
            for i in range (1,7):
                sendCmd("$RASPI>?&cmd=v")  #visitors
                time.sleep(.3)
            for i in range (1,5):
                sendCmd("$RASPI>?&cmd=h")  #home
                time.sleep(.3)
        if(remainder == 3):
            sendCmd("$RASPI>?&cmd=t")
        loop_counter += 1
        
        
                    
