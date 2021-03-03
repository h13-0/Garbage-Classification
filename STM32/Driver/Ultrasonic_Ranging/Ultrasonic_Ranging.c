#include "Ultrasonic_Ranging.h"

#include "stm32f10x.h"

#include "SystemClock.h"

void Ultrasonic_Init()
{
		RCC_PeriphClockEnable();
	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		//��ʼ��Trig
    GPIO_InitStructure.GPIO_Pin = Trig_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Trig_Port, &GPIO_InitStructure);
	
		//��ʼ��Echo
    GPIO_InitStructure.GPIO_Pin = Echo_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Echo_Port, &GPIO_InitStructure);
}

double Get_Distance_Value()
{
		GPIO_SetBits(Trig_Port,Trig_Pin);
		delay_us(20);
		GPIO_ResetBits(Trig_Port,Trig_Pin);
		
		uint64_t StartTime = Get_us();
		
		//�ȴ�ģ���ڲ������ߵ�ƽ
		while(GPIO_ReadInputDataBit(Echo_Port,Echo_Pin) != Bit_SET)
		{
				
				if((Get_us() - StartTime) >= 1500)
					return -1;
		}
	
		StartTime = Get_us();
		while(GPIO_ReadInputDataBit(Echo_Port,Echo_Pin) != Bit_RESET)
		{
				//����40ms, 6.8m�Ͳ��ȴ�
				if((Get_us() - StartTime) >= 40000)
					return -2;
		}
		
		return Speed_Of_Sound * (Get_us() - StartTime) / 2000.0;
}
