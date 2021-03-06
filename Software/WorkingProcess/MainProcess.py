# import PyQt5
from PyQt5.QtWidgets import QWidget
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

class Process(QWidget):
    __detectorOkSignal__ = pyqtSignal()

    def __init__(self, ui):
        QWidget.__init__(self)

        self.ui = ui

        self.functions = None
        # 设置信号
        self.__detectorOkSignal__.connect(lambda:self.functionInit())
    
        # 创建一个新线程以初始化Detector
        self.detector = None
        self.detectorLock = threading.Lock()
        self.detectorInitThread = threading.Thread(target=self.detectorInit)
        self.detectorInitThread.setDaemon(True)
        self.detectorInitThread.start()

        # 初始化垃圾桶信息区域
        self.garbageMessage = GarbageMessage(self.ui)

        # 开始播放视频
        self.videoPlayer = VideoPlayer(self.ui.frame_label)
        self.videoPlayer.fromVideo()
        self.videoPlayer.start()

        # 创建子线程
        self.mainThread = threading.Thread(target=self.main)
        self.mainThread.setDaemon(True)
        self.mainThread.start()


    def main(self):
        # 等待detector加载完毕
        with self.detectorLock:
            pass


        # end
        while True:
            pass


    def detectorInit(self):
        # 初始化Detector
        with self.detectorLock:
            className = [ ['电池', '有害垃圾'] , ['碎瓷片', '其他垃圾'] , ['易拉罐', '可回收垃圾'] , ['烟蒂', '其他垃圾'] , ['药物', '有害垃圾'] ,
                    ['水果', '厨余垃圾'] , ['纸张', '可回收垃圾'], ['碎砖块', '其他垃圾'] , ['蔬菜', '厨余垃圾'] , ['水瓶', '可回收垃圾'] ]
            self.detector = Detector(self.ui, 0, "./weight/Result.hdf5", className)

        self.__detectorOkSignal__.emit()


    # 等待Detector加载完毕后初始化功能区
    def functionInit(self):
        self.functions = Functions(self.ui, self.videoPlayer, self.detector)
