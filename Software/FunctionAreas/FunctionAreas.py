# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

# import Working mode definition
from WorkingProcess.WorkingMode import Mode

class Functions():
    def __init__(self, ui, videoPlayer, detector):
        self.__ui__ = ui
        self.__videoPlayer__ = videoPlayer
        self.__detector__ = detector

        # 当前模式
        self.__mode__ = Mode['preMode']

        # 颜色设置
        self.__enableStyleSheet__ = "background-color: #FFFFFF;"
        self.__disableStyleSheet__ = "background-color: #E4E4E4;"

        # 模式切换
        self.__ui__.PreMode.clicked.connect(lambda:self.__preMode__())
        self.__ui__.FinalMode.clicked.connect(lambda:self.__finalMode__())

        # 图像功能区
        self.__ui__.PlayVideo.clicked.connect(lambda:self.__playVideo__())
        self.__ui__.PlayPhoto.clicked.connect(lambda:self.__playPhoto__())
        self.__ui__.TakeTurns.clicked.connect(lambda:self.__takeTurns__())

        # 背景阈值Slider
        self.__ui__.threSlider.valueChanged.connect(lambda:self.__threValueChanged__())

        # 录入新垃圾
        self.__ui__.ReadyToRecord.clicked.connect(lambda:self.__readyToRecord__())
        # 重新录入
        self.__ui__.ReRecord.clicked.connect(lambda:self.__reRecord__())

        ## 新的可回收垃圾
        self.__ui__.NewRecycle.clicked.connect(lambda:self.__newRecycle__())

        ## 新的厨余垃圾
        self.__ui__.NewKitchen.clicked.connect(lambda:self.__newKitchen__())

        ## 新的有害垃圾
        self.__ui__.NewHarmful.clicked.connect(lambda:self.__newHarmful__())

        ## 新的其他垃圾
        self.__ui__.NewOther.clicked.connect(lambda:self.__newOther__())


    # 获取当前模式
    def getCurrentMode(self):
        return self.__mode__


    # 切换为初赛模式
    def __preMode__(self):
        self.__ui__.PreMode.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.FinalMode.setStyleSheet(self.__disableStyleSheet__)

        self.__ui__.PreMode.setEnabled(False)
        self.__ui__.FinalMode.setEnabled(True)

        self.__mode__ = Mode['preMode']


    # 切换为决赛模式
    def __finalMode__(self):
        self.__ui__.PreMode.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.FinalMode.setStyleSheet(self.__enableStyleSheet__)

        self.__ui__.PreMode.setEnabled(True)
        self.__ui__.FinalMode.setEnabled(False)

        self.__mode__ = Mode['finalMode']


    # 切换为视频播放模式
    def __playVideo__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__disableStyleSheet__)

        self.__ui__.PlayVideo.setEnabled(False)
        self.__ui__.PlayPhoto.setEnabled(True)
        self.__ui__.TakeTurns.setEnabled(True)

        self.__videoPlayer__.fromVideo()


    # 切换为图片播放模式
    def __playPhoto__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__disableStyleSheet__)

        self.__ui__.PlayVideo.setEnabled(True)
        self.__ui__.PlayPhoto.setEnabled(False)
        self.__ui__.TakeTurns.setEnabled(True)

        self.__videoPlayer__.fromFloder()


    # 切换为轮流播放模式
    def __takeTurns__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__enableStyleSheet__)

        self.__ui__.PlayVideo.setEnabled(True)
        self.__ui__.PlayPhoto.setEnabled(True)
        self.__ui__.TakeTurns.setEnabled(False)

        self.__videoPlayer__.takeTurns()

    
    # 设置阈值Slider当前值
    def setThreSliderValue(self, value):
        if((value >= 0) and (value <= 255)):
            self.__ui__.threSlider.setValue(value)


    # 阈值Slider触发函数
    def __threValueChanged__(self):
        self.__detector__.setThreValue(self.__ui__.threSlider.value())


    # 准备录入新垃圾
    def __readyToRecord__(self):
        self.__ui__.ReadyToRecord.setStyleSheet(self.__disableStyleSheet__)
        self.__enableAllKinds__()

    
    # 新垃圾录入完毕(或失败), 由其他对象调起
    def recordFinish(self):
        self.__ui__.ReadyToRecord.setStyleSheet(self.__enableStyleSheet__)
        self.__disableAllKinds__()

    
    # 重新录入, 删除所有模板
    def __reRecord__(self):
        pass


    # 录入新的可回收垃圾
    def __newRecycle__(self):
        self.__disableAllKinds__()


    # 录入新的厨余垃圾
    def __newKitchen__(self):
        self.__disableAllKinds__()


    # 录入新的有害垃圾
    def __newHarmful__(self):
        self.__disableAllKinds__()


    # 录入新的其他垃圾
    def __newOther__(self):
        self.__disableAllKinds__()


    # 准备录入, 使四种垃圾的按钮全部生效
    def __enableAllKinds__(self):
        self.__ui__.NewRecycle.setEnabled(True)
        self.__ui__.NewKitchen.setEnabled(True)
        self.__ui__.NewHarmful.setEnabled(True)
        self.__ui__.NewOther.setEnabled(True)

        self.__ui__.NewRecycle.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.NewKitchen.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.NewHarmful.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.NewOther.setStyleSheet(self.__enableStyleSheet__)


    # 录入完毕, 使四种垃圾的按钮全部失效
    def __disableAllKinds__(self):
        self.__ui__.NewRecycle.setEnabled(False)
        self.__ui__.NewKitchen.setEnabled(False)
        self.__ui__.NewHarmful.setEnabled(False)
        self.__ui__.NewOther.setEnabled(False)

        self.__ui__.NewRecycle.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.NewKitchen.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.NewHarmful.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.NewOther.setStyleSheet(self.__disableStyleSheet__)
