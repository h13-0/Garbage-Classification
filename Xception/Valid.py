import tensorflow as tf
from tensorflow.keras.preprocessing import image

# Helper libraries
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
from datetime import datetime 

# System API
import os

# Configure
## 源图片的总文件夹位置, 尽量保证输入图片的长宽比为1:1即可, 程序会自动缩放。
Base_Dir   = '../../datasets/garbage_finally/'
## 权重文件的位置
Weight_File_Path = "./weights/epoch-140-val_acc-0.9651-acc_top3-1.0000-acc_top5-1.0000.hdf5"

Image_Shape = (299,299)
Base_Dir = Base_Dir + "Test"

# Load Images
def LoadImages(Dir,label):
    Input_image_list = []
    Source_image_list = []
    Input_label_list = []

    for Files in os.listdir(Dir):
        if(os.path.isfile(Dir + "/" + Files)):
            print("Loading " + Dir + "/" + Files)
            cv_img = cv.imread(Dir + "/" + Files)
            cv_img = cv.resize(cv_img,(299,299))
            cv_img = cv_img.astype(np.float32) / 255.0
            img = image.img_to_array(cv.cvtColor(cv_img,cv.COLOR_BGR2RGB))
            Input_image_list.append(img)
            Input_label_list.append(label)

    return Input_image_list,Input_label_list

def plot_image(i, predictions_array, true_label, img, Class_name):
    predictions_array, true_label, img = predictions_array, true_label[i], img[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(img, cmap=plt.cm.binary)

    predicted_label = np.argmax(predictions_array)
    if predicted_label == true_label:
        color = 'blue'
    else:
        color = 'red'

    plt.xlabel("{} {:2.0f}% ({})".format(Class_name[predicted_label],
                                100*np.max(predictions_array),
                                Class_name[true_label]),
                                color=color)


def plot_value_array(i, predictions_array, true_label, classes):
    predictions_array, true_label = predictions_array, true_label[i]
    plt.grid(False)
    plt.xticks(range(classes))
    plt.yticks([])
    thisplot = plt.bar(range(classes), predictions_array, color="#777777")
    plt.ylim([0, 1])
    predicted_label = np.argmax(predictions_array)

    thisplot[predicted_label].set_color('red')
    thisplot[true_label].set_color('blue')


def main():
    image_list = []
    label_list = []
    Class_name = []

    index = 0
    for dirs in os.listdir(Base_Dir):
        if(os.path.isdir(Base_Dir + "/" + dirs)):
            temp_image_list,temp_label_list = LoadImages(Base_Dir + "/" + dirs,index)
            image_list = image_list + temp_image_list
            label_list = label_list + temp_label_list
            Class_name.append(dirs)

        index += 1

    test_images = np.array(image_list)
    test_labels = label_list

    # Build Model
    model = tf.keras.applications.Xception(include_top=True,weights=Weight_File_Path,input_shape=(299,299,3),classes=len(Class_name),pooling="avg")

    StartTime = datetime.now() 
    predictions = model.predict(np.array(test_images))
    EndTime = datetime.now()
    print(predictions)

    print(str((EndTime - StartTime).microseconds / len(test_images)) + " us per picture.")

    Correct = 0
    for i in range(len(test_images)):
        if(np.argmax(predictions[i]) == test_labels[i]):
            Correct += 1
    
    print("Accuracy: " + str(Correct / len(test_images)))

    index = 0
    while(index < len(test_images)):
        num_rows = 5
        num_cols = 4
        num_images = num_rows*num_cols
        plt.figure(figsize=(2*2*num_cols, 2*num_rows))
        for i in range(num_images):
            if(i + index < len(test_images)):
                plt.subplot(num_rows, 2*num_cols, 2*i+1)
                plot_image(i + index, predictions[i + index], test_labels, test_images, Class_name)
                plt.subplot(num_rows, 2*num_cols, 2*i+2)
                plot_value_array(i + index, predictions[i + index], test_labels, len(Class_name))
        plt.tight_layout()
        plt.show()

        index += 20


if __name__ == '__main__':
    main()