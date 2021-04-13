#include "CapacityDetector.h"

#include "stm32f10x.h"

/**********��Ŀ�����***********/
#include "TIM_Init.h"
#include "usart.h"
#include "Ultrasonic_Ranging.h"

static uint8_t times = 0;
volatile static double RecycleSum = 0, KitchenSum = 0, HarmfulSum = 0, OtherSum = 0;
volatile static uint32_t recTimes = 0, kitTimes = 0, harmTimes = 0, othTimes = 0;
Ultrasonic_TypeDef RecUltra, KitUltra, HarmUltra, OthUltra;

//�ɻ���: 540 ~ 250
//��������: 460 ~ 242
//��������: 543 ~ 250
//�к�����: 370 ~ 240

#define RecMax 490
#define RecMin 410

#define KitMax 550
#define KitMin 250

#define OthMax 543
#define OthMin 250

#define HarmMax 370
#define HarmMin 240

void CapacityDetectorInit()
{
		//��ʼ�����ݲɼ���ʱ��
		//3�봥��һ��, ÿ�βɼ�3�γ�����������, Ȼ��3�ξ�ֵ���ϱ���
		RecUltra = Ultrasonic_Init(GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11);
		KitUltra = Ultrasonic_Init(GPIOA, GPIO_Pin_6, GPIOA, GPIO_Pin_7);
		
		//Oth B0 B1
		OthUltra = Ultrasonic_Init(GPIOB, GPIO_Pin_0, GPIOB, GPIO_Pin_1);
		HarmUltra = Ultrasonic_Init(GPIOA, GPIO_Pin_4,GPIOA, GPIO_Pin_5);
	
		//�Ŷ�ʱ������ִ�л�ȡ��������, �����
		//TIM3_Timer_Init(17999, 11999);
}

// �����ȡ���ݻ�����BUG �����, ����ֻ�ܲ���ϱ��ͼ��
void CapacityDetector_Handler()
{
		//Recycle
		if(times < 3)
		{
				double recTemp = Get_Distance_Value(&RecUltra);
				if(recTemp > 0)
				{
						RecycleSum += recTemp;
						recTimes ++;
				}
				
				if(recTemp > RecMax)
					recTemp = RecMax;
				
				if(recTemp < RecMin)
					recTemp = RecMin;
				
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
	
		
		printf("%lf\r\n", (double)(*rec));
		*rec = getAvailable(rec, RecMax, RecMin);
		printf("%lf\r\n", (double)(*kit));
		*kit = getAvailable(kit, 460, 242);
		printf("%lf\r\n", (double)(*oth));
		*oth = getAvailable(oth, 543, 250);
		printf("%lf\r\n", (double)(*harm));
		*harm = getAvailable(harm, 370, 240);
}
