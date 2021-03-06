# import pyserial
import serial

class Slave():
    def __init__(self, com = '/dev/tty'):
        self.__usart__ = serial.Serial(com, 115200, timeout=3)


    def __receive__(self):
        self.__usart__.readline()

    def loadTest(self):
        pass