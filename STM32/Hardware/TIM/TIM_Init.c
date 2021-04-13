#include "TIM_Init.h"

/**********STM32标准库**********/
#include "stm32f10x.h"


void TIM2_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);				//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  			//使能GPIO外设和AFIO复用功能模块时钟

		//通用GPIO结构体
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //初始化Channel 1234
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //所有IO一起初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //初始化Channel2
    
		//初始化GPIO
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化TIM2
    TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
		
		//初始化Channel1
    //TIM_OC1Init(TIM2, &TIM_OCInitStructure);  									//根据指定的参数初始化外设TIM4 OC1
    //TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  					//使能TIM4在CCR2上的预装载寄存器


    //初始化Channel2
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
		//初始化Channel3
    //TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    //TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
		//初始化Channel4
    //TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    //TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);  																		//使能TIM2
}


void TIM3_Timer_Init(uint16_t arr,uint16_t psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 									//使能指定的TIM3中断,允许更新中断

		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						//TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		TIM_Cmd(TIM3, ENABLE);  																		//使能TIMx外设
}


void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

		//开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	     //使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

		//通用GPIO结构体
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //初始化Channel 1、2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    
		//初始化GPIO
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//初始化Channel 3、4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    
		//初始化GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM4 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
		
		//初始化Channel1
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  									//根据指定的参数初始化外设TIM4 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  					//使能TIM4在CCR2上的预装载寄存器


    //初始化Channel2
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		//初始化Channel3
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		//初始化Channel4
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_Cmd(TIM3, ENABLE);  																		//使能TIM3
}


void TIM4_Timer_Init(uint16_t arr,uint16_t psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//时钟使能

		TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; 									//设置用来作为TIMx时钟频率除数的预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 									//使能指定的TIM4中断,允许更新中断

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  						//TIM2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		TIM_Cmd(TIM4, ENABLE);  																		//使能TIMx外设
}


void TIM4_PWM_Init(uint16_t arr,uint16_t psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);				//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  			//使能GPIO外设和AFIO复用功能模块时钟

		//通用GPIO结构体
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //初始化Channel 1234
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //所有IO一起初始化
    
		//初始化GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //初始化TIM4
    TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 									//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM4 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
		
		//初始化Channel1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  									//根据指定的参数初始化外设TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  					//使能TIM4在CCR2上的预装载寄存器


    //初始化Channel2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//初始化Channel3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//初始化Channel4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);  																		//使能TIM4
}
