# import Tensorflow
import tensorflow as tf
from tensorflow.keras.preprocessing import image

# import PyQt5
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt

# Helper libraries
import cv2 as cv
import numpy as np
import threading, signal
import os
import time

class Detector(threading.Thread):
    def __init__(self, Qlabel, cameraID, weigthFile, className):
        self.__qlabel__ = Qlabel
        self.cameraID = cameraID

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

        
    # 预测种类
    def predict(self):
        imgList = self.getSplitedFrame()
        imgs = np.array(imgList)

        predictions = self.__model__.predict(imgs)

        print(predictions)

    
    # 录入新的物体(模板匹配)
    def newObject(self):
        pass