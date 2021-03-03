import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import datasets, layers, models
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications.imagenet_utils import preprocess_input

# Helper libraries
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import PIL

# System API
import os

# Configure
# 以下内容请和 Valid.py 中的匹配
## 源图片的总文件夹位置
Base_Dir = "E:/TensorFlow/datasets/garbage_finally/"
train_dir = Base_Dir + "Train"
validation_dir = Base_Dir + "Valid"
log_dir = "./log"
## 导入模型的图片大小, 没必要修改, 但是尽量和使用时设置的一致。
Image_Shape = (299,299)
batch_size = 4
classes = 10
epochs  = 25
save_dir='./weights'
filepath="epoch-{epoch:02d}-val_acc-{val_accuracy:.4f}-acc_top3-{acc_top3:.4f}-acc_top5-{acc_top5:.4f}.hdf5"
learning_rate = 0.0005 #0.0005

physical_devices = tf.config.experimental.list_physical_devices('GPU')
for physical_device in physical_devices:
    tf.config.experimental.set_memory_growth(physical_device, True)

# 定义训练集图像生成器，并进行图像增强
train_image_generator = ImageDataGenerator( rescale=1./255,  # 归一化
                                            rotation_range=40, #旋转范围
                                            width_shift_range=0.2, #水平平移范围
                                            height_shift_range=0.2, #垂直平移范围
                                            shear_range=0.2, #剪切变换的程度
                                            zoom_range=0.2, #缩放范围
                                            horizontal_flip=True,  #水平翻转
                                            fill_mode='nearest')  

# 使用图像生成器从文件夹train_dir中读取样本，对标签进行one-hot编码
train_data_gen = train_image_generator.flow_from_directory(directory=train_dir,
                                                           batch_size=batch_size,
                                                           shuffle=True,   #打乱数据
                                                           target_size=Image_Shape,
                                                           class_mode='categorical')
# 训练集样本数    
total_train = train_data_gen.n

# 定义验证集图像生成器，并对图像进行预处理
validation_image_generator = ImageDataGenerator(rescale=1./255)  #归一化

# 使用图像生成器从验证集validation_dir中读取样本
val_data_gen = validation_image_generator.flow_from_directory(directory=validation_dir,
                                                              batch_size=batch_size,
                                                              shuffle=False,  #不打乱数据
                                                              target_size=Image_Shape,
                                                              class_mode='categorical')
# 验证集样本数    
total_val = val_data_gen.n

# Build Model
base_model = tf.keras.applications.Xception(weights="imagenet",include_top=False,input_shape=(299, 299, 3),pooling="avg")
outputs = layers.Dense(classes, activation='softmax')(base_model.output)
model = keras.Model(base_model.inputs, outputs)

# Callbacks
## Checkpoint
checkpoint = keras.callbacks.ModelCheckpoint(os.path.join(save_dir, filepath),
	monitor='val_acc',verbose=1, 
	save_best_only=False)

## Tensorboard Callback
tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir=log_dir, histogram_freq=1)

# acc
def acc_top3(y_true, y_pred):
    return keras.metrics.top_k_categorical_accuracy(y_true, y_pred, k=3)
  
def acc_top5(y_true, y_pred):
    return keras.metrics.top_k_categorical_accuracy(y_true, y_pred, k=5)

# compile
model.compile(optimizer=tf.keras.optimizers.Adam(lr=learning_rate),
              loss='categorical_crossentropy',
              metrics=['accuracy', acc_top3, acc_top5])

# 模型训练
model.fit(x=train_data_gen,
                    steps_per_epoch=total_train // batch_size,
                    epochs=epochs,
                    shuffle=True,
                    validation_data=val_data_gen,
                    validation_steps=total_val // batch_size,
                    callbacks=[checkpoint,tensorboard_callback])
