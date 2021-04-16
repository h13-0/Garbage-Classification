from QtUI import UI
from QtUI.ProcessBar import RoundProgress
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QTextCursor


class Ui_Main(UI.Ui_MainWindow, QWidget):
    def setupUi(self, MainWindow):
        super().setupUi(MainWindow)

        self.stackedWidget_Other.addWidget(self.processbar_other)
        self.stackedWidget_Harmful.addWidget(self.processbar_harmful)
        self.stackedWidget_Recycle.addWidget(self.processbar_recycle)
        self.stackedWidget_Kitchen.addWidget(self.processbar_kitchen)
        self.processbar_kitchen.show()
        self.processbar_other.show()
        self.processbar_recycle.show()
        self.processbar_harmful.show()


    def __init__(self):
        UI.Ui_MainWindow.__init__(self)
        QWidget.__init__(self)

        self.processbar_recycle = RoundProgress()
        self.processbar_harmful = RoundProgress()
        self.processbar_other = RoundProgress()
        self.processbar_kitchen  = RoundProgress()
    

    def outputResult(self, result):
        self.textEdit.setText(result)
        self.textEdit.moveCursor(QTextCursor.End)


    def setRecycleNumber(self, number):
        self.RecycleNumbers.setText("当前件数: " + str(number))

    
    def setKitchenNumber(self, number):
        self.KitchenNumbers.setText("当前件数: " + str(number))

    
    def setHarmfulNumber(self, number):
        self.HarmfulNumbers.setText("当前件数: " + str(number))


    def setOtherNumber(self, number):
        self.OtherNumbers.setText("当前件数: " + str(number))
