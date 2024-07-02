import socket
import sys
import time
# coding=UTF-8
import os
import joystick
import numpy
def TCP_init():
    global tcp, raspi_client
    address = ('192.168.137.1',8888)
    tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp.bind(address)
    print('Server @ %s:%d Done'%(address))
    print('Waiting for Client')
    tcp.listen(2)
    data=b''
    raspi_client,(client_ip, client_port) =tcp.accept()
    print("Connection Accepted from Client %s:%s." %(client_ip,client_port))
    time.sleep(1)
    tcp.settimeout(0.1)


def init():
    joystick.joystick_init()
    TCP_init()

def trans_hex(input):
    output=0
    input=0

def TCPserver():
    TX=[0]*12
    while True:
        try:
            joystick.TX_refresh(TX)
            TX_str = hex(TX[0])[2:].rjust(2, '0')
            for i in range(1, 12):
                TX_str += hex(TX[i])[2:].rjust(2, '0')
            print(TX_str)
            joystick.joystick_input()
            raspi_client.send(str.encode(TX_str))#str.encode(TX_str))#TX_str))#,joystick.rc_command.RUD,joystick.rc_command.ELE,joystick.rc_command.AIL,joystick.rc_command.BOTTON)
            print('Success')
            time.sleep(0.01)
#            data=b''
        except Exception:
            time.sleep(0.01)
            print('Lost')
            tcp.close()
            TCP_init()



if __name__ == '__main__':
    init()
    TCPserver()
