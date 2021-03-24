#include "Timer3.h"

#include "stm32f10x.h"

/**********��Ŀ�����***********/
#include "TIM_Init.h"
#include "usart.h"

static uint8_t times = 0;
static double RecycleSum = 0, KitchenSum = 0, HarmfulSum = 0, OtherSum = 0;

void CapacityDetectorInit()
{
		//��ʼ�����ݲɼ���ʱ��
		//3�봥��һ��, ÿ�βɼ�3�γ�����������, Ȼ��3�ξ�ֵ���ϱ���
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
