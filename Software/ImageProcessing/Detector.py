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

# import resize tool
from ImageProcessing.ResizeTo import ResizeTo

class Detector(threading.Thread, QWidget):
    def __init__(self, ui, cameraID, threValue, weigthFile, className):
        threading.Thread.__init__(self)
        QWidget.__init__(self)
        
        # UI和摄像头
        self.__qlabel__ = ui.frame_label
        self.cameraID = cameraID

        # 背景阈值相关参数
        self.threValue = threValue
        self.threValueLock = threading.Lock()

        # 目标物体所在Rect
        ## 选择是否绘出Rect, 共用一个锁即可
        self.__drawRect__ = False

        ## 目标所在方框
        self.__rect__ = None
        self.__rectLock__ = threading.Lock()

        # 目标的边缘
        ## 选择是否绘出Contour
        self.__drawContour__ = False
        
        ## 目标轮廓
        self.__contour__ = None
        self.__contourLock__ = threading.Lock()

        # label和种类信息
        self.__className__ = className

        # 输出结果, 共用一个锁即可
        ## 选择是否在图像中标出结果
        self.__drawResult__ = False

        ## 推演出的结果
        self.__nameResult__ = None
        self.__probabilityResult__ = None
        self.__resultLock__ = threading.Lock()


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

            # 获取是否需要画出方框
            ## TODO: 支持多方框
            ## TODO: 使用完一次后即销毁
            rect = None
            with self.__rectLock__:
                if(self.__drawRect__):
                    rect = self.__rect__

            # 获取是否需要画出结果
            ## TODO: 支持多结果
            ## TODO: 使用完一次后即销毁
            nameResult = None
            probabilityResult = None

            with self.__resultLock__:
                if(self.__drawResult__):
                    nameResult = self.__nameResult__
                    probabilityResult = self.__probabilityResult__

            # 绘制方框
            if(not rect is None):
                image = cv.rectangle(image, self.__rect__, (0, 255, 0), 2)

            # 绘制结果
            if(not nameResult is None):
                image = cv.putText(image, nameResult + " " + probabilityResult, (rect[0], rect[1] - 4), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 255, 0), 2)
                pass

            image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
            qImage = QImage(image.data, 640, 480, 640 * 3, QImage.Format_RGB888)
            self.__qlabel__.setPixmap(QPixmap.fromImage(qImage))
            time.sleep(0.03)


    # 预处理图像, 用黑色方块覆盖机械部分
    def getPreprocessedFrame(self, mask):
        return cv.bitwise_and(self.getFrame(), mask)


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
    def hasObject(self, frame):
        ret = False
        thre = None
        # threshold
        with self.threValueLock:
            ret, thre = cv.threshold(frame, self.threValue, 255, cv.THRESH_TOZERO)

        # erode
        kernel = np.ones((5,5),np.uint8) 
        erosion = cv.erode(thre, kernel, iterations = 1)

        # BGR To Gray
        gray = cv.cvtColor(erosion, cv.COLOR_BGR2GRAY)
        
        # findContours
        Contours, Hierarchy = cv.findContours(gray, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)

        # output maximum area
        maxArea = 0
        maxContour = None
        for contour in Contours:
            area = cv.contourArea(contour)
            if(area > maxArea):
                maxArea = area
                maxContour = contour

        # output Rect of Item
        with self.__rectLock__:
            self.__rect__ = cv.boundingRect(maxContour)

            return maxArea, self.__rect__


    # 选择是否需要画出方框
    def drawRect(self, value):
        with self.__rectLock__:
            self.__drawRect__ = value


    # 选择是否绘制出边缘
    ## 这样效果会和Yolo有很明显的区别
    def drawContour(self, value):
        with self.__contourLock__:
            self.drawRect = value


    # 选择是否在图像中标出结果
    def drawResult(self, value):
        with self.__resultLock__:
            self.__drawResult__ = value
        

    # 预测种类
    # return: Name, classes, probability
    def predict(self, frame):
        frame = ResizeTo(frame, 299)
        frame = frame.astype(np.float32) / 255.0
        frame = image.img_to_array(cv.cvtColor(frame, cv.COLOR_BGR2RGB))
        imgList = []
        imgList.append(frame)
        imgs = np.array(imgList)

        predictions = self.__model__.predict(imgs)

        with self.__resultLock__:
            self.__nameResult__ = self.__className__[np.argmax(predictions[0])][1]
            self.__probabilityResult__ = str(100*np.max(predictions[0])) + "%"
            
            return self.__className__[np.argmax(predictions[0])][0], self.__className__[np.argmax(predictions[0])][2], self.__probabilityResult__
    

    # 录入新的物体(模板匹配)
    ## 省赛不需要 暂时不写
    def newObject(self):
        pass


    # 设置背景阈值
    def setThreValue(self, value):
        with self.threValueLock:
            self.threValue = value