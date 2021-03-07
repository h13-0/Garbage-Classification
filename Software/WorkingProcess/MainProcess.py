# import PyQt5
from PyQt5.QtWidgets import QWidget, QMessageBox
from PyQt5.QtCore import pyqtSignal

# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import threading
import time

# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

## GarbageFunction
from GarbageFunction.GarbageMessage import GarbageMessage

## FunctionAreas
from FunctionAreas.FunctionAreas import Functions

## Hardware
from Hardware.Slave import Slave

class Process(QWidget):
    __detectorOkSignal__ = pyqtSignal()

    def __init__(self, ui, cam, com, thre, minArea):
        QWidget.__init__(self)

        self.__ui__ = ui
        self.__cam__ = cam
        self.__com__ = com

        self.functions = None
        # 设置信号
        self.__detectorOkSignal__.connect(lambda:self.__detectorInited__())
    
        # 创建一个新线程以初始化Detector
        self.__detector__ = None
        self.__detectorLock__ = threading.Lock()
        self.__detectorInitThread__ = threading.Thread(target=self.__detectorInit__)
        self.__detectorInitThread__.setDaemon(True)
        self.__detectorInitThread__.start()

        # 开始轮流播放
        self.__videoPlayer__ = VideoPlayer(self.__ui__.frame_label)
        self.__videoPlayer__.takeTurns()
        self.__videoPlayer__.start()

        # 初始化垃圾桶信息区域
        self.__garbageMessage__ = GarbageMessage(self.__ui__)


    # 主要逻辑部分
    def __main__(self):
        # Write your Code Here:
        while True:
            pass


    # 子线程初始化detector
    def __detectorInit__(self):
        # 初始化Detector
        with self.__detectorLock__:
            className = [ ['电池', '有害垃圾'] , ['碎瓷片', '其他垃圾'] , ['易拉罐', '可回收垃圾'] , ['烟蒂', '其他垃圾'] , ['药物', '有害垃圾'] ,
                    ['水果', '厨余垃圾'] , ['纸张', '可回收垃圾'], ['碎砖块', '其他垃圾'] , ['蔬菜', '厨余垃圾'] , ['水瓶', '可回收垃圾'] ]
            self.__detector__ = Detector(self.__ui__, self.__cam__, "./weight/Result.hdf5", className)

        # 发送Detector加载完毕的信号
        self.__detectorOkSignal__.emit()


    # Detector加载完毕后执行以下内容
    def __detectorInited__(self):
        self.__functions__ = Functions(self.__ui__, self.__videoPlayer__, self.__detector__)

        # 创建子线程
        self.__mainThread__ = threading.Thread(target=self.__main__)
        self.__mainThread__.setDaemon(True)
        self.__mainThread__.start()

        # 创建下位机对象
        try:
            self.__slave__ = Slave(self.__com__)
            self.__garbageMessage__.setSlave(self.__slave__)
        except Exception:
            print("串口: " + str(self.__com__) + " 打开失败")
            msg_box = QMessageBox.critical(self, "Error!", "串口: " + str(self.__com__) + " 打开失败",  QMessageBox.Ok, QMessageBox.Ok)
