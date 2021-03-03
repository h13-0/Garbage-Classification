#include "stm32f10x.h"
#include "H-Bridge.h"

/*******************************************************************/
//H��PWM IO��ʼ������:
//����:
//  ��
//
//����:
//  ��ʼ����ʱ����������
//
//��ע:
//  D0 -> TIM4_Channel1 -> B6
//	D1 -> TIM4_Channel2 -> B7
//	D2 -> TIM4_Channel3 -> B8
//	D4 -> TIM4_Channel4 -> B9
/*******************************************************************/
void H_BridegInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

		//ͨ��GPIO�ṹ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //��ʼ��Channel 1234
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //����IOһ���ʼ��
    
		//��ʼ��GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //��ʼ��TIM4
    TIM_TimeBaseStructure.TIM_Period = 999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM4 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
		
		//��ʼ��Channel1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���


    //��ʼ��Channel2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC2
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
		
		//��ʼ��Channel3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC3
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
		
		//��ʼ��Channel4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

/*******************************************************************/
//H��������ƺ���:
//����:
//  int16_t value; ��Чֵ:-1000 ~ 1000
//
//����:
//  ���PWMֵ
/*******************************************************************/
void SetChannel0Value(int16_t value)
{
		if(value > 1000)
				value = 1000;
		
		if(value < -1000)
				value = -1000;
		
		if(value > 0)
		{
				TIM_SetCompare1(TIM4,value);
				TIM_SetCompare2(TIM4,0);
		} else if(value < 0)
		{
				TIM_SetCompare1(TIM4,0);
				TIM_SetCompare2(TIM4,- value);
		} else if(value == 0)
		{
				TIM_SetCompare1(TIM4,0);
				TIM_SetCompare2(TIM4,0);
		}
}

void SetChannel1Value(int16_t value)
{
		if(value > 1000)
				value = 1000;
		
		if(value < -1000)
				value = -1000;
		
		if(value > 0)
		{
				TIM_SetCompare3(TIM4,value);
				TIM_SetCompare4(TIM4,0);
		} else if(value < 0)
		{
				TIM_SetCompare3(TIM4,0);
				TIM_SetCompare4(TIM4,- value);
		} else if(value == 0)
		{
				TIM_SetCompare3(TIM4,0);
				TIM_SetCompare4(TIM4,0);
		}
}

void Brake()
{
		TIM_SetCompare1(TIM4,1000);
		TIM_SetCompare2(TIM4,1000);
		TIM_SetCompare3(TIM4,1000);
		TIM_SetCompare4(TIM4,1000);
}
