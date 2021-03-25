# STM32下位机端

## 串口通信格式
### 基础格式
```
${key}[:${class}]:${value}
```

### 垃圾桶容量信息
```
Capacity:${class}:${value}
```

class分别有: `Recycle` `Kitchen` `Harmful` `Other`, 分别对应四种垃圾

value表示当前的容量, int, 取值0-100

eg:
```
Capacity:Recycle:50
```
表示可回收垃圾容量为50%

### 挡板信息
```
Baffle:${class}:${msg}
```

class分别有: `Left` `Right` `Middle`, 分别对应左、右挡板和终端挡板

msg分别有: 
STM32端发送: `Opened` `Closed`, 分别表示成功开启/关闭
Linux端发送: `Open` `Close`, 分别表示开启/关闭

eg:


### 传送带信息
```
Conveyor:${cmd}
```

cmd分别有:
Linux端发送: `Forward` `Backward` `Stop`
STM32端不发送任何消息



### 滑台云台信息
此部分只负责把垃圾投送到对应垃圾桶

Linux:
```
Slider:${class}
```
class即垃圾种类, `Recycle` `Kitchen` `Harmful` `Other`


STM32:
```
Slider:OK
```



