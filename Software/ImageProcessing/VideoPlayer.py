# import PyQt5
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt

# import cv2
import cv2 as cv
import numpy as np

# import helper libs
import threading
import time
import os

imgType_list = {'jpg','bmp','png','jpeg'}

class VideoPlayer(threading.Thread):
    def __init__(self,Qlabel):
        super(VideoPlayer, self).__init__()
        self.__flag = threading.Event()
        self.__flag.set()    # 设置为True

        self.Qlabel = Qlabel

        # status:
        ## notWorking: 未在播放, Qlabel已经被释放
        ## working: 正在工作, Qlabel正在被占用
        self.status = "notWorking"

        self.cap = None


    def fromVideo(self,file = "./video.mp4"):
        self.mode = 'video'
        self.file = file
        

    def fromFloder(self,floder = "./imgs"):
        self.stop()
        self.mode = 'picture'
        self.photoList = []
        self.numberOfPictures = 0

        # Read images from floders
        for files in os.listdir(floder):
            for imgtype in imgType_list:
                if(files.endswith(imgtype) and os.path.isfile(files)):
                    self.photoList += os.path.join(floder + files)
                    numberOfPictures += 1
                    break
        
        # 
    

    def loadImage(self,image):
        image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
        width = image.shape[1]
        height = image.shape[0]
        q_image = QImage(image.data, width, height, width * 3, QImage.Format_RGB888)
        if width / 640 >= height / 480:
            ratio = width / 640
        else:
            ratio = height / 480
        new_width = width / ratio
        new_height = height / ratio
        new_img = q_image.scaled(int(new_width), int(new_height), Qt.KeepAspectRatio)
        return QPixmap.fromImage(new_img)


    def player(self):
        if(self.mode == 'picture'):
            # Play from images
            while True:
                pass


        else:
            while True:
                # Play from Video
                self.__flag.wait()
                self.cap = cv.VideoCapture(self.file)
                if(not self.cap.isOpened()):
                    raise Exception("Could not open VideoCapture: " + str(self.file))
            
                while True:
                    ret, frame = self.cap.read()
                    if(ret):
                        self.Qlabel.setPixmap(self.loadImage(frame))
                        time.sleep(1 / self.cap.get(cv.CAP_PROP_FPS))
                    else:
                        break
                    self.__flag.wait()


    def start(self, delay=1):
        self.thread = threading.Thread(target=self.player)
        self.thread.setDaemon(True)
        self.thread.start()
        self.status = "working"


    def pause(self):
        if(self.status == "working"):
            self.__flag.clear()   # 设置为False, 让线程阻塞
            self.status = "notWorking"


    def resume(self):
        if(self.status == "notWorking"):
            self.__flag.set()
            self.status = "working"


    # 更换播放源时使用
    def destroy(self):
        self.__flag.clear()
        time.sleep(0.1)
        if(not self.cap == None):
            self.cap.release()