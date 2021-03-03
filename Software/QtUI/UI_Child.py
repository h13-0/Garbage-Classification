from QtUI import UI
from QtUI.ProcessBar import RoundProgress


class Ui_Main(UI.Ui_MainWindow):

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
        super().__init__()
        self.processbar_recycle = RoundProgress()
        self.processbar_harmful = RoundProgress()
        self.processbar_other = RoundProgress()
        self.processbar_kitchen  = RoundProgress()
