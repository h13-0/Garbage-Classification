#ifndef _BLUEROCKER_H_
#define _BLUEROCKER_H_
#include "stm32f10x.h"
#define LEN 32
typedef struct 
{  
	uint8_t keyVal;//������ֵ�����𲻻��0��1-8��
	uint16_t rockerVal;//ҡ�˵�ֵ��ͣ�±�0xff 0xff��0-360��
	uint8_t seekBarVal;//�����ֵ��0-100��
}BlueRockerTypeDef;

void BlueRocker_init(BlueRockerTypeDef* BlueRocker);
uint8_t Rocker_Data_Analyze(BlueRockerTypeDef* BlueRocker,uint8_t * str,uint8_t len);
void Usart_Receive(BlueRockerTypeDef* BlueRocker,unsigned char cha);
#endif
