#include "Hall_Sensor.h"

#include "stm32f10x.h"


void Hall_Sensors_Init()
{
		RCC_PeriphClockEnable();
	
		/*定义结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*使能时钟*/
    RCC_PeriphClockEnable();

    /*配置中断源*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line11|EXTI_Line12;             //配置EXTI中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //配置EXTI模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //配置EXTI触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //使能EXTI中断线
    EXTI_Init(&EXTI_InitStructure);

    /*设置中断通道*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;    //EXTIx 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占最高优先级(反正占用时间少)
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化NVIC寄存器
		
		
		GPIO_InitStructure.GPIO_Pin = FRONT_LIMIT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //下拉模式
    GPIO_Init(FRONT_LIMIT_SENDOR_PORT,&GPIO_InitStructure);       			//设置GPIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
		
		GPIO_InitStructure.GPIO_Pin = BEHIND_LIMIT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //下拉模式
    GPIO_Init(BEHIND_LIMIT_SENSOR_PORT,&GPIO_InitStructure);       			//设置GPIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);
}

inline void Front_Limit_IRQHandler()
{
	/*
		if(GPIO_ReadInputDataBit(Left_Limit_Sensor_Port,Left_Limit_Sensor_Pin))
				ResetLeftLimit();
		else
				SetLeftLimit();
	*/
}

inline void Behind_Limit_IRQHandler()
{
	/*
		if(GPIO_ReadInputDataBit(Right_Limit_Sensor_Port,Right_Limit_Sensor_Pin))
				ResetRightLimit();
		else
				SetRightLimit();
	*/
}

inline uint8_t Front_Triggered()
{
		
}

inline uint8_t Behind_Triggered()
{
		
}
