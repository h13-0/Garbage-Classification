#ifndef _Intel8080_h
#define _Intel8080_h
#include <stm32f10x_gpio.h> 

//Ϊ��Ч��,����ֻ֧��ʹ��ͬһ��IO���������
//�����IO��
#define Port GPIO

//֧��IO���ù���,���ܻή��Ч�� (��ʱûд��,����Ҳû��)
//#define SupportIOReuse

//����GPIOģʽ,һ����������ͺ�,����Ҫ������ƽ��ʱ�����ѡ��Ϊ������ģʽ
#define I8080_GPIO_Mode GPIO_Mode_Out_PP;


//��������
void I8080_Writebyte(uint8_t);
void I8080_Init(void);
uint8_t I8080_Readbyte(void);


/*
typedef enum
{
	
}I8080_Mode;

typedef struct
{
		GPIO_TypeDef*		 	GPIOx;
		GPIOMode_TypeDef 	Mode;
		GPIOSpeed_TypeDef Speed;
		
}I8080_TypeDef;
*/













#endif
