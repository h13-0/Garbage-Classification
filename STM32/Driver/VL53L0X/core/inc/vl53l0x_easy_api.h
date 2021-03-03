#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_it.h"
#include "sys.h"
#include "SystemClock.h"

//VL53L0X传感器上电默认IIC地址为0X52(不包含最低位)
#define VL53L0X_Addr 0x52

//SDA SCL相关配置见vl53l0x_i2c.h

//Xshut(Rst)相关配置
#define RCC_Enable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE)
#define Xshut_Port GPIOB
#define Xshut_Pin  GPIO_Pin_5
#define VL53L0X_Xshut PBout(5)

//使能2.8V IO电平模式
#define USE_I2C_2V8  1

//测量模式
#define Default_Mode   0// 默认
#define HIGH_ACCURACY  1//高精度
#define LONG_RANGE     2//长距离
#define HIGH_SPEED     3//高速

//APIs
//初始化
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//初始化vl53l0x

//设置测量模式
//'mode' 参考上文 '测量模式'
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode);

//单次距离测量(宏函数)
#define vl53l0x_start_single_test(dev,pdata) VL53L0X_PerformSingleRangingMeasurement(dev, pdata)

//校准
#define adjust_num 5//校准错误次数
//校准函数:
//每个模块一段时间内校准一次即可, 校准完毕后把本函数串口终端返回值写入程序中即可。
//温差一般8度以上需要再次校准
VL53L0X_Error vl53l0x_adjust(VL53L0X_Dev_t *dev);

//vl53l0x复位函数
//dev:设备I2C参数结构体
void vl53l0x_reset(VL53L0X_Dev_t *dev);

#endif
