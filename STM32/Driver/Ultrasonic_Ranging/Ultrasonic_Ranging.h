#ifndef _ULTRASONIC_RANGING_H_
#define _ULTRASONIC_RANGING_H_

#define RCC_PeriphClockEnable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE)

#define Speed_Of_Sound 340.0
//#define Max_Distance   4000 //mm

#include "stm32f10x.h"

typedef struct 
{
		GPIO_TypeDef* Trig_Port;
		uint16_t Trig_Pin;
		GPIO_TypeDef* Echo_Port;
		uint16_t Echo_Pin;
}Ultrasonic_TypeDef;

Ultrasonic_TypeDef Ultrasonic_Init(GPIO_TypeDef* Trig_Port, uint16_t Trig_Pin, GPIO_TypeDef* Echo_Port, uint16_t Echo_Pin);

//虽然说Echo可以用中断 但是感觉没必要
double Get_Distance_Value(Ultrasonic_TypeDef* ultrasonic);

#endif
