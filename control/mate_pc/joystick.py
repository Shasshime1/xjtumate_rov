import ctypes
import time
import sys


class RCCommand(ctypes.Structure):
    _fields_ = [
        ("THR", ctypes.c_uint16),
        ("RUD", ctypes.c_uint16),
        ("ELE", ctypes.c_uint16),
        ("AIL", ctypes.c_uint16),
        ("AUX", ctypes.c_uint16),
        ("BOTTON", ctypes.c_uint16)
    ]


def RC_ValueChange(joyinfoex):
    rc_command.THR = joyinfoex.dwZpos // 256
    rc_command.RUD = joyinfoex.dwVpos // 256
    rc_command.ELE = joyinfoex.dwYpos // 256
    rc_command.AIL = joyinfoex.dwXpos // 256
    rc_command.AUX = joyinfoex.dwRpos // 256
    rc_command.BOTTON = joyinfoex.dwButtons


class JOYINFOEX(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_ulong),
        ("dwFlags", ctypes.c_ulong),
        ("dwXpos", ctypes.c_ulong),
        ("dwYpos", ctypes.c_ulong),
        ("dwZpos", ctypes.c_ulong),
        ("dwRpos", ctypes.c_ulong),
        ("dwUpos", ctypes.c_ulong),
        ("dwVpos", ctypes.c_ulong),
        ("dwButtons", ctypes.c_ulong),
        ("dwButtonNumber", ctypes.c_ulong),
        ("dwPOV", ctypes.c_ulong),
        ("dwReserved1", ctypes.c_ulong),
        ("dwReserved2", ctypes.c_ulong)
    ]


def joystick_init():
    global winmm, joyinfoex, rc_command
    winmm = ctypes.windll.winmm
    joyinfoex = JOYINFOEX()
    joyinfoex.dwSize = ctypes.sizeof(JOYINFOEX)
    joyinfoex.dwFlags = 0x000000FF
    rc_command = RCCommand()
    print('Waiting for Joystick')
    while True:
        time.sleep(0.2)
        if winmm.joyGetNumDevs() >= 1:
            break
    joyreturn = winmm.joyGetPosEx(0, ctypes.byref(joyinfoex))
    RC_ValueChange(joyinfoex)
    while True:
        time.sleep(0.2)
        sys.stdout.flush()
        if winmm.joyGetPosEx(0, ctypes.byref(joyinfoex)) == 0:
            print('Joystick Connected')
            RC_ValueChange(joyinfoex)
            print('CH1(AIL):', rc_command.AIL)
            print('CH2(ELE):', rc_command.ELE)
            print('CH3(THR):', rc_command.THR)
            print('CH4(RUD):', rc_command.RUD)
            print('CH5(AUX):', rc_command.AUX)
            print('CH6(BUT):', rc_command.BOTTON)
            break
    # else:
    # print('Waiting for Joystick')


    #    if ctypes.windll.msvcrt._kbhit():  # Check if a key is pressed
    #        break

def TX_refresh(TX):
    TX[0] =0xff
    TX[1] =rc_command.AIL
    TX[2] =rc_command.ELE
    TX[3] =rc_command.THR
    TX[4] =rc_command.RUD
    TX[5] =rc_command.AUX
    TX[6] =rc_command.BOTTON
    TX[7] =0
    TX[8] =0
    TX[9] =0
    TX[10]=0
    TX[11]=0x0f


def joystick_input():
    joyNums = winmm.joyGetNumDevs()
    if joyNums >= 1:
        joyreturn = winmm.joyGetPosEx(0, ctypes.byref(joyinfoex))
        RC_ValueChange(joyinfoex)
        if joyreturn != 0:
            error_msgs = {
                1: "bad parameters",
                2: "request not completed",
                6: "joystick is unplugged"
            }
            time.sleep(1)
            default_msg = "unknown error"
            print(error_msgs.get(joyreturn, default_msg))
            joystick_init()