#include "holder.h"
#include "stm32f10x.h"
#include "PID.h"
#include "usart.h"
#include "math.h"
#include "stdlib.h"

//变量
static PIDtypedef PIDRow;
static PIDtypedef PIDCol;


/*******************************************************************/
//云台PWM IO初始化函数:
//参数:
//  无
//
//作用:
//  初始化定时器及其引脚
//
//备注:
//  云台水平旋转舵机接到TIM3_Channel1
//  云台垂直旋转舵机接到TIM3_Channel2
/*******************************************************************/
void HolderIOinit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设

    //初始化Channel1 PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    //初始化Channel2 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = 19999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =70; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM3 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器


    //初始化TIM3 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}


/*******************************************************************/
//云台PID初始化函数, 用于控制其达到固定角度:
//参数:
//  无
//
//作用:
//  初始化云台控制PID
/*******************************************************************/
void HolderPIDinit()
{
    PIDCol.proportion = 15.0;
    PIDCol.integral = 0.15;
    PIDCol.derivative = 100.0;

    PIDRow.proportion = 15.0;
    PIDRow.integral = 0.15;
    PIDRow.derivative = 100.0;
}

/*******************************************************************/
//云台速度控制函数:
//参数:
//  int16_t target; 有效值:-1000 ~ 1000
//
//作用:
//  控制云台速度
/*******************************************************************/
void setRowSpeed(int16_t target)
{
		if(target > 1000)
				target = 1000;
		
		if(target < -1000)
				target = -1000;
	
    target += 1500;
    TIM_SetCompare1(TIM3,target);                      //50hz 20000 -> 20ms 1 -> 0.001ms; 0.5ms~2.5ms,500~2500
}

void setColSpeed(int16_t target)
{
		if(target > 1000)
				target = 1000;
		
		if(target < -1000)
				target = -1000;
		
    target += 1500;
    TIM_SetCompare2(TIM3,target);                      //50hz 20000 -> 20ms 1 -> 0.001ms; 0.5ms~2.5ms,500~2500
}

/*******************************************************************/
//云台角度控制函数:
//参数:
//  double target; 有效值: -180 ~ 180
//
//作用:
//  控制云台角度
/*******************************************************************/
void setRowAngle(double target)
{
    PIDRow.setpoint = target;
}

void setColAngle(double target)
{
    PIDCol.setpoint = target;
}

/*******************************************************************/
//云台控制实现函数
//参数:
//  CurrentRow -> 当前水平角度 由MPU6050获得并传入
//  CurrentCol -> 当前垂直角度 由MPU6050获得并传入
//
//备注:
//  请在需要控制云台运动或者保持姿态时 **以100hz的频率** 调用此函数
/*******************************************************************/
void executeHolder(double CurrentCol,double CurrentRow)
{
    //上一次的status
    //bit:
    //[0]: 上一次Col的正负 正:1 负:0
    //[1]: 上一次Row的正负 正:1 负:0
    //[2]
    //[3]
    //[4]
    //[5]
    //[6]
    //[7]
    static uint8_t laststatus = 0;

    int16_t pidvalue;

    //Culate Col
    if(abs((float)CurrentCol - (float)PIDCol.setpoint) > Permissible_Error)
    {
        if(laststatus&0x80 && (CurrentCol < PIDCol.setpoint))
        {
            laststatus &= 0x7f;
            PIDCol.sum_error = 0;
        }

        if(!(laststatus&0x80) && (CurrentCol > PIDCol.setpoint))
        {
            laststatus |= 0x80;
            PIDCol.sum_error = 0;
        }

        pidvalue = incPIDcalc(&PIDCol,CurrentCol);

        if(pidvalue >= 1000)
            pidvalue = 1000;

        if(pidvalue <= -1000)
            pidvalue = -1000;

        TIM_SetCompare1(TIM3,pidvalue + 1500);                      //50hz 20000 -> 20ms 1 -> 0.001ms; 0.5ms~2.5ms,500~2500
    } else {
				PIDCol.sum_error = 0;
    }
		//printf("CurrentCol: %lf,pidvalue: %d\r\n",CurrentCol,pidvalue);

		//Culate Row
    if(abs((float)CurrentRow - (float)PIDRow.setpoint) > Permissible_Error)
    {
        if(laststatus&0x40 && (CurrentRow < PIDRow.setpoint))
        {
            laststatus &= 0xbf;
            PIDRow.sum_error = 0;
        }

        if(!(laststatus&0x40) && (CurrentRow > PIDRow.setpoint))
        {
            laststatus |= 0x40;
            PIDRow.sum_error = 0;
        }

        pidvalue = incPIDcalc(&PIDRow,CurrentRow);

        if(pidvalue >= 1000)
            pidvalue = 1000;

        if(pidvalue <= -1000)
            pidvalue = -1000;

        TIM_SetCompare2(TIM3,1500 - pidvalue);                      //50hz 20000 -> 20ms 1 -> 0.001ms; 0.5ms~2.5ms,500~2500
    } else {
        PIDRow.sum_error = 0;
    }
    //printf("CurrentRow: %lf,pidvalue: %d\r\n",CurrentRow,pidvalue);
}

void setColP(double CP)
{
    PIDCol.proportion = CP;
}

void setColI(double CI)
{
    PIDCol.integral = CI;
}

void setColD(double CD)
{
    PIDCol.derivative = CD;
}

void setRowP(double RP)
{
    PIDRow.proportion = RP;
}

void setRowI(double RI)
{
    PIDRow.integral = RI;
}

void setRowD(double RD)
{
    PIDRow.derivative = RD;
}

