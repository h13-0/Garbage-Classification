# import Tensorflow
import tensorflow as tf
from tensorflow.keras.preprocessing import image

# import PyQt5
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget

# Helper libraries
import cv2 as cv
import numpy as np
import threading, signal
import os
import time

class Detector(threading.Thread, QWidget):
    # 结果输出用editText的信号槽
    __outputSignal__ = pyqtSignal([str])
 
    def __init__(self, ui, cameraID, weigthFile, className):
        threading.Thread.__init__(self)
        QWidget.__init__(self)
        
        self.__qlabel__ = ui.frame_label
        self.cameraID = cameraID
        self.className = className

        '''
        # PreMode   初赛模式 -> 即一次投入一个垃圾
        # FinalMode 决赛模式 -> 即一次投入两个垃圾
        '''
        self.mode = "PreMode"

        # 连接信号
        self.__outputSignal__.connect(ui.outputResult)

        # Camera
        self.__capLock__ = threading.Lock()
        self.__cap__ = cv.VideoCapture(self.cameraID)
        self.__cap__.set(cv.CAP_PROP_FRAME_WIDTH, 640)
        self.__cap__.set(cv.CAP_PROP_FRAME_HEIGHT, 480)

        if(not self.__cap__.isOpened):
            raise Exception("Could not open camera: " + str(self.cameraID))

        # 创建显示线程且立即阻塞
        self.__displayFlag__ = threading.Event()
        self.__displayFlag__.clear()

        self.__displayThread__ = threading.Thread(target=self.__display__, )
        self.__displayThread__.setDaemon(True)
        self.__displayThread__.start()

        # Build Model
        physical_devices = tf.config.experimental.list_physical_devices('GPU')
        for physical_device in physical_devices:
            tf.config.experimental.set_memory_growth(physical_device, True)
        self.__model__ = tf.keras.applications.Xception(include_top=True,weights=weigthFile,input_shape=(299,299,3),classes=len(className),pooling="avg")

        # 加载完毕
        self.__outputSignal__.emit("加载完毕")


    # 获取单帧图像
    def getFrame(self):
        with self.__capLock__:
            ret, frame = self.__cap__.read()
            return frame


    # 需要显示的时候才调用此方法
    def show(self):
        self.__displayFlag__.set()


    # 但是不释放摄像头(懒得写)
    def pause(self):
        self.__displayFlag__.clear()


    # 私有方法, 显示图像用
    def __display__(self):
        while True:
            self.__displayFlag__.wait()
            image = self.getFrame()
            image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
            qImage = QImage(image.data, 640, 480, 640 * 3, QImage.Format_RGB888)
            self.__qlabel__.setPixmap(QPixmap.fromImage(qImage))
            time.sleep(0.02)


    # 将Frame左右分割
    ## 可选数据源
    def getSplitedFrame(self, frame = None):
        if frame is None:
            frame = self.getFrame()

        frame = cv.resize(frame, (398, 299))

        if(frame.ndim == 2):
            # 单通道图
            # 黑色底图
            leftFrame = np.zeros((299, 299), dtype= np.uint8)
            rightFrame = np.zeros((299, 299), dtype= np.uint8)

            # 将frame左右分别拷贝到leftFrame和rightFrame的中间
            np.copyto(leftFrame[0:299, 49:248], frame[0:299, 0:199])
            np.copyto(rightFrame[0:299, 49:248], frame[0:299, 199:398])

        else:
            # 三通道
            # 黑色底图
            leftFrame = np.zeros((299, 299, 3), dtype= np.uint8)
            rightFrame = np.zeros((299, 299, 3), dtype= np.uint8)

            # 将frame左右分别拷贝到leftFrame和rightFrame的中间
            np.copyto(leftFrame[0:299, 49:248], frame[0:299, 0:199])
            np.copyto(rightFrame[0:299, 49:248], frame[0:299, 199:398])

        return leftFrame, rightFrame


    # 检测垃圾是否被放置
    def hasObject(self):
        # threshold
        ret, thre = cv.threshold(self.getFrame(), 100, 255, cv.THRESH_TOZERO)

        # erode
        kernel = np.ones((5,5),np.uint8) 
        erosion = cv.erode(thre, kernel, iterations = 1)

        # BGR To Gray
        gray = cv.cvtColor(erosion, cv.COLOR_BGR2GRAY)

        # Split image
        left, right = self.getSplitedFrame(gray)
        
        # findContours
        leftContours, leftHierarchy = cv.findContours(left, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)
        rightContours, rightHierarchy = cv.findContours(right, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)

        # output maximum area
        leftArea = 0
        for contour in leftContours:
            area = cv.contourArea(contour)
            if(area > leftArea):
                leftArea = area
        
        rightArea = 0
        for contour in rightContours:
            area = cv.contourArea(contour)
            if(area > rightArea):
                rightArea = area

        return leftArea, rightArea


    def switchMode(self, mode = "PreMode"):
        if((mode == 'PreMode') or (mode == 'FinalMode')):
            self.mode = mode
    
        
    # 预测种类
    def predict(self):
        imgList = self.getSplitedFrame()
        imgs = np.array(imgList)

        predictions = self.__model__.predict(imgs)

        self.__outputSignal__.emit(
            "左侧: " + self.className[np.argmax(predictions[0])][0] + ", 是: " + self.className[np.argmax(predictions[0])][1] + " " + str(100*np.max(predictions[0])) + "%" + '\r\n' + 
            "右侧: " + self.className[np.argmax(predictions[1])][0] + ", 是: " + self.className[np.argmax(predictions[1])][1] + " " + str(100*np.max(predictions[1])) + "%"
        )
    

    # 录入新的物体(模板匹配)
    def newObject(self):
        pass