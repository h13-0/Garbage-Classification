#include "CapacityDetector.h"

#include "stm32f10x.h"

/**********项目所需库***********/
#include "TIM_Init.h"
#include "usart.h"
#include "Ultrasonic_Ranging.h"

//#define STM32_PROJRCT_DEBUG

static uint8_t times = 0;
volatile static double RecycleSum = 0, KitchenSum = 0, HarmfulSum = 0, OtherSum = 0;
volatile static uint32_t recTimes = 0, kitTimes = 0, harmTimes = 0, othTimes = 0;
Ultrasonic_TypeDef RecUltra, KitUltra, HarmUltra, OthUltra;

//可回收: 540 ~ 250
//厨余垃圾: 460 ~ 242
//其他垃圾: 543 ~ 250
//有害垃圾: 370 ~ 240

#define RecMax 320
#define RecMin 135

#define KitMax 350
#define KitMin 95

#define OthMax 320
#define OthMin 100

#define HarmMax 185
#define HarmMin 60

void CapacityDetectorInit()
{
		//初始化数据采集定时器
		//3秒触发一次, 每次采集3次超声波的数据, 然后3次均值后上报。
		RecUltra = Ultrasonic_Init(GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11);
		KitUltra = Ultrasonic_Init(GPIOA, GPIO_Pin_6, GPIOA, GPIO_Pin_7);
		
		//Oth B0 B1
		OthUltra = Ultrasonic_Init(GPIOB, GPIO_Pin_0, GPIOB, GPIO_Pin_1);
		HarmUltra = Ultrasonic_Init(GPIOA, GPIO_Pin_4,GPIOA, GPIO_Pin_5);
	
		//放定时器里面执行获取不到数据, 很奇怪
		//TIM3_Timer_Init(17999, 11999);
}

// 间隔获取数据会遇到BUG 很奇怪, 所以只能拆分上报和检测
void CapacityDetector_Handler()
{
		//Recycle
		if(times < 3)
		{
				double recTemp = Get_Distance_Value(&RecUltra);
				if(recTemp > 0)
				{
					/*
						if(recTemp > RecMax)
								recTemp = RecMax;
				
						if(recTemp < RecMin)
								recTemp = RecMin;
					*/
						RecycleSum += recTemp;
						recTimes ++;
				}
				
				times ++;
		}
		//Kitchen
		else if(times < 6)
		{
				double kitTemp = Get_Distance_Value(&KitUltra);
				if(kitTemp > 0)
				{
						KitchenSum += kitTemp;
						kitTimes ++;
				}
				
				times ++;
		}
		//Harmful
		else if(times < 9)
		{
				double harmTemp = Get_Distance_Value(&HarmUltra);
				if(harmTemp > 0)
				{
						HarmfulSum += harmTemp;
						harmTimes ++;
				}
				
				times ++;
		}
		//Other
		else if(times < 12)
		{
				double othTemp = Get_Distance_Value(&OthUltra);
				if(othTemp > 0)
				{
						OtherSum += othTemp;
						othTimes ++;
				}
				
				times ++;
		} else {
				times = 0;
		}
}

double getAvailable(double *current, uint16_t max, uint16_t min)
{
		if(*current > max)
			return 0.0;
		
		if(*current < min)
			return 100.0;
		
		double result = 0;
		result = (max - *current) / (max - min);
		return result * 100.0;
}

void CapacityDetector_getValue(double *rec, double *kit, double *oth, double *harm)
{
		*rec = RecycleSum / recTimes;
		recTimes = 0;
		RecycleSum = 0;
	
		*kit = KitchenSum / kitTimes;
		kitTimes = 0;
		KitchenSum = 0;
	
		*oth = OtherSum / othTimes;
		othTimes = 0;
		OtherSum = 0;
	
		*harm = HarmfulSum / harmTimes;
		harmTimes = 0;
		HarmfulSum = 0;
	
#ifdef STM32_PROJRCT_DEBUG
		printf("rec: %lf\r\n", *rec);
		printf("kit: %lf\r\n", *kit);
		printf("oth: %lf\r\n", *oth);
	printf("harm: %lf\r\n", *harm);
#endif
		*rec = getAvailable(rec, RecMax, RecMin);
		*kit = getAvailable(kit, KitMax, KitMin);
		*oth = getAvailable(oth, OthMax, OthMin);
		*harm = getAvailable(harm, HarmMax, HarmMin);
}
