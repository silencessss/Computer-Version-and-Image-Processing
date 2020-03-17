import sys
import os
import cv2
import dlib
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import QPalette, QBrush, QPixmap
#偵測相關套件
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
class Ui_MainWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Ui_MainWindow, self).__init__(parent)

        self.timer_camera = QtCore.QTimer()  # 初始化計时器
        self.cap = cv2.VideoCapture()
        self.CAM_NUM = 0
        self.set_ui()#UI介面
        self.slot_init()#建立連接
        self.__flag_work = 0
        self.x = 0
        self.count = 0
        self._image_counter=0

    def set_ui(self):
        self.__layout_main = QtWidgets.QVBoxLayout()  # QHBoxLayout，按照水平左到右增加物件
        self.__layout_fun_button = QtWidgets.QHBoxLayout()
        self.__layout_data_show = QtWidgets.QVBoxLayout()  # QVBoxLayout按照垂直上到下增加物件

        self.button_open_camera = QtWidgets.QPushButton(u'OpenCamera')
        self.button_close = QtWidgets.QPushButton(u'Close')
        self.button_recommend=QtWidgets.QPushButton(u'Recommend')

        #button颜色修改
        button_color = [self.button_open_camera, self.button_close,self.button_recommend]
        for i in range(3):
            button_color[i].setStyleSheet("QPushButton{color:black}"
                                           "QPushButton:hover{color:red}"
                                           "QPushButton{background-color:rgb(250,222,173)}"
                                           "QpushButton{border:2px}"
                                           "QPushButton{border_radius:10px}"
                                           "QPushButton{padding:2px 4px}")
        
        

        self.button_open_camera.setMinimumHeight(50)
        self.button_close.setMinimumHeight(50)
        self.button_recommend.setMinimumHeight(50)

        # move()方法是移动窗口在屏幕上的位置到x = 500，y = 500的位置上
        self.move(500, 500)

        # 信息显示
        self.label_show_camera = QtWidgets.QLabel()
        self.label_move = QtWidgets.QLabel()
        self.label_move.setFixedSize(100, 100)

        self.label_show_camera.setFixedSize(641, 481)
        self.label_show_camera.setAutoFillBackground(False)

        self.__layout_fun_button.addWidget(self.button_open_camera)
        self.__layout_fun_button.addWidget(self.button_close)
        self.__layout_fun_button.addWidget(self.label_move)
        self.__layout_fun_button.addWidget(self.button_recommend)

        self.__layout_main.addLayout(self.__layout_fun_button)
        self.__layout_main.addWidget(self.label_show_camera)

        self.setLayout(self.__layout_main)
        self.label_move.raise_()
        self.setWindowTitle(u'RECOMMEND')

        """
        # 背景颜色
        palette1 = QPalette()
        palette1.setBrush(self.backgroundRole(),QBrush(QPixmap('cat.jpg')))
        self.setPalette(palette1)
        """
        
    def slot_init(self):  # 建立通信連接
        self.button_open_camera.clicked.connect(self.button_open_camera_click)
        self.timer_camera.timeout.connect(self.show_camera)
        self.button_close.clicked.connect(self.close)
        #self.button_recommend.clicked.connect(self.recommended)

    def button_open_camera_click(self):
        if self.timer_camera.isActive() == False:
            flag = self.cap.open(self.CAM_NUM)
            #相機檢查
            if flag == False:
                msg = QtWidgets.QMessageBox.Warning(self, u'Warning', u'Please check your camera device',
                                                    buttons=QtWidgets.QMessageBox.Ok,
                                                    defaultButton=QtWidgets.QMessageBox.Ok)
                # if msg==QtGui.QMessageBox.Cancel:
                #                     pass
            else:
                self.timer_camera.start(30)
                self.button_open_camera.setText(u'CloseCamera')
        else:
            self.timer_camera.stop()
            self.cap.release()
            self.label_show_camera.clear()
            self.button_open_camera.setText(u'OpenCamera')

    def show_camera(self):
        
        #self.cap=cv2.VideoCapture(0)
        ret, self.image = self.cap.read()
        #show = cv2.resize(self.image, (640, 480))
        show = cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)
        gray_show=cv2.cvtColor(show,cv2.COLOR_BGR2GRAY)
        faces = detector(gray_show)
        for face in faces:
            print(face)
            x1=face.left()
            y1=face.top()
            x2=face.right()
            y2=face.bottom()
            cv2.rectangle(show,(x1,y1),(x2,y2),(0,0,255),2)
            landmarks=predictor(gray_show,face)
            for n in range(0,68):
                x=landmarks.part(n).x
                y=landmarks.part(n).y
                cv2.circle(show,(x,y),2,(255,0,0),-1)
        
        
        #須轉影像格式輸出顯示畫面
        showImage = QtGui.QImage(show.data, show.shape[1], show.shape[0], QtGui.QImage.Format_RGB888)
        self.label_show_camera.setPixmap(QtGui.QPixmap.fromImage(showImage))
        show = cv2.cvtColor(show, cv2.COLOR_RGB2BGR)
        path=r'C:\Users\User\Documents\pyqttest\capturetest'
        name="frame.jpg"
        cv2.imwrite(os.path.join(path,name),show);
        
        
        img_out = show[landmarks.part(1).y:landmarks.part(4).y,landmarks.part(4).x:landmarks.part(48).x-5]
        path=r'C:\Users\User\Documents\pyqttest\capturetest'
        name="roi_left.jpg"
        cv2.imwrite(os.path.join(path,name),img_out)
        img_out = show[landmarks.part(15).y:landmarks.part(12).y,landmarks.part(54).x+5:landmarks.part(12).x]
        path=r'C:\Users\User\Documents\pyqttest\capturetest'
        name="roi_right.jpg"
        cv2.imwrite(os.path.join(path,name),img_out)
        img_out = show[landmarks.part(19).y-45:landmarks.part(19).y,landmarks.part(17).x:landmarks.part(26).x]
        path=r'C:\Users\User\Documents\pyqttest\capturetest'
        name="roi_up.jpg"
        cv2.imwrite(os.path.join(path,name),img_out)
        img_out = show[landmarks.part(57).y:landmarks.part(8).y,landmarks.part(6).x:landmarks.part(10).x]
        path=r'C:\Users\User\Documents\pyqttest\capturetest'
        name="roi_down.jpg"
        cv2.imwrite(os.path.join(path,name),img_out)
        
       
            
        
        
        
   
    def closeEvent(self, event):
        ok = QtWidgets.QPushButton()
        cancel = QtWidgets.QPushButton()
        msg = QtWidgets.QMessageBox(QtWidgets.QMessageBox.Warning, u'Close', u'Close?')
        msg.addButton(ok, QtWidgets.QMessageBox.ActionRole)
        msg.addButton(cancel, QtWidgets.QMessageBox.RejectRole)
        ok.setText(u'Sure')
        cancel.setText(u'Concele')
        if msg.exec_() == QtWidgets.QMessageBox.RejectRole:
            event.ignore()
        else:
            if self.cap.isOpened():
                self.cap.release()
            if self.timer_camera.isActive():
                self.timer_camera.stop()
            event.accept()
    
    #def recommended(self,img):
        
      

if __name__ == '__main__':
    App = QApplication(sys.argv)
    win = Ui_MainWindow()
    win.show()
    sys.exit(App.exec_())
