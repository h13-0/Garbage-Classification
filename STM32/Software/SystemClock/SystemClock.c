#include "stm32f10x.h"
#include "SystemClock.h"

#define SysLoadValue SystemCoreClock/8000
static uint64_t time = 0;

void SystemClockInit()
{ 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 										//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  					//��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  								//�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 										//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  																		//��ʼ��NVIC�Ĵ���
	
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);								//ѡ���ⲿʱ��  HCLK/8
		SysTick->LOAD = SysLoadValue;
		SysTick->VAL = 0x00;																									//��ռ�����
		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;	//����Systick, ����Systick�ж�
}

inline void Systick_IQR()
{
		time ++;
}

uint64_t Get_ms()
{
		return time;
}

uint64_t Get_us()
{
		return (time*1000 + (SysLoadValue - SysTick->VAL)/9);;
}

void delay_ms(uint32_t delaytime)
{
		uint64_t TimeInitalValue = time;
		while((time - TimeInitalValue) < delaytime);
}

void delay_us(uint32_t delaytime)
{
		uint64_t TimeInitalValue = time*1000 + SysLoadValue - SysTick->VAL;
		while((time*1000 + SysLoadValue - SysTick->VAL - TimeInitalValue) < delaytime);
}
