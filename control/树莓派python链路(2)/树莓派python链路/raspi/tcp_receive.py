# coding=UTF-8
import sys
import time
import socket
import RPi.GPIO

import serial_tx
tcp=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
data=b''
def tcp_open():
    global address
    address=('169.254.219.45',8888)
    #address=('192.168.55.159',8888)
    print("Connecting to server @ %s:%d..." %(address))
    tcp.connect(address)
    print("Connection Done")
    data=b''
    # while True:
	    # try:
	        # print("Connecting to server @ %s:%d..." %(address))
	        # tcp.connect(address)
	        # break
	    # except Exception:
	        # print("Can't connect to server,try it latter!")
	        # time.sleep(1)
	        # continue
def out():
    return data

def tcp_receive():
    data=tcp.recv(1000)
    if not data:
        tcp.close()
        tcp_open()
    print(data)
    serial_tx.ser.write(data)
    if not data:
        tcp.close()
        tcp_open()
    #time.sleep(0.1)
    #tcp.send(b'12')
    #print(type(data))
if __name__ == '__main__':
    tcp_open()
    tcp_receive()
