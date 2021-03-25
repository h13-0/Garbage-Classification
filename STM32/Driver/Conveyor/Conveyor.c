#include "Conveyor.h"

#include "stm32f10x.h"

#include "TIM_Init.h"


// 由于传送带有"后台运行"的需要, 所以将PA1改为PWM输出IO
void ConveyorInit()
{
		TIM2_PWM_Init(719, 99);
		TIM_SetCompare2(TIM2, 0);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = CONVEYOR_DIR_PIN;
		GPIO_Init(CONVEYOR_DIR_PORT, &GPIO_InitStructure);
}

void SetForward()
{
		GPIO_SetBits(CONVEYOR_DIR_PORT, CONVEYOR_DIR_PIN);
}

void SetBackward()
{
		GPIO_ResetBits(CONVEYOR_DIR_PORT, CONVEYOR_DIR_PIN);
}

void ConveyorForward()
{
		SetForward();
		TIM_SetCompare2(TIM2, 360);
}

void ConveyorBackward()
{
		SetBackward();
		TIM_SetCompare2(TIM2, 360);
}

void ConveyorStop()
{
		TIM_SetCompare2(TIM2, 0);
}
