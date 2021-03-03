#include "stm32f10x.h"
#include "SystemClock.h"

#define SysLoadValue SystemCoreClock/8000
static uint64_t time = 0;

void SystemClockInit()
{ 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 										//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  					//先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  								//从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 										//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  																		//初始化NVIC寄存器
	
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);								//选择外部时钟  HCLK/8
		SysTick->LOAD = SysLoadValue;
		SysTick->VAL = 0x00;																									//清空计数器
		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;	//启动Systick, 启动Systick中断
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
