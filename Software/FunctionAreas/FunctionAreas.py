# import Private Implementation
## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

class Functions():
    def __init__(self, ui, videoPlayer, detector):
        self.ui = ui
        self.videoPlayer = videoPlayer
        self.detector = detector

        # 颜色设置
        self.enableStyleSheet = "background-color: #FFFFFF;"
        self.disableStyleSheet = "background-color: #E4E4E4;"

        # 模式切换
        self.ui.PreMode.clicked.connect(lambda:self.preMode())
        self.ui.FinalMode.clicked.connect(lambda:self.finalMode())

        # 图像功能区
        self.ui.PlayVideo.clicked.connect(lambda:self.playVideo())
        self.ui.PlayPhoto.clicked.connect(lambda:self.playPhoto())
        self.ui.TakeTurns.clicked.connect(lambda:self.takeTurns())

        # 录入新垃圾


    # 切换为初赛模式
    def preMode(self):
        self.ui.PreMode.setStyleSheet(self.enableStyleSheet)
        self.ui.FinalMode.setStyleSheet(self.disableStyleSheet)
        self.detector.switchMode("PreMode")


    # 切换为决赛模式
    def finalMode(self):
        self.ui.PreMode.setStyleSheet(self.disableStyleSheet)
        self.ui.FinalMode.setStyleSheet(self.enableStyleSheet)
        self.detector.switchMode("FinalMode")


    # 切换为视频播放模式
    def playVideo(self):
        self.ui.PlayVideo.setStyleSheet(self.enableStyleSheet)
        self.ui.PlayPhoto.setStyleSheet(self.disableStyleSheet)
        self.ui.TakeTurns.setStyleSheet(self.disableStyleSheet)
        self.videoPlayer.fromVideo()


    # 切换为图片播放模式
    def playPhoto(self):
        self.ui.PlayVideo.setStyleSheet(self.disableStyleSheet)
        self.ui.PlayPhoto.setStyleSheet(self.enableStyleSheet)
        self.ui.TakeTurns.setStyleSheet(self.disableStyleSheet)
        self.videoPlayer.fromFloder()


    # 切换为轮流播放模式
    def takeTurns(self):
        self.ui.PlayVideo.setStyleSheet(self.disableStyleSheet)
        self.ui.PlayPhoto.setStyleSheet(self.disableStyleSheet)
        self.ui.TakeTurns.setStyleSheet(self.enableStyleSheet)
        self.videoPlayer.takeTurns()

