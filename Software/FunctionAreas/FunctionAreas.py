# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

class Functions():
    def __init__(self, ui, videoPlayer, detector):
        self.__ui__ = ui
        self.__videoPlayer__ = videoPlayer
        self.__detector__ = detector

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

        # 录入新垃圾


    # 切换为初赛模式
    def __preMode__(self):
        self.__ui__.PreMode.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.FinalMode.setStyleSheet(self.__disableStyleSheet__)
        self.__detector__.switchMode("PreMode")


    # 切换为决赛模式
    def __finalMode__(self):
        self.__ui__.PreMode.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.FinalMode.setStyleSheet(self.__enableStyleSheet__)
        self.__detector__.switchMode("FinalMode")


    # 切换为视频播放模式
    def __playVideo__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__disableStyleSheet__)
        self.__videoPlayer__.fromVideo()


    # 切换为图片播放模式
    def __playPhoto__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__enableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__disableStyleSheet__)
        self.__videoPlayer__.fromFloder()


    # 切换为轮流播放模式
    def __takeTurns__(self):
        self.__ui__.PlayVideo.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.PlayPhoto.setStyleSheet(self.__disableStyleSheet__)
        self.__ui__.TakeTurns.setStyleSheet(self.__enableStyleSheet__)
        self.__videoPlayer__.takeTurns()

    
    # 准备录入新垃圾
    def __readyToRecord__(self):
        self.__ui__.ReadyToRecord.setStyleSheet(self.__disableStyleSheet__)

    
    # 新垃圾录入完毕(或失败), 由其他对象调起
    def recordFinish(self):
        self.__ui__.ReadyToRecord.setStyleSheet(self.__enableStyleSheet__)


    # 录入新的可回收垃圾
    def __newRecycle__(self):
        pass


    # 录入新的厨余垃圾
    def __newKitchen__(self):
        pass


    # 录入新的有害垃圾
    def __newHarmful__(self):
        pass


    # 录入新的其他垃圾
    def __newOther__(self):
        pass


