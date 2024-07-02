import sys
import time
import socket
import RPi.GPIO
import serial

import tcp_receive
import serial_tx


def init():
	serial_tx.serial_open()
	tcp_receive.tcp_open()



def trans():
	while 1:
		#if tcp_receive.tcp.connect(tcp_receive.address)==0:
		#try:
			tcp_receive.tcp_receive()
			#serial_tx.ser.write(tcp_receive.data)
			#print(tcp_receive.data)
		#except Exception:
		#else:
			#connection.close()
			#tcp_receive.tcp_open()

init()
trans()



# if __name__ == '__main__':
    # init()





