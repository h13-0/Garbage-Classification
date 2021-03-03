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
        self.Qlabel = Qlabel
        self.cameraID = cameraID

        # Camera
        self.capLock = threading.Lock()
        self.cap = cv.VideoCapture(self.cameraID)
        self.cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv.CAP_PROP_FRAME_HEIGHT, 480)

        if(not self.cap.isOpened):
            raise Exception("Could not open camera: " + str(self.cameraID))

        # 创建显示线程且立即阻塞
        self.__displayFlag = threading.Event()
        self.__displayFlag.clear()

        self.displayThread = threading.Thread(target=self.display, )
        self.displayThread.setDaemon(True)
        self.displayThread.start()

        # Build Model
        self.model = tf.keras.applications.Xception(include_top=True,weights=weigthFile,input_shape=(299,299,3),classes=len(className),pooling="avg")

        # 跳过若干帧
        for i in range(10):
            self.getFrame()

        # 获取空图像直方图做原始图像直方图
        self.originalHist = cv.calcHist([self.getFrame()], [0], None, [256], [0, 255])


    # 获取单帧图像
    def getFrame(self):
        with self.capLock:
            ret, frame = self.cap.read()
            return frame


    # 需要显示的时候才调用此方法
    def show(self):
        self.__displayFlag.set()


    # 但是不释放摄像头(懒得写)
    def pause(self):
        self.__displayFlag.clear()


    def display(self):
        while True:
            self.__displayFlag.wait()
            image = self.getFrame()
            qImage = QImage(image.data, 640, 480, 640 * 3, QImage.Format_RGB888)
            self.Qlabel.setPixmap(QPixmap.fromImage(qImage))


    # 检测垃圾是否被放置
    def hasObject(self):
        tempFrame = self.getFrame()

        # 黑色底图
        leftFrame = np.zeros((299, 299, 3), dtype= np.uint8)
        rightFrame = np.zeros((299, 299, 3), dtype= np.uint8)

        # 将tempFrame左右分别拷贝到leftFrame和rightFrame的中间
        
    
    def predict(self):
        pass