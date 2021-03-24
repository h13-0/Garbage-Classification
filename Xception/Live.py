# import TensorFlow
import tensorflow as tf
from tensorflow.keras.preprocessing import image

# Helper libraries
import cv2 as cv
import numpy as np
import threading, signal
import os
import time

# Configure
Image_Shape = (299,299)
## 权重文件的位置
Weight_File_Path = "./Result977/Result.hdf5"
Class_name = ['Battery','Brokenceramics','Cans','Cigarettebutts','Fruit','Paper','Tile','Vegetables','Walterbottles']

frame = np.zeros((1,1), np.uint8)
framelock = threading.Lock()

name = None
namelock = threading.Lock()

probability = None
probabilitylock = threading.Lock()

def GetFrame():
    global frame
    cap = cv.VideoCapture(0)
    if(not cap.isOpened()):
        raise Exception("Open Capture error!")

    # Build Model
    model = tf.keras.applications.Xception(include_top=True,weights=Weight_File_Path,input_shape=(299,299,3),classes=len(Class_name),pooling="avg")
    

    while(True):
        ret, getframe = cap.read()
        shape = getframe.shape
        size = min(shape[0],shape[1])
        getframe = getframe[int((shape[0]-size)/2):int((shape[0]-size)/2) + size, int((shape[1]-size)/2):int((shape[1]-size)/2) + size]

        with framelock:
            frame = getframe.copy()
               
        
def show():
    global frame
    while(True):
        with framelock:
            frame_show = frame.copy()

        with namelock:
            if(not name is None):
                frame_show = cv.putText(frame_show,name,(0, 18), cv.FONT_HERSHEY_SIMPLEX, 0.75, (255, 0, 0), 2)

        with probabilitylock:
            if(not probability is None):
                frame_show = cv.putText(frame_show,str(probability) + "%",(0, 32), cv.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

        cv.imshow("Live",frame_show)
        cv.waitKey(1)


def main():
    global frame, name, probability

    th_get = threading.Thread(target=GetFrame,)
    th_get.setDaemon(True)
    th_get.start()

    th_show = threading.Thread(target=show,)
    th_show.setDaemon(True)
    th_show.start()

    # Build Model
    model = tf.keras.applications.Xception(include_top=True,weights=Weight_File_Path,input_shape=(299,299,3),classes=len(Class_name),pooling="avg")

    while(True):
        with framelock:
            if(frame.shape == (480, 480, 3)):
                break
            else:
                time.sleep(1)

    while(True):
        frame_main = None
        with framelock:
            frame_main = frame.copy()

        frame_main = cv.resize(frame_main,Image_Shape)
        frame_main = frame_main.astype(np.float32) / 255.0

        templist = []
        templist.append(frame_main)
        img = np.array(templist)

        prediction = model.predict(img)

        with namelock:
            name = Class_name[np.argmax(prediction)]
            #print(name)

        with probabilitylock:
            probability = 100*np.max(prediction)
            #print(probability)

        

        


if(__name__ == '__main__'):
    main()