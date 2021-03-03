#ifndef holder_h
#define holder_h

#include "PID.h"

//硬件定义
//TIM3_Channel1 -> Row -> 一级舵机
//TIM3_Channel2 -> Col -> 二级舵机

//宏定义设置
#define Permissible_Error 0.5  //请写允许误差的角度绝对值

//APIs

/*******************************************************************/
//*必须* 云台PWM IO初始化函数:
//参数:
//  无
//
//作用:
//  初始化定时器及其引脚
//
//备注:
//  云台水平旋转舵机接到TIM3_Channel1
//  云台垂直旋转舵机接到TIM3_Channel2
/*******************************************************************/
void HolderIOinit(void);


/*******************************************************************/
//*可选* 云台PID初始化函数:
//参数:
//  无
//
//作用:
//  初始化云台控制PID 如果选择速度控制法则不需要
/*******************************************************************/
void HolderPIDinit(void);


/*******************************************************************/
//云台速度控制函数:
//参数:
//  int16_t target; 有效值:-1000 ~ 1000
//
//作用:
//  控制云台速度
/*******************************************************************/
void setRowSpeed(int16_t target);
void setColSpeed(int16_t target);


/*******************************************************************/
//云台角度控制函数:
//参数:
//  double target; 有效值: -180 ~ 180
//
//作用:
//  控制云台角度
/*******************************************************************/
void setRowAngle(double target);
void setColAngle(double target);


/*******************************************************************/
//*可选* 云台控制实现函数
//参数:
//  CurrentRow -> 当前水平角度 由MPU6050获得并传入
//  CurrentCol -> 当前垂直角度 由MPU6050获得并传入
//
//备注:
//  请在需要控制云台运动或者保持姿态时 **以100hz的频率** 调用此函数
/*******************************************************************/
void executeHolder(double CurrentCol,double CurrentRow);

//Debug APIs
void setColP(double CP);
void setColI(double CI);
void setColD(double CD);

void setRowP(double RP);
void setRowI(double RI);
void setRowD(double RD);

#endif
