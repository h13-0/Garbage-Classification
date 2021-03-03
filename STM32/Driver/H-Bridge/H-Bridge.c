#include "stm32f10x.h"
#include "H-Bridge.h"

/*******************************************************************/
//H桥PWM IO初始化函数:
//参数:
//  无
//
//作用:
//  初始化定时器及其引脚
//
//备注:
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


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

		//通用GPIO结构体
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    //初始化Channel 1234
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //所有IO一起初始化
    
		//初始化GPIO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //初始化TIM4
    TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = 71; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM4 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
		
		//初始化Channel1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器


    //初始化Channel2
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIM4 OC2
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
		
		//初始化Channel3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
		
		//初始化Channel4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器

    TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

/*******************************************************************/
//H桥输出控制函数:
//参数:
//  int16_t value; 有效值:-1000 ~ 1000
//
//作用:
//  输出PWM值
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
