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

# import Working mode definition
from WorkingProcess.WorkingMode import Mode

import cv2 as cv

class Process(QWidget):
    # 结果输出用editText的信号槽
    __outputSignal__ = pyqtSignal([str])
    
    # Detector加载完毕的信号槽
    __detectorOkSignal__ = pyqtSignal()

    def __init__(self, ui, cam, com, thre, minArea):
        QWidget.__init__(self)

        self.__ui__ = ui
        self.__cam__ = cam
        self.__com__ = com
        self.thre = thre

        self.functions = None

        # 设置信号
        self.__detectorOkSignal__.connect(lambda:self.__detectorInited__())
        
        self.__outputSignal__.connect(ui.outputResult)
    
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
        self.__detector__.drawRect(True)
        self.__detector__.drawResult(True)
        mask = cv.imread("./ImageProcessing/mask.png")
        while True:
            if(self.__functions__.getCurrentMode() == Mode['preMode']):
                # 单分类模式
                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))
                if(maxArea < 1000):
                    self.__detector__.pause()
                    self.__videoPlayer__.resume()
                    self.__outputSignal__.emit("正在等待被测物体...")
                    time.sleep(0.1)

                else:
                    # 物体大小大于阈值, 触发检测
                    self.__videoPlayer__.pause()
                    self.__detector__.show()

                    name, classes, probability = self.__detector__.predict(self.__detector__.getFrame())

                    for i in range(3):
                        tempname, classes, probability = self.__detector__.predict(self.__detector__.getFrame())
                        if(not tempname == name):
                            break
                        if(i == 2):
                            self.__outputSignal__.emit(
                                name + ", 是: " + classes + " " + probability
                            )

                            if(classes == "有害垃圾"):
                                self.__slave__.conveyorForward()
                                startTime = time.time()
                                
                                while(time.time() - startTime < 3.5):
                                    maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))
                                    

                                self.__slave__.sendHarmful()
                                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))
                                #self.__garbageMessage__.

                            elif(classes == "可回收垃圾"):
                                self.__slave__.conveyorForward()
                                startTime = time.time()
                                
                                while(time.time() - startTime < 3.5):
                                    maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                                self.__slave__.sendRecycle()
                                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                            elif(classes == "其他垃圾"):
                                self.__slave__.conveyorForward()
                                startTime = time.time()
                                
                                while(time.time() - startTime < 3.5):
                                    maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                                self.__slave__.sendOther()
                                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                            elif(classes == "厨余垃圾"):
                                self.__slave__.conveyorForward()
                                startTime = time.time()
                                
                                while(time.time() - startTime < 3.5):
                                    maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                                self.__slave__.sendKitchen()
                                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(mask))

                    self.__slave__.conveyorStop()


                    

                    # Do sth.
                    

            elif(self.__functions__.getCurrentMode() == Mode['finalMode']):
                # 多分类模式
                '''
                self.__outputSignal__.emit(
                    "左侧: " + self.__className__[np.argmax(predictions[0])][0] + ", 是: " + self.__className__[np.argmax(predictions[0])][1] + " " + str(100*np.max(predictions[0])) + "%" + '\r\n' + 
                    "右侧: " + self.__className__[np.argmax(predictions[1])][0] + ", 是: " + self.__className__[np.argmax(predictions[1])][1] + " " + str(100*np.max(predictions[1])) + "%"
                )
                '''
                pass


    # 子线程初始化detector
    def __detectorInit__(self):
        # 初始化Detector
        with self.__detectorLock__:
            ## 由于OpenCV无法正确显示中文, 并且比赛要求显示中文, 所以只能中英同时使用...
            className = [ ['电池', 'Battery', '有害垃圾'], ['碎瓷片', 'Brokenceramics', '其他垃圾'], ['易拉罐', 'Can', '可回收垃圾'], 
                ['烟蒂', 'Cigarettebutts', '其他垃圾'], ['水果', 'Fruit', '厨余垃圾'], ['纸张', 'Paper', '可回收垃圾'], 
                ['碎砖块', 'Tile', '其他垃圾'], ['蔬菜', 'Vegetables', '厨余垃圾'], ['水瓶', 'Walterbottles', '可回收垃圾'] ]
            self.__detector__ = Detector(self.__ui__, self.__cam__, self.thre, "./weight/Result.hdf5", className)

        # 发送Detector加载完毕的信号
        self.__detectorOkSignal__.emit()

        # 加载完毕
        self.__outputSignal__.emit("加载完毕")


    # Detector加载完毕后执行以下内容
    def __detectorInited__(self):
        # 初始化功能区面板
        self.__functions__ = Functions(self.__ui__, self.__videoPlayer__, self.__detector__)
        # 初始化背景阈值Slider
        self.__functions__.setThreSliderValue(self.thre)

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
