# import PyQt5
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtCore import QObject
from PyQt5.QtGui import QImage, QPixmap

# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import sys

## Image Processing
from ImageProcessing.VideoPlayer import VideoPlayer
from ImageProcessing.Detector import Detector

## WorkingProcess
### POP Methods
from WorkingProcess.TestProcess import TestProcess

def main():
    # 初始化UI
    global ui
    app = QApplication(sys.argv)
    Mainw = QMainWindow()
    ui = Ui_Main()
    ui.setupUi(Mainw)
    Mainw.show()

    # Test
    TestProcess(ui)
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
