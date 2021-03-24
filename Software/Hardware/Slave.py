# import pyserial
import serial

# import Helper libs
import threading

# import Helper libs
import time

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

        self.__isSliderOK__ = False
        self.__SliderOKLock__ = threading.Lock()


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
                    # 校验
                    if(len(sp) == 3):
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

                # 滑块事件
                elif(sp[0] == 'Slider'):
                    # 校验
                    if(len(sp) == 2):
                        if(sp[1] == 'OK'):
                            # Do someThing
                            with self.__SliderOKLock__:
                                self.isSliderOK = True
                            pass


    # 返回容量信息
    def getLoadTest(self):
        return self.__recycleLoad__, self.__kitchenLoad__, self.__harmfulLoad__, self.__otherLoad__


    # 托盘运送垃圾
    ## 懒得用阻塞了 直接等待就行
    def sendRecycle(self, timeout = 5):
        return self.__sendGarbage__("Recycle", timeout)


    def sendKitchen(self, timeout = 5):
        return self.__sendGarbage__("Kitchen", timeout)


    def sendHarmful(self, timeout = 5):
        return self.__sendGarbage__("Harmful", timeout)


    def sendOther(self, timeout = 5):
        return self.__sendGarbage__("Other", timeout)

    
    def __sendGarbage__(self, classes, timeout):
        startTime = time.time()
        with self.__SliderOKLock__:
            self.isSliderOK = False
        
        self.__usart__.write(("Slider:" + classes).encode("utf-8"))
        while((time.time() - startTime < timeout*1000) or not self.isSliderOK):
            time.sleep(0.1)

        with self.__SliderOKLock__:
            return self.__isSliderOK__


    # 开挡板
    def openBaffle(self, target, timeout):
        self.__usart__.write(("Baffle:" + target + "Open\r\n").encode("utf-8"))
        # wait

    
    # 关挡板
    def closeBaffle(self, target, timeout):
        self.__usart__.write(("Baffle:" + target + "Close\r\n").encode("utf-8"))
        # wait

    def conveyorForward(self):
        self.__usart__.write(("Conveyor:" + "Forward\r\n").encode("utf-8"))

    def sliderSend(self, classes):
        self.__usart__.write(("Slider:" + classes + "\r\n").encode("utf-8"))

