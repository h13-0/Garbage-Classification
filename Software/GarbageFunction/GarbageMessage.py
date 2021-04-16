# 主要负责管理UI左侧的垃圾桶信息及对应按钮
# import PyQt5
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget


# import Helper libs
import threading
import time

class GarbageMessage(QWidget):
    __recycleNumberSignal__ = pyqtSignal([int])
    __kitchenNumberSignal__ = pyqtSignal([int])
    __harmfulNumberSignal__ = pyqtSignal([int])
    __otherNumberSignal__ = pyqtSignal([int])

    def __init__(self, ui):
        QWidget.__init__(self)
        self.__ui__ = ui
        self.__slave__ = None
        self.__slaveLock__ = threading.Lock()

        # 显示动画
        self.__ui__.processbar_other.parameterUpdate(100)
        self.__ui__.processbar_other.parameterUpdate(0)

        self.__ui__.processbar_harmful.parameterUpdate(100)
        self.__ui__.processbar_harmful.parameterUpdate(0)

        self.__ui__.processbar_recycle.parameterUpdate(100)
        self.__ui__.processbar_recycle.parameterUpdate(0)

        self.__ui__.processbar_kitchen.parameterUpdate(100)
        self.__ui__.processbar_kitchen.parameterUpdate(0)


        # 连接信号
        self.__recycleNumberSignal__.connect(self.__ui__.setRecycleNumber)
        self.__kitchenNumberSignal__.connect(self.__ui__.setKitchenNumber)
        self.__harmfulNumberSignal__.connect(self.__ui__.setHarmfulNumber)
        self.__otherNumberSignal__.connect(self.__ui__.setOtherNumber)

        self.__ui__.LoadTest.clicked.connect(lambda:self.__loadFlash__())
        self.__ui__.ClearNumbers.clicked.connect(lambda:self.clear())

        # 垃圾数量统计
        self.__recycleNumber__ = 0
        self.__kitchenNumber__ = 0
        self.__harmfulNumber__ = 0
        self.__otherNumber__ = 0

        # 归零
        self.clear()

        # UI刷新线程
        self.__loadFlashThread__ = threading.Thread(target=self.__loadFlash__)
        self.__loadFlashThread__.setDaemon(True)
        self.__loadFlashThread__.start()


    # 清零计数
    def clear(self):
        self.__recycleNumber__ = 0
        self.__kitchenNumber__ = 0
        self.__harmfulNumber__ = 0
        self.__otherNumber__ = 0

        self.__recycleNumberSignal__.emit(0)
        self.__kitchenNumberSignal__.emit(0)
        self.__harmfulNumberSignal__.emit(0)
        self.__otherNumberSignal__.emit(0)


    def sum(self):
        return self.__recycleNumber__ + self.__kitchenNumber__ + self.__harmfulNumber__ + self.__otherNumber__


    # 增加垃圾数量
    def newRecycle(self):
        self.__recycleNumber__ += 1
        self.__recycleNumberSignal__.emit(self.__recycleNumber__)
        return self.sum(), self.__recycleNumber__

    
    def newKitchen(self):
        self.__kitchenNumber__ += 1
        self.__kitchenNumberSignal__.emit(self.__kitchenNumber__)
        return self.sum(), self.__kitchenNumber__

    
    def newHarmful(self):
        self.__harmfulNumber__ += 1
        self.__harmfulNumberSignal__.emit(self.__harmfulNumber__)
        return self.sum(), self.__harmfulNumber__

    
    def newOther(self):
        self.__otherNumber__ += 1
        self.__otherNumberSignal__.emit(self.__otherNumber__)
        return self.sum(), self.__otherNumber__
    

    # 设置slave
    def setSlave(self, slave):
        with self.__slaveLock__:
            self.__slave__ = slave

    def __loadFlash__(self):
        while True:
            time.sleep(10)
            with self.__slaveLock__:
                if not self.__slave__ is None:
                    rec, kit, har, oth = self.__slave__.getLoadTest()

                    self.__ui__.processbar_recycle.parameterUpdate(rec)
                    self.__ui__.processbar_kitchen.parameterUpdate(kit)
                    self.__ui__.processbar_harmful.parameterUpdate(har)
                    self.__ui__.processbar_other.parameterUpdate(oth)


