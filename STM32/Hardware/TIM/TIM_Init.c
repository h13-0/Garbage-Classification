#include "TIM_Init.h"

/**********STM32��׼��**********/
#include "stm32f10x.h"


void TIM2_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);				//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  			//ʹ��GPIO�����AFIO���ù���ģ��ʱ��

		//ͨ��GPIO�ṹ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //��ʼ��Channel 1234
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //����IOһ���ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //��ʼ��Channel2
    
		//��ʼ��GPIO
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //��ʼ��TIM2
    TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
		
		//��ʼ��Channel1
    //TIM_OC1Init(TIM2, &TIM_OCInitStructure);  									//����ָ���Ĳ�����ʼ������TIM4 OC1
    //TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  					//ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���


    //��ʼ��Channel2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
		//��ʼ��Channel3
    //TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    //TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
		//��ʼ��Channel4
    //TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    //TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);  																		//ʹ��TIM2
}


void TIM3_Timer_Init(uint16_t arr,uint16_t psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 									//ʹ��ָ����TIM3�ж�,��������ж�

		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						//TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  														//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

		TIM_Cmd(TIM3, ENABLE);  																		//ʹ��TIMx����
}


void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

		//����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	     //ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

		//ͨ��GPIO�ṹ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //��ʼ��Channel 1��2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    
		//��ʼ��GPIO
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//��ʼ��Channel 3��4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    
		//��ʼ��GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM4 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
		
		//��ʼ��Channel1
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  									//����ָ���Ĳ�����ʼ������TIM4 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  					//ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���


    //��ʼ��Channel2
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		//��ʼ��Channel3
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		//��ʼ��Channel4
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_Cmd(TIM3, ENABLE);  																		//ʹ��TIM3
}


void TIM4_Timer_Init(uint16_t arr,uint16_t psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//ʱ��ʹ��

		TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 									//ʹ��ָ����TIM4�ж�,��������ж�

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  						//TIM2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  														//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

		TIM_Cmd(TIM4, ENABLE);  																		//ʹ��TIMx����
}


void TIM4_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);				//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  			//ʹ��GPIO�����AFIO���ù���ģ��ʱ��

		//ͨ��GPIO�ṹ��
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //��ʼ��Channel 1234
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //����IOһ���ʼ��
    
		//��ʼ��GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //��ʼ��TIM4
    TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM4 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
		
		//��ʼ��Channel1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  									//����ָ���Ĳ�����ʼ������TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  					//ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���


    //��ʼ��Channel2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//��ʼ��Channel3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//��ʼ��Channel4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);  																		//ʹ��TIM4
}
