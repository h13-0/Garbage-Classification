#ifndef _ANO_TC_H_
#define _ANO_TC_H_

/************���豸����**************/
//#define S_ADDR 0x00  //�����豸
//#define S_ADDR 0xAF  //��λ��
//#define S_ADDR 0x01  //΢������
//#define S_ADDR 0x02  //΢������
//#define S_ADDR 0x03  //��������
//#define S_ADDR 0x04  //�캽�߷ɿ�
#define S_ADDR 0x05  //�ؿ��߷ɿ�
//#define S_ADDR 0x10  //��������
//#define S_ADDR 0x11  //����ң��2.0
//#define S_ADDR 0x22  //��������
//#define S_ADDR 0x29  //����OPENMV
//#define S_ADDR 0x29  //���߶�λģ��

/***********Ŀ���豸����*************/
#define D_ADDR 0xAF  //��������λ��
//#define D_ADDR 0x00  //�����豸
//#define D_ADDR 0x01  //΢������
//#define D_ADDR 0x02  //΢������
//#define D_ADDR 0x03  //��������
//#define D_ADDR 0x04  //�캽�߷ɿ�
//#define D_ADDR 0x05  //�ؿ��߷ɿ�
//#define D_ADDR 0x10  //��������
//#define D_ADDR 0x11  //����ң��2.0
//#define D_ADDR 0x22  //��������
//#define D_ADDR 0x29  //����OPENMV
//#define D_ADDR 0x29  //���߶�λģ��

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
