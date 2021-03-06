# import PyQt5
from PyQt5.QtWidgets import QApplication, QMainWindow

# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import sys
import os
import platform

## WorkingProcess
from WorkingProcess.MainProcess import Process

def main():
    # 如果被部署到Linux上 首先清除系统缓存以获取更大的可用内存
    if(platform.system()=='Linux'):
        os.system("sync; echo 3 > /proc/sys/vm/drop_caches")
    
    # 初始化UI
    app = QApplication(sys.argv)
    
    Mainw = QMainWindow()
    ui = Ui_Main()
    ui.setupUi(Mainw)
    Mainw.show()

    # process
    process = Process(ui)
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
