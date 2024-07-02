import serial
import time

ser = serial.Serial("/dev/ttyAMA0", 115200)

def serial_open():
	
	if ser.isOpen():
	    print("serial ready")
	    print(ser.name)
	else:
	    print("serial lost")
#	a=b'65536'
#	ser.write(a)


if __name__ == '__main__':
    serial_open()


# ser.write(recv)
# count = ser.inWaiting()
# if count != 0:
    # recv = ser.read(count)
