# STM32下位机端

## 串口通信格式
### 基础格式
```
key0[:key1]:value
```

### 垃圾桶容量信息
```
Capacity:${class}:${value}
```
eg:
```
Capacity:Recycle:50
```
表示可回收垃圾容量为50%

### 挡板信息
```
Baffle:${class}:${msg}
```

### 