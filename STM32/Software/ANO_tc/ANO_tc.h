#ifndef _ANO_TC_H_
#define _ANO_TC_H_

/************本设备定义**************/
//#define S_ADDR 0x00  //其他设备
//#define S_ADDR 0xAF  //上位机
//#define S_ADDR 0x01  //微型四轴
//#define S_ADDR 0x02  //微型六轴
//#define S_ADDR 0x03  //匿名八轴
//#define S_ADDR 0x04  //领航者飞控
#define S_ADDR 0x05  //拓空者飞控
//#define S_ADDR 0x10  //匿名数传
//#define S_ADDR 0x11  //匿名遥控2.0
//#define S_ADDR 0x22  //匿名光流
//#define S_ADDR 0x29  //匿名OPENMV
//#define S_ADDR 0x29  //无线定位模块

/***********目标设备定义*************/
#define D_ADDR 0xAF  //发送至上位机
//#define D_ADDR 0x00  //其他设备
//#define D_ADDR 0x01  //微型四轴
//#define D_ADDR 0x02  //微型六轴
//#define D_ADDR 0x03  //匿名八轴
//#define D_ADDR 0x04  //领航者飞控
//#define D_ADDR 0x05  //拓空者飞控
//#define D_ADDR 0x10  //匿名数传
//#define D_ADDR 0x11  //匿名遥控2.0
//#define D_ADDR 0x22  //匿名光流
//#define D_ADDR 0x29  //匿名OPENMV
//#define D_ADDR 0x29  //无线定位模块

#include "stdint.h"
#include "stm32f10x.h"
	
typedef struct
{
	  int16_t ACC_X;
	  int16_t ACC_Y;
	  int16_t ACC_Z;
	  int16_t GYRO_X;
	  int16_t GYRO_Y;
	  int16_t GYRO_Z;
	  int16_t MAG_X;
	  int16_t MAG_Y;
    int16_t MAG_Z;
}ANO_SENSER_Typedef;

void ANO_SENSER(USART_TypeDef * USARTx,ANO_SENSER_Typedef ANO_SENSER_MSG);

#endif
