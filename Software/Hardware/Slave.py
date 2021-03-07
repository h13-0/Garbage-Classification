# import pyserial
import serial

# import Helper libs
import threading

class Slave():
    def __init__(self, com):
        self.__usart__ = serial.Serial(com, 115200, timeout=None)

        # 垃圾桶容量信息
        self.__loadLock__ = threading.Lock()
        self.__recycleLoad__ = 0
        self.__kitchenLoad__ = 0
        self.__harmfulLoad__ = 0
        self.__otherLoad__ = 0

        self.__receiveThread__ = threading.Thread(target=self.__receive__)
        self.__receiveThread__.setDaemon(True)
        self.__receiveThread__.start()


    def __receive__(self):
        while True:
            try:
                msg = self.__usart__.readline().decode(encoding = "utf-8")
                msg = msg.replace("\r","").replace("\n","")
            except Exception:
                msg = ""

            if(msg.find(":") > 1):
                sp = msg.split(":")

                # 垃圾桶容量信息
                if(sp[0] == 'Capacity'):
                    # 校验
                    if(len(sp) == 3):
                        with self.__loadLock__:
                            
                            # 可回收垃圾
                            if(sp[1] == "Recycle"):
                                self.__recycleLoad__ = int(sp[2])

                            # 厨余垃圾
                            elif(sp[1] == "Kitchen"):
                                self.__kitchenLoad__ = int(sp[2])

                            # 有害垃圾
                            elif(sp[1] == "Harmful"):
                                self.__harmfulLoad__ = int(sp[2])

                            # 其他垃圾
                            elif(sp[1] == "Other"):
                                self.__otherLoad__ = int(sp[2])

                # 挡板事件
                elif(sp[0] == 'Baffle'):
                    # 左侧挡板
                    if(sp[1] == 'Left'):
                        if(sp[2] == 'Opened'):
                            # Do someting
                            pass

                        elif(sp[2] == 'Closed'):
                            # Do something
                            pass

                        

                    # 右侧挡板
                    elif(sp[1] == 'Right'):
                        if(sp[2] == 'Opened'):
                            # Do someting
                            pass

                        elif(sp[2] == 'Closed'):
                            # Do something
                            pass

                    # 终端挡板
                    elif(sp[1] == 'Middle'):
                        if(sp[2] == 'Opened'):
                            # Do someting
                            pass

                        elif(sp[2] == 'Closed'):
                            # Do something
                            pass


        
    # 返回容量信息
    def getLoadTest(self):
        return self.__recycleLoad__, self.__kitchenLoad__, self.__harmfulLoad__, self.__otherLoad__

    
    def openBaffle(self, target, timeout):
        self.__usart__.write(("Baffle:" + target + "Open\r\n").encode("utf-8"))
        # wait

    
    def closeBaffle(self, target, timeout):
        self.__usart__.write(("Baffle:" + target + "Close\r\n").encode("utf-8"))
        # wait

    def conveyorForward(self):
        self.__usart__.write(("Conveyor" + "Forward\r\n").encode("utf-8"))