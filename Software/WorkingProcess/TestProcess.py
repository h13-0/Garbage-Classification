# import UI
from QtUI.UI_Child import Ui_Main
from PyQt5.QtCore import pyqtSignal

# import Helper libs
import threading
import time

# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

## GarbageFunction
from GarbageFunction.GarbageMessage import GarbageMessage


## Hardware

def test():
    global videoPlayer
    global detector, detectorLock, className

    # 等待detector加载完毕
    with detectorLock:
        pass
    
    # Switch Test
    '''
    while True:
        
        time.sleep(10)

        print("Switch To Images Player")

        videoPlayer.fromFloder()
        videoPlayer.start(3)

        time.sleep(10)

        print("Switch To Detector")

        videoPlayer.pause()
        detector.show()

        time.sleep(10)

        print("Switch To Video Player")

        detector.pause()
        videoPlayer.fromVideo()
        '''

    
    # hasObjectTest
    '''
    videoPlayer.pause()
    detector.show()
    
    while True:
        print(detector.hasObject())
        time.sleep(0.1)
        '''
    '''
    # predict
    while True:
        #detector
        time.sleep(0.1)
        '''

    # Qt Linux Child thread Test
    videoPlayer.pause()
    detector.show()
    

    while True:
        detector.predict()

    # end
    while True:
        pass
        

def outputResult(str):
    global ui
    ui.setText(str)


def detectorInit():
    # 初始化Detector
    global detector, detectorLock, className, ui
    with detectorLock:
        className = [ ['电池', '有害垃圾'] , ['碎瓷片', '其他垃圾'] , ['易拉罐', '可回收垃圾'] , ['烟蒂', '其他垃圾'] , ['药物', '有害垃圾'] ,
                ['水果', '厨余垃圾'] , ['纸张', '可回收垃圾'], ['碎砖块', '其他垃圾'] , ['蔬菜', '厨余垃圾'] , ['水瓶', '可回收垃圾'] ]
        detector = Detector(ui, 0, "./weight/Result.hdf5", className)

    # 初始化完毕后 锁住本线程(即挂起)
    blocking = threading.Event()
    blocking.clear()
    blocking.wait()


def TestProcess(UI):
    global ui
    ui = UI
    
    # 创建一个新线程以初始化Detector
    global detector, detectorLock, className
    detectorLock = threading.Lock()
    detectorInitThread = threading.Thread(target=detectorInit)
    detectorInitThread.setDaemon(True)
    detectorInitThread.start()
    

    # 初始化垃圾桶信息区域
    garbageMessage = GarbageMessage(ui)
    

    # 开始播放视频
    global videoPlayer
    videoPlayer = VideoPlayer(ui.frame_label)
    videoPlayer.fromVideo()
    videoPlayer.start()

    testThread = threading.Thread(target=test)
    testThread.setDaemon(True)
    testThread.start()