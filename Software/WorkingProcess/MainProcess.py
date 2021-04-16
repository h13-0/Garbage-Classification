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

    def __init__(self, ui, cam, com, thre, minArea, maxRetry):
        QWidget.__init__(self)

        # 传递参数
        self.__ui__ = ui
        self.__cam__ = cam
        self.__com__ = com
        self.__thre__ = thre
        self.__minArea__ = minArea
        self.__maxRetry__ = maxRetry

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

        # 设置蒙版
        self.__preModeMask__ = cv.imread("./ImageProcessing/singleModeMask.jpg")
        self.__finalModeMask__ = cv.imread("./ImageProcessing/multiModeMask.jpg")

        # 结果列表 (字符串型)
        self.__resultList__ = ""
        # 表示正在等待新物体
        self.__waiting__ = False


    # 主要逻辑部分
    def __main__(self):
        # Write your Code Here:
        

        # 开始计算尝试次数
        times = 0
        while True:
            if(self.__functions__.getCurrentMode() == Mode['preMode']):
                # 单分类模式
                self.__detector__.drawRect(True)
                self.__detector__.drawResult(True)

                self.__slave__.openBaffle("Left")
                self.__slave__.openBaffle("Left")
                self.__slave__.openBaffle("Right")
                self.__slave__.openBaffle("Right")

                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
                if(maxArea < self.__minArea__):
                    self.__detector__.pause()
                    self.__videoPlayer__.resume()
                    if(not self.__waiting__):
                        self.__outputSignal__.emit(self.__resultList__ + "正在等待被测物体...")
                        self.__waiting__ = True
                    time.sleep(0.1)

                else:
                    time.sleep(1.0)
                    maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
                    if(maxArea < self.__minArea__):
                        continue
                    # 物体大小大于阈值, 触发检测
                    self.__videoPlayer__.pause()
                    self.__detector__.show()
                    self.__waiting__ = False

                    name, classes, probability = self.__detector__.predict(self.__detector__.getFrame())

                    for i in range(3):
                        tempname, classes, probability = self.__detector__.predict(self.__detector__.getFrame())
                        if(not tempname == name):
                            break
                        if(i == 2):
                            currentID = 0
                            currentNum = 0

                            currentID, currentNum = self.__newGarbage__(classes)

                            self.__resultList__ += "投放顺序: " + str(currentID) + " " + classes + " 该种类垃圾数量: " + str(currentNum) + " ok!\r\n"

                            self.__outputSignal__.emit(self.__resultList__)

                            self.__sendGarbage__(classes)
                    
                    # 防止丢包
                    time.sleep(0.1)
                    self.__slave__.conveyorStop()
                    self.__slave__.conveyorStop()

                    # Do sth.
                    

            elif(self.__functions__.getCurrentMode() == Mode['finalMode']):
                # 多分类模式
                self.__detector__.drawRect(False)
                self.__detector__.drawResult(False)
                # 又得重复发包...
                self.__slave__.closeBaffle("Left")
                self.__slave__.closeBaffle("Left")
                self.__slave__.closeBaffle("Right")
                self.__slave__.closeBaffle("Right")

                leftPreProcessed, rightPreProcessed = self.__detector__.getSplitedFrame(self.__detector__.getPreprocessedFrame(self.__finalModeMask__))

                leftMaxArea, leftRect = self.__detector__.hasObject(leftPreProcessed)
                rightMaxArea, RightRect = self.__detector__.hasObject(rightPreProcessed)

            
                # 判断垃圾左右情况
                if((leftMaxArea > self.__minArea__) and (rightMaxArea > self.__minArea__)):
                    # 等待一下, 降低敏感度, 防止投放的时候手臂干扰导致的误判
                    time.sleep(1.5)
                    leftPreProcessed, rightPreProcessed = self.__detector__.getSplitedFrame(self.__detector__.getPreprocessedFrame(self.__finalModeMask__))
                    leftMaxArea, leftRect = self.__detector__.hasObject(leftPreProcessed)
                    rightMaxArea, RightRect = self.__detector__.hasObject(rightPreProcessed)
                    if((leftMaxArea < self.__minArea__) or (rightMaxArea < self.__minArea__)):
                        continue

                    self.__waiting__ = False
                    # 当左右成功分类
                    self.__videoPlayer__.pause()
                    self.__detector__.show()

                    self.__slave__.shakeMiddleBaffle()
                    self.__slave__.shakeMiddleBaffle()

                    # 对左右进行识别
                    self.__detectSplited__()

                    # 分类完毕后:
                    # 尝试次数归零
                    times = 0
                    self.__slave__.stopShakeMiddleBaffle()
                    self.__slave__.stopShakeMiddleBaffle()


                # 当其一左一右时:
                elif( ((leftMaxArea < self.__minArea__) and (rightMaxArea > self.__minArea__)) or ((leftMaxArea > self.__minArea__) and (rightMaxArea < self.__minArea__)) ):
                    # 等待一下, 降低敏感度, 防止投放的时候手臂干扰导致的误判
                    time.sleep(1.5)
                    leftPreProcessed, rightPreProcessed = self.__detector__.getSplitedFrame(self.__detector__.getPreprocessedFrame(self.__finalModeMask__))
                    leftMaxArea, leftRect = self.__detector__.hasObject(leftPreProcessed)
                    rightMaxArea, RightRect = self.__detector__.hasObject(rightPreProcessed)
                    if(times == 0):
                        if(not (((leftMaxArea < self.__minArea__) and (rightMaxArea > self.__minArea__)) or ((leftMaxArea > self.__minArea__) and (rightMaxArea < self.__minArea__)))):
                            continue

                    self.__videoPlayer__.pause()
                    self.__detector__.show()

                    self.__slave__.shakeMiddleBaffle()
                    self.__slave__.shakeMiddleBaffle()
                    self.__waiting__ = False

                    # 由于五号南孚电池有大量的黑色区域, opencv识别比较麻烦, 所以先用神经网络跑一下看看没有东西的那一边是不是电池
                    if(leftMaxArea < self.__minArea__):
                        Name, classes, probability = self.__detector__.predict(self.__detector__.getSplitedFrame()[0])
                    else:
                        Name, classes, probability = self.__detector__.predict(self.__detector__.getSplitedFrame()[1])
                    
                    if(Name == "电池"):
                        self.__videoPlayer__.pause()
                        self.__detector__.show()

                        self.__slave__.stopShakeMiddleBaffle()
                        self.__slave__.stopShakeMiddleBaffle()

                        # 对左右进行识别
                        self.__detectSplited__()

                        # 分类完毕后:
                        # 尝试次数归零
                        times = 0

                    # 在一定的尝试次数内:
                    if(times < self.__maxRetry__):
                        if(not self.__waiting__):
                            self.__outputSignal__.emit(self.__resultList__ + "正在尝试分离多个目标...")
                            self.__waiting__ = True

                        self.__slave__.conveyorBackward()
                        time.sleep(3)
                        self.__slave__.conveyorStop()
                        time.sleep(0.2)
                        # Shake
                        for i in range(10):
                            self.__slave__.conveyorForward()
                            time.sleep(0.15)
                            self.__slave__.conveyorBackward()
                            time.sleep(0.25)
                        
                        self.__slave__.conveyorForward()
                        time.sleep(3)

                        times += 1

                    # 当超过尝试次数:
                    else:
                        self.__slave__.conveyorForward()
                        time.sleep(2)
                        self.__slave__.conveyorStop()
                        time.sleep(0.5)
                        
                        # 对左右进行识别
                        self.__detectSplited__()
                        times = 0

                    self.__slave__.stopShakeMiddleBaffle()
                    self.__slave__.stopShakeMiddleBaffle()
                    
                # 当没有目标物体时:
                elif((leftMaxArea < self.__minArea__) and (rightMaxArea < self.__minArea__)):
                    self.__detector__.pause()
                    self.__videoPlayer__.resume()
                    if(not self.__waiting__):
                        self.__outputSignal__.emit(self.__resultList__ + "正在等待被测物体...")
                        self.__waiting__ = True
                    self.__slave__.stopShakeMiddleBaffle()
                    self.__slave__.stopShakeMiddleBaffle()
                    time.sleep(0.1)

                # 最后停下传送带
                self.__slave__.conveyorStop()
                time.sleep(0.2)


    def __detectSplited__(self):
        # 先左后右判断目标种类
        resultClasses = []
        existBattery = False
        batteryPosition = 0
        for splitIndex in range(2):
            # 判断目标种类
            name = ""
            
            for i in range(3):
                tempname, classes, probability = self.__detector__.predict(self.__detector__.getSplitedFrame(self.__detector__.getFrame())[splitIndex])
                # 当识别结果发生变化
                if(not tempname == name):
                    name = tempname
                    # 重新计次
                    i = 0

                # 当连续三次为同一结果
                if(i == 2):
                    resultClasses.append(classes)
                    if(tempname == "电池"):
                        existBattery = True
                        batteryPosition = splitIndex

                    currentID = 0
                    currentNum = 0

                    currentID, currentNum = self.__newGarbage__(classes)

                    self.__resultList__ += "投放顺序: " + str(currentID) + " " + classes + " 该种类垃圾数量: " + str(currentNum) + " ok!\r\n"
                    self.__outputSignal__.emit(self.__resultList__) 

        # 由于电池用传送带不好运输 所以在检测到电池时对电池优先运输
        if(existBattery):
            if(batteryPosition == 0):
                self.__slave__.openBaffle("Right")
                time.sleep(0.05)
                self.__slave__.openBaffle("Right")

            else:
                self.__slave__.openBaffle("Left")
                time.sleep(0.05)
                self.__slave__.openBaffle("Left")
            
            # 等待0.3秒 等待挡板完全打开
            time.sleep(0.3)
            self.__sendGarbage__("有害垃圾")

            # 输送另外一个垃圾
            if(batteryPosition == 0):
                self.__slave__.openBaffle("Left")
                time.sleep(0.05)
                self.__slave__.openBaffle("Left")

            else:
                self.__slave__.openBaffle("Right")
                time.sleep(0.05)
                self.__slave__.openBaffle("Right")

            time.sleep(0.3)
            if(1 - batteryPosition < len(resultClasses)):
                self.__sendGarbage__(resultClasses[1 - batteryPosition])

        else:
            for i in range(2):
                if(i == 0):
                    self.__slave__.openBaffle("Right")
                    time.sleep(0.05)
                    self.__slave__.openBaffle("Right")

                    # 等待0.3秒 等待挡板完全打开
                    time.sleep(0.3)

                    self.__sendGarbage__(resultClasses[i])
                else:
                    if(i < len(resultClasses)):
                        self.__slave__.openBaffle("Left")
                        time.sleep(0.05)
                        self.__slave__.openBaffle("Left")

                        # 等待0.3秒 等待挡板完全打开
                        time.sleep(0.3)

                        self.__sendGarbage__(resultClasses[i])
    

    # 垃圾处理函数
    def __sendGarbage__(self, classes):
        if(classes == "有害垃圾"):
            self.__slave__.conveyorBackward()
            time.sleep(0.15)
            self.__slave__.conveyorForward()
            startTime = time.time()
            while(time.time() - startTime < 5.0):
                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
            
            self.__slave__.conveyorStop()
            self.__slave__.conveyorStop()
            time.sleep(0.1)
            self.__slave__.sendHarmful()
        
        elif(classes == "厨余垃圾"):
            self.__slave__.conveyorBackward()
            time.sleep(0.15)
            self.__slave__.conveyorForward()
            startTime = time.time()
            while(time.time() - startTime < 3.0):
                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
            
            for i in range(7):
                self.__slave__.conveyorBackward()
                time.sleep(0.15)
                self.__slave__.conveyorForward()
                time.sleep(0.15)
            
            self.__slave__.conveyorStop()
            self.__slave__.conveyorStop()

            time.sleep(0.1)
            self.__slave__.sendKitchen()
            
        elif(classes == "可回收垃圾"):
            self.__slave__.conveyorForward()
            startTime = time.time()
            while(time.time() - startTime < 3.0):
                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
            
            for i in range(7):
                self.__slave__.conveyorBackward()
                time.sleep(0.20)
                self.__slave__.conveyorForward()
                time.sleep(0.20)
            
            self.__slave__.conveyorStop()
            self.__slave__.conveyorStop()

            time.sleep(0.1)
            self.__slave__.sendRecycle()
        
        elif(classes == "其他垃圾"):
            self.__slave__.conveyorForward()
            startTime = time.time()
            while(time.time() - startTime < 3.0):
                maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
            
            self.__slave__.conveyorStop()
            self.__slave__.conveyorStop()
            time.sleep(0.1)
            self.__slave__.sendOther()

        # 更新方框
        maxArea, rect = self.__detector__.hasObject(self.__detector__.getPreprocessedFrame(self.__preModeMask__))
        

    # 新垃圾数量统计
    def __newGarbage__(self, classes):
        if(classes == "可回收垃圾"):
            return self.__garbageMessage__.newRecycle()
        elif(classes == "厨余垃圾"):
            return self.__garbageMessage__.newKitchen()
        elif(classes == "其他垃圾"):
            return self.__garbageMessage__.newOther()
        elif(classes == "有害垃圾"):
            return self.__garbageMessage__.newHarmful()


    # 子线程初始化detector
    def __detectorInit__(self):
        # 初始化Detector
        with self.__detectorLock__:
            ## 由于OpenCV无法正确显示中文, 并且比赛要求显示中文, 所以只能中英同时使用...
            className = [ ['电池', 'Battery', '有害垃圾'], ['易拉罐', 'Cans', '可回收垃圾'], 
                ['烟蒂', 'Cigarettebutts', '其他垃圾'], ['果蔬', 'Kitchen', '厨余垃圾'], ['果蔬', 'Kitchen', '厨余垃圾'], 
                ['碎瓷砖', 'Tile', '其他垃圾'], ['蔬菜', 'Vegetables', '厨余垃圾'], ['水瓶', 'Walterbottles', '可回收垃圾'] ]
            self.__detector__ = Detector(self.__ui__, self.__cam__, self.__thre__, "./weight/Result.hdf5", className)

            ## 即使模型加载完毕后, 第一次预测也需要大量时间, 所以这里直接进行第一次预测
            self.__detector__.predict(self.__detector__.getFrame())

        # 创建下位机对象
        try:
            self.__slave__ = Slave(self.__com__)
            self.__garbageMessage__.setSlave(self.__slave__)
        except Exception:
            print("串口: " + str(self.__com__) + " 打开失败")
            msg_box = QMessageBox.critical(self, "Error!", "串口: " + str(self.__com__) + " 打开失败",  QMessageBox.Ok, QMessageBox.Ok)

        # 发送Detector加载完毕的信号
        self.__detectorOkSignal__.emit()

        # 加载完毕
        self.__outputSignal__.emit(self.__resultList__ + "加载完毕")


    # Detector加载完毕后执行以下内容
    def __detectorInited__(self):
        # 初始化功能区面板
        self.__functions__ = Functions(self.__ui__, self.__videoPlayer__, self.__detector__)
        # 初始化背景阈值Slider
        self.__functions__.setThreSliderValue(self.__thre__)

        # 创建子线程
        self.__mainThread__ = threading.Thread(target=self.__main__)
        self.__mainThread__.setDaemon(True)
        self.__mainThread__.start()
