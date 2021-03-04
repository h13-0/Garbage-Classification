# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import threading
import time

# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

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
    
    # end
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


def TestProcess(UI):
    global ui
    ui = UI
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