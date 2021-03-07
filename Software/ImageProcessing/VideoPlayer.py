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

imgType_list = {'.jpg','.bmp','.png','.jpeg'}

class VideoPlayer(threading.Thread):
    def __init__(self,Qlabel):
        threading.Thread.__init__(self)
        super(VideoPlayer, self).__init__()
        self.__qlabel__ = Qlabel

        # 创建播放线程 但立即阻塞
        self.__playerFlag__ = threading.Event()
        self.__playerFlag__.clear()    # 立即阻塞

        self.thread = threading.Thread(target=self.__player__)
        self.thread.setDaemon(True)
        self.thread.start()

        # status:
        ## notWorking: 未在播放, Qlabel已经被释放
        ## working: 正在工作, Qlabel正在被占用
        self.status = "notWorking"

        # mode:
        ## picture: 循环播放图片
        ## video: 循环播放视频
        ## takeTurns: 循环播放图片和视频
        self.mode = None

        self.__cap__ = None


    # 循环播放视频
    def fromVideo(self, file = "./video.mp4"):
        if(self.status == "working"):
            self.pause()
            time.sleep(0.1)
            self.mode = 'video'
            self.__file__ = file
            self.resume()

        else:
            self.pause()
            time.sleep(0.1)
            self.mode = 'video'
            self.__file__ = file
        

    # 循环播放文件夹下所有图片
    def fromFloder(self, floder = "./imgs"):
        if(self.status == "working"):
            self.pause()
            time.sleep(0.1)
            self.mode = 'picture'
            
            self.photoList = []
            self.numberOfPictures = 0

            # Read images from floders
            ## ToDo: Rename files
            for files in os.listdir(floder):
                for imgtype in imgType_list:
                    if(files.endswith(imgtype) and os.path.isfile(os.path.join(floder, files))):
                        self.photoList.append(os.path.join(floder, files))
                        self.numberOfPictures += 1
                        break
            
            self.resume()

        else:
            self.mode = 'picture'
            self.photoList = []
            self.numberOfPictures = 0

            # Read images from floders
            ## ToDo: Rename files
            for files in os.listdir(floder):
                for imgtype in imgType_list:
                    if(files.endswith(imgtype) and os.path.isfile(os.path.join(floder, files))):
                        self.photoList.append(os.path.join(floder, files))
                        self.numberOfPictures += 1
                        break
    

    # 视频图片轮流播放
    def takeTurns(self, file = "./video.mp4", floder = "./imgs"):
        if(self.status == "working"):
            self.pause()
            time.sleep(0.1)
            self.mode = "takeTurns"
            self.__file__ = file

            self.photoList = []
            self.numberOfPictures = 0

            # Read images from floders
            ## ToDo: Rename files
            for files in os.listdir(floder):
                for imgtype in imgType_list:
                    if(files.endswith(imgtype) and os.path.isfile(os.path.join(floder, files))):
                        self.photoList.append(os.path.join(floder, files))
                        self.numberOfPictures += 1
                        break

            self.resume()
            
        else:
            self.mode = "takeTurns"
            self.__file__ = file
            self.photoList = []
            self.numberOfPictures = 0

            # Read images from floders
            ## ToDo: Rename files
            for files in os.listdir(floder):
                for imgtype in imgType_list:
                    if(files.endswith(imgtype) and os.path.isfile(os.path.join(floder, files))):
                        self.photoList.append(os.path.join(floder, files))
                        self.numberOfPictures += 1
                        break


    # 从cv2的numpy转换到QPixmap的内部实现
    ## 自动缩放, 不拉伸
    def __loadImage__(self,image):
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


    # 播放器的内部实现
    def __player__(self):
        self.__playerFlag__.wait()
        while True:
            if(self.mode == 'picture'):
                # Play from images
                if(self.numberOfPictures < 1):
                    raise Exception("No Image(s) in this floder.")

                for img in self.photoList:
                    image = cv.imread(img)
                    if(image is None):
                        continue

                    self.__playerFlag__.wait()

                    self.__qlabel__.setPixmap(self.__loadImage__(image))
                    if(not self.mode == 'picture'):
                        break

                    time.sleep(self.__delay__)
                

            elif(self.mode == 'video'):
                # Play from Video
                if(self.__cap__ is None):
                    self.__cap__ = cv.VideoCapture(self.__file__)
                    if(not self.__cap__.isOpened()):
                        raise Exception("Could not open VideoCapture: " + str(self.__file__))

                ret, frame = self.__cap__.read()

                if(ret):
                    self.__playerFlag__.wait()

                    self.__qlabel__.setPixmap(self.__loadImage__(frame))
                else:
                    break

                time.sleep(1 / self.__cap__.get(cv.CAP_PROP_FPS))

            elif(self.mode == 'takeTurns'):
                # TakeTurns
                if(self.__cap__ is None):
                    self.__cap__ = cv.VideoCapture(self.__file__)
                    if(not self.__cap__.isOpened()):
                        raise Exception("Could not open VideoCapture: " + str(self.__file__))

                ret = True
                ret, frame = self.__cap__.read()

                if(ret):
                    self.__playerFlag__.wait()

                    self.__qlabel__.setPixmap(self.__loadImage__(frame))
                    
                    time.sleep(1 / self.__cap__.get(cv.CAP_PROP_FPS))
                else:
                    if(self.numberOfPictures < 1):
                        raise Exception("No Image(s) in this floder.")

                    for img in self.photoList:
                        image = cv.imread(img)
                        if(image is None):
                            continue

                        self.__playerFlag__.wait()

                        self.__qlabel__.setPixmap(self.__loadImage__(image))

                        if(not self.mode == 'takeTurns'):
                            break

                        time.sleep(self.__delay__)
                    
                    self.__cap__ = cv.VideoCapture(self.__file__)
                    if(not self.__cap__.isOpened()):
                        raise Exception("Could not open VideoCapture: " + str(self.__file__))


    # 开始播放
    def start(self, delay=1.5):
        self.__delay__ = delay
        self.__playerFlag__.set()
        self.status = "working"


    # 暂停播放
    def pause(self):
        if(self.status == "working"):
            self.__playerFlag__.clear()   # 设置为False, 让线程阻塞
            self.status = "notWorking"


    # 继续播放
    def resume(self):
        if(self.status == "notWorking"):
            self.__playerFlag__.set()
            self.status = "working"


    # 销毁对象
    ## 懒得测试 应该没有内存泄漏
    def __del__(self):
        self.__playerFlag__.clear()
        time.sleep(0.1)
        if(not self.__cap__ == None):
            self.__cap__.release()
        