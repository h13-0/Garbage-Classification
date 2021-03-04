# 一个基于Xception图像识别的垃圾分类项目

## 图像识别部分
### Xception 图像分类实现
#### 模型训练

#### 模型校验

#### 实时监测
```
cd Xception
python ./Live.py
```
至此, Xception图像分类部分完毕。但是仍然无法进行实际的图像识别流程, 原因如下文 `Xception具体操作流程`。

### Xception具体操作流程
由于图像分类无法对未知物体或无物体时的结果进行正确预测, 所以需要进行一些预处理。



### yolo-fastest 图像识别实现
由于RK3399, Jetson Nano等廉价Linux开发板算力较弱无法流畅运行yolov4, 外加yolov4训练对电脑需求过高, 故本项目不再使用yolov4实现。  
本项目使用了基于华为垃圾分类竞赛数据集基础上的拓展数据集 (读起来真拗口。。)  

本项目使用的源dataset (VOC格式): https://aistudio.baidu.com/aistudio/datasetdetail/70610  
本项目使用的源dataset (图像分类格式): 

我根据比赛要求改进的dataset:  
[Classification](./dataset/Classification)  
[yolo-mark](./dataset/yolo-mark)

需要配置[Darknet](https://github.com/AlexeyAB/darknet)环境。  

由于竞赛需求, 本人只使用了上述数据集的部分种类  
```
classes = {"干电池" : 0, "陶瓷器皿" : 1, "易拉罐" : 2, "烟蒂" : 3, "果皮果肉" : 4, "菜帮菜叶" : 5, "饮料瓶" : 6}
```

本人的 yolo-fastest 实现:  
使用方法:  
```
cd yolo-fastest
darknet detector demo .\train.data .\yolo-fastest.cfg .\backup\yolo-fastest_best.weights -c 0
```
![](./yolo-fastest/test.png)

但是实测效果一般, 故比赛中又改为了 Xception。

## STM32端具体实现

## 机械结构及设计思路
暂未整理
