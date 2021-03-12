#include "Hall_Sensor.h"

#include "stm32f10x.h"


void Hall_Sensors_Init()
{
		RCC_PeriphClockEnable();
	
		/*����ṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*ʹ��ʱ��*/
    RCC_PeriphClockEnable();

    /*�����ж�Դ*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line11|EXTI_Line12;             //����EXTI�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //����EXTIģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //����EXTI������ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //ʹ��EXTI�ж���
    EXTI_Init(&EXTI_InitStructure);

    /*�����ж�ͨ��*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;    //EXTIx �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ������ȼ�(����ռ��ʱ����)
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                         //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
		
		
		GPIO_InitStructure.GPIO_Pin = FRONT_LIMIT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //����ģʽ
    GPIO_Init(FRONT_LIMIT_SENDOR_PORT,&GPIO_InitStructure);       			//����GPIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
		
		GPIO_InitStructure.GPIO_Pin = BEHIND_LIMIT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //����ģʽ
    GPIO_Init(BEHIND_LIMIT_SENSOR_PORT,&GPIO_InitStructure);       			//����GPIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);
}

inline void Front_Limit_IRQHandler()
{
	/*
		if(GPIO_ReadInputDataBit(Left_Limit_Sensor_Port,Left_Limit_Sensor_Pin))
				ResetLeftLimit();
		else
				SetLeftLimit();
	*/
}

inline void Behind_Limit_IRQHandler()
{
	/*
		if(GPIO_ReadInputDataBit(Right_Limit_Sensor_Port,Right_Limit_Sensor_Pin))
				ResetRightLimit();
		else
				SetRightLimit();
	*/
}

inline uint8_t Front_Triggered()
{
		
}

inline uint8_t Behind_Triggered()
{
		
}
