import os
import cv2
import dlib
import numpy as np
from PyQt5 import QtCore, QtGui, QtWidgets                    
from PyQt5.QtWidgets import (QApplication, QMainWindow, QPushButton, QWidget, QLabel, QVBoxLayout)             
###
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(525, 386)
        self.horizontalLayout = QtWidgets.QHBoxLayout(Form)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")

        self.image_label = QtWidgets.QLabel(Form)
        self.image_label.setText("")
        self.image_label.setObjectName("image_label")
        self.verticalLayout.addWidget(self.image_label)

        self.control_bt = QtWidgets.QPushButton(Form)
        self.control_bt.setObjectName("control_bt")
        self.verticalLayout.addWidget(self.control_bt)

        self.capture = QtWidgets.QPushButton(Form)
        self.capture.setObjectName("capture")
        self.verticalLayout.addWidget(self.capture)

        self.horizontalLayout.addLayout(self.verticalLayout)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form",     "Cam view"))
        self.control_bt.setText(_translate("Form", "Open Camera"))
        self.capture.setText(_translate("Form",    "Recommend"))
###
class video (QtWidgets.QDialog, Ui_Form):
    def __init__(self):
        super().__init__()                  
                     
        self.setupUi(self)                                     

        self.control_bt.clicked.connect(self.start_webcam)
        self.capture.clicked.connect(self.capture_image)
        self.capture.clicked.connect(self.startUIWindow)       

        self.image_label.setScaledContents(True)

        self.cap = None                                        

        self.timer = QtCore.QTimer(self, interval=5)
        self.timer.timeout.connect(self.update_frame)
        self._image_counter = 0

    @QtCore.pyqtSlot()
    def start_webcam(self):
        if self.cap is None:
            self.cap = cv2.VideoCapture(0)
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH,  640)
            self.cap.set(cv2.CAP_PROP_FPS,20)
            
        self.timer.start()

    @QtCore.pyqtSlot()
    def update_frame(self):
        ret, image = self.cap.read()
        #simage     = cv2.flip(image, 1)
        show = cv2.cvtColor(image,cv2.COLOR_BGR2RGB)
        gray_show=cv2.cvtColor(show,cv2.COLOR_BGR2GRAY)
        

        faces = detector(show)
        for face in faces:
            print(face)
            x1 = face.left()
            y1 = face.top()
            x2 = face.right()
            y2 = face.bottom()
            cv2.rectangle(show, (x1, y1), (x2, y2), (255, 0, 0), 2)

            landmarks = predictor(show, face)

            for n in range(0, 68):
                x = landmarks.part(n).x
                y = landmarks.part(n).y
                cv2.circle(show, (x, y), 2, (0, 0, 255), -1)
        self.displayImage(show.data, True)

    @QtCore.pyqtSlot()
    def capture_image(self):
        flag, frame = self.cap.read()
        path = r'C:\Users\User\Documents\pyqttest\capturetest'                         
        if flag:
            QtWidgets.QApplication.beep()
            name = "frame.jpg"
            cv2.imwrite(os.path.join(path, name), frame)
            self._image_counter += 1

    def displayImage(self, img, window=True):
        qformat = QtGui.QImage.Format_Indexed8
        if len(img.shape)==3 :
            if img.shape[2]==4:
                qformat = QtGui.QImage.Format_RGBA8888
            else:
                qformat = QtGui.QImage.Format_RGB888
        outImage = QtGui.QImage(img, img.shape[1], img.shape[0], img.strides[0], qformat)
        outImage = outImage.rgbSwapped()
        if window:
            self.image_label.setPixmap(QtGui.QPixmap.fromImage(outImage))

    def startUIWindow(self):
        self.Window = UIWindow()                            
        self.setWindowTitle("ROIWindow")

#        self.setCentralWidget(self.Window)
#        self.show()
### 
        self.Window.ToolsBTN.clicked.connect(self.goWindow1)

        self.hide()
        self.Window.show()

    def goWindow1(self):
        self.show()
        self.Window.hide()
        
    
               
        


            #cv2.imshow("Frame", frame)
###

###
class UIWindow(QWidget):
    def __init__(self, parent=None):
        super(UIWindow, self).__init__(parent)

        self.resize(300, 300)
        self.label = QLabel("Save Done", alignment=QtCore.Qt.AlignCenter)

        self.ToolsBTN = QPushButton('back')
#        self.ToolsBTN.move(50, 350)

        self.v_box = QVBoxLayout()
        self.v_box.addWidget(self.label)
        self.v_box.addWidget(self.ToolsBTN)
        self.setLayout(self.v_box)


if __name__=='__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = video()
    window.setWindowTitle('ROIWindow')
    window.show()
    sys.exit(app.exec_())