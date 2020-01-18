'''

pyScoreboard


'''
# !/usr/bin/python3


import serial
import time

import serial_list  # wcp
#import header  # wcp


#PORTS
ports = serial_list.serial_ports()
ports_num = len(ports)
print('Found ', end=' ')
print(ports_num, end=' ports.')
if (ports_num) == 0:
	print('No USB serial ports found.')
if ports_num >= 1:
	# port0 = '/dev/ttyACM0'
	port0 = ports[0]
	print('Found USB serial port: ' + ports[0])
	#SOUP = True

print("Connecting to USB serial port: ", end='')
print(port0)
scoreboard = serial.Serial(port0, 115200, timeout=.1)
#print(scoreboard.name)


#setup
#scoreboard.write(b'$RASPI>?msg=hello_from_raspi\n')


def getTime():
    scoreboard.write(b'$RASPI>?gettime=20191201T220400Z\n')
    #time.sleep(.3)


def getTemp():
    scoreboard.write(b'$RASPI>?gettemp=75F\n')


#loop
while(1):

    #READ
    dataScoreboard = scoreboard.readline()[:-2] #.decode('ascii')  # the last bit gets rid of the new-line chars
    #dataScoreboard = scoreboard.read()
    if dataScoreboard:
        print(dataScoreboard)
        #WRITE
        #getTime()
        #time.sleep(.5)

