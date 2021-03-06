# 主要负责管理UI左侧的垃圾桶信息及对应按钮
# import PyQt5
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget


class GarbageMessage(QWidget):
    __recycleNumberSignal__ = pyqtSignal([int])
    __kitchenNumberSignal__ = pyqtSignal([int])
    __harmfulNumberSignal__ = pyqtSignal([int])
    __otherNumberSignal__ = pyqtSignal([int])

    def __init__(self, UI):
        QWidget.__init__(self)
        self.ui = UI

        # 显示动画
        self.ui.processbar_recycle.parameterUpdate(100)
        self.ui.processbar_recycle.parameterUpdate(0)

        self.ui.processbar_kitchen.parameterUpdate(100)
        self.ui.processbar_kitchen.parameterUpdate(0)

        self.ui.processbar_harmful.parameterUpdate(100)
        self.ui.processbar_harmful.parameterUpdate(0)

        self.ui.processbar_other.parameterUpdate(100)
        self.ui.processbar_other.parameterUpdate(0)

        # 连接信号
        self.__recycleNumberSignal__.connect(self.ui.setRecycleNumber)
        self.__kitchenNumberSignal__.connect(self.ui.setKitchenNumber)
        self.__harmfulNumberSignal__.connect(self.ui.setHarmfulNumber)
        self.__otherNumberSignal__.connect(self.ui.setOtherNumber)

        self.ui.LoadTest.clicked.connect(lambda:self.loadTest())
        self.ui.ClearNumbers.clicked.connect(lambda:self.clear())

        # 归零
        self.clear()


    def clear(self):
        print("clear")
        self.__recycleNumberSignal__.emit(0)
        self.__kitchenNumberSignal__.emit(0)
        self.__harmfulNumberSignal__.emit(0)
        self.__otherNumberSignal__.emit(0)


    def loadTest(self):
        pass


