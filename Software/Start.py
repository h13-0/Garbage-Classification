# import PyQt5
from PyQt5.QtWidgets import QApplication, QMainWindow

# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import sys
import os
import platform

## WorkingProcess
### POP Methods
from WorkingProcess.TestProcess import TestProcess
#from WorkingProcess.MainProcess import MainProcess

def main():
    # 如果被部署到Linux上 首先清除系统缓存以获取更大的可用内存
    if(platform.system()=='Linux'):
        os.system("sync; echo 3 > /proc/sys/vm/drop_caches")
    
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
