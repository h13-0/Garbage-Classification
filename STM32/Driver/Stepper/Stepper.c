#include "Stepper.h"

/**********STM32标准库**********/
#include "stm32f10x.h"

/**********项目所需库***********/
#include "SystemClock.h"

Stepper_TypeDef Stepper_Init(GPIO_TypeDef* DIR_Port, uint16_t DIR_Pin, GPIO_TypeDef* STEP_Port, uint16_t STEP_Pin)
{
		Stepper_ClockEnable();
		
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_InitStructure.GPIO_Pin = DIR_Pin;
		GPIO_Init(DIR_Port, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = STEP_Pin;
		GPIO_Init(STEP_Port, &GPIO_InitStructure);
	
		Stepper_TypeDef Stepper = {.DIR_Port = DIR_Port, .DIR_Pin = DIR_Pin, .STEP_Port = STEP_Port, .STEP_Pin = STEP_Pin};
		return Stepper;
}

void Stepper_Forward(Stepper_TypeDef* stepper, uint16_t steps, uint16_t delayus)
{
		GPIO_SetBits(stepper -> DIR_Port, stepper -> DIR_Pin);
	
		for(uint16_t i = 0; i < steps; i++)
		{
				GPIO_SetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
				delay_us(delayus);
				GPIO_ResetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
				delay_us(delayus);
		}
}

void Stepper_Backward(Stepper_TypeDef* stepper, uint16_t steps, uint16_t delayus)
{
		GPIO_ResetBits(stepper -> DIR_Port, stepper -> DIR_Pin);
	
		for(uint16_t i = 0; i < steps; i++)
		{
				GPIO_SetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
				delay_us(delayus);
				GPIO_ResetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
				delay_us(delayus);
		}
}

void Stepper_ForwardMode(Stepper_TypeDef* stepper)
{
		GPIO_SetBits(stepper -> DIR_Port, stepper -> DIR_Pin);
}

void Stepper_BackwardMode(Stepper_TypeDef* stepper)
{
		GPIO_ResetBits(stepper -> DIR_Port, stepper -> DIR_Pin);
}	

inline void Stepper_SingleStep(Stepper_TypeDef* stepper, uint16_t delayus)
{
		GPIO_SetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
		delay_us(delayus);
		GPIO_ResetBits(stepper -> STEP_Port, stepper -> STEP_Pin);
		delay_us(delayus);
}
