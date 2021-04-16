# import PyQt5
from PyQt5.QtWidgets import QApplication, QMainWindow

# import UI
from QtUI.UI_Child import Ui_Main

# import Helper libs
import sys
import os
import platform
import argparse

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

    # argparse
    parser = argparse.ArgumentParser()
    if(platform.system()=='Windows'):
        parser.add_argument('--serial', type=str, default="COM5", help="Serial port")
    elif(platform.system()=='Linux'):
        parser.add_argument('--serial', type=str, default="/dev/ttyTHS1", help="Serial port")

    parser.add_argument('--cam', type=int, default=0, help="Camrea ID")
    
    parser.add_argument('--thre', type=int, default=170, help="threshold Value")

    parser.add_argument('--minArea', type=int, default=20, help="Minimum Area Of Object")

    parser.add_argument('--retry', type=int, default=5, help="Maximum number of attempts")

    args = parser.parse_args()


    # process
    process = Process(ui, args.cam, args.serial, args.thre, args.minArea, args.retry)
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
