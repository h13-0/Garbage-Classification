#include "Timer3.h"

#include "stm32f10x.h"

/**********项目所需库***********/
#include "TIM_Init.h"
#include "usart.h"

static uint8_t times = 0;
static double RecycleSum = 0, KitchenSum = 0, HarmfulSum = 0, OtherSum = 0;

void CapacityDetectorInit()
{
		//初始化数据采集定时器
		//3秒触发一次, 每次采集3次超声波的数据, 然后3次均值后上报。
		TIM3_Timer_Init(17999, 11999);
}

void Timer3_IQR()
{
		//Recycle
		if(times < 3)
		{
				
		}
		//Kitchen
		else if(times < 6)
		{
		}
		//Harmful
		else if(times < 9)
		{
				
		}
		//Other
		else if(times < 12)
		{
				//
				if(times == 11)
				{
						//OtherSum +=
						printf("Capacity:Other:%lf\r\n", OtherSum/3);
						OtherSum = 0.0;
						times = 0;
				}					
		} else {
				times = 0;
		}
}
