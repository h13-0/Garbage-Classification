#include "holder.h"
#include "stm32f10x.h"
#include "PID.h"
#include "usart.h"
#include "math.h"
#include "stdlib.h"

//����
static PIDtypedef PIDRow;
static PIDtypedef PIDCol;


/*******************************************************************/
//��̨PWM IO��ʼ������:
//����:
//  ��
//
//����:
//  ��ʼ����ʱ����������
//
//��ע:
//  ��̨ˮƽ��ת����ӵ�TIM3_Channel1
//  ��̨��ֱ��ת����ӵ�TIM3_Channel2
/*******************************************************************/
void HolderIOinit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO����

    //��ʼ��Channel1 PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    //��ʼ��Channel2 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = 19999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =70; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM3 Channel1 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���


    //��ʼ��TIM3 Channel2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}


/*******************************************************************/
//��̨PID��ʼ������, ���ڿ�����ﵽ�̶��Ƕ�:
//����:
//  ��
//
//����:
//  ��ʼ����̨����PID
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
//��̨�ٶȿ��ƺ���:
//����:
//  int16_t target; ��Чֵ:-1000 ~ 1000
//
//����:
//  ������̨�ٶ�
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
//��̨�Ƕȿ��ƺ���:
//����:
//  double target; ��Чֵ: -180 ~ 180
//
//����:
//  ������̨�Ƕ�
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
//��̨����ʵ�ֺ���
//����:
//  CurrentRow -> ��ǰˮƽ�Ƕ� ��MPU6050��ò�����
//  CurrentCol -> ��ǰ��ֱ�Ƕ� ��MPU6050��ò�����
//
//��ע:
//  ������Ҫ������̨�˶����߱�����̬ʱ **��100hz��Ƶ��** ���ô˺���
/*******************************************************************/
void executeHolder(double CurrentCol,double CurrentRow)
{
    //��һ�ε�status
    //bit:
    //[0]: ��һ��Col������ ��:1 ��:0
    //[1]: ��һ��Row������ ��:1 ��:0
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

