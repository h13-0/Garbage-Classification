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
        self.__model__ = tf.keras.applications.Xception(include_top=True,weights=weigthFile,input_shape=(299,299,3),classes=len(className),pooling="avg")

        # 获取空图像直方图做原始图像直方图
        self.__originalLeftHist__ = None
        self.__originalRightHist__ = None
        self.__originalHistLock__ = threading.Lock()


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
    def getSplitedFrame(self):
        tempFrame = self.getFrame()
        tempFrame = cv.resize(tempFrame, (398, 299))

        # 黑色底图
        leftFrame = np.zeros((299, 299, 3), dtype= np.uint8)
        rightFrame = np.zeros((299, 299, 3), dtype= np.uint8)

        # 将tempFrame左右分别拷贝到leftFrame和rightFrame的中间
        np.copyto(leftFrame[0:299, 49:248], tempFrame[0:299, 0:199])
        np.copyto(rightFrame[0:299, 49:248], tempFrame[0:299, 199:398])

        return leftFrame, rightFrame


    # 校准基准图片, 需要在检测放置之前被调用.
    # 可以重复调用(建议)
    def calibrateBasePicture(self):
        with self.__originalHistLock__:
            left, right = self.getSplitedFrame()
            self.__originalLeftHist__ = cv.calcHist([left], [0], None, [256], [0, 255])
            self.__originalRightHist__ = cv.calcHist([right], [0], None, [256], [0, 255])


    # 检测垃圾是否被放置
    def hasObject(self):
        left, right = self.getSplitedFrame()
        leftHist = cv.calcHist([left], [0], None, [256], [0, 255])
        rightHist = cv.calcHist([right], [0], None, [256], [0, 255])

        # 开始检测
        with self.__originalHistLock__:
            if(self.__originalLeftHist__ is None):
                raise Exception("Please Calibrate Base Picture before detect.")

            leftDiff = cv.compareHist(leftHist, self.__originalLeftHist__, cv.HISTCMP_BHATTACHARYYA)
            rightDiff = cv.compareHist(rightHist, self.__originalRightHist__, cv.HISTCMP_CHISQR)

        return leftDiff, rightDiff

        
    # 预测种类
    def predict(self):
        imgList = self.getSplitedFrame()
        imgs = np.array(imgList)

        predictions = self.__model__.predict(imgs)

        print(predictions)