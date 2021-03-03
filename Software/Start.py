# import TensorFlow


# import OpenCV
import cv2 as cv
import numpy as np

# import PyQt5
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtCore import QObject
from PyQt5.QtGui import QImage, QPixmap


# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import sys
import threading
import time

# import Private Implementation
## UI
from QtUI.ProcessBar import RoundProgress

## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

## Hardware

videoPlayer = None

def test():
    global videoPlayer
    global detector, detectorLock, className

    # 等待detector加载完毕
    with detectorLock:
        pass
    
    
    time.sleep(3)

    videoPlayer.pause()
    print("switch")

    videoPlayer.fromFloder()
    videoPlayer.start(3)

    time.sleep(10)
    print("switch")
    videoPlayer.fromVideo()
    videoPlayer.start()

    while True:
        pass


def detectorInit():
    # 初始化Detector
    global detector, detectorLock, className, ui
    with detectorLock:
        className = ['Battery','Brokenceramics','Cans','Cigarettebutts','Drug','Fruit','Paper','Tile','Vegetables','Walterbottles']
        detector = Detector(ui.frame_label, 0, "./weight/Result.hdf5", className)

    # 初始化完毕后 锁住本线程(即挂起)
    blocking = threading.Event()
    blocking.clear()
    blocking.wait()


def main():
    # 初始化UI
    global ui
    app = QApplication(sys.argv)
    Mainw = QMainWindow()
    ui = Ui_Main()
    ui.setupUi(Mainw)
    Mainw.show()

    # 创建一个新线程以初始化Detector
    global detector, detectorLock, className
    detectorLock = threading.Lock()
    detectorInitThread = threading.Thread(target=detectorInit)
    detectorInitThread.setDaemon(True)
    detectorInitThread.start()

    # 设置当前垃圾容量
    ui.processbar_harmful.parameterUpdate(30)
    ui.processbar_other.parameterUpdate(40)
    ui.processbar_recycle.parameterUpdate(50)
    ui.processbar_kitchen.parameterUpdate(50)

    # 开始播放视频
    global videoPlayer
    videoPlayer = VideoPlayer(ui.frame_label)
    videoPlayer.fromVideo()
    videoPlayer.start()

    testThread = threading.Thread(target=test)
    testThread.setDaemon(True)
    testThread.start()
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
