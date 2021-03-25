#ifndef _TIM_INIT_H_
#define _TIM_INIT_H_

#include "stm32f10x.h"

void TIM2_PWM_Init(uint16_t arr,uint16_t psc);

void TIM3_Timer_Init(uint16_t arr,uint16_t psc);

void TIM3_PWM_Init(uint16_t arr,uint16_t psc);

void TIM4_Timer_Init(uint16_t arr,uint16_t psc);

void TIM4_PWM_Init(uint16_t arr,uint16_t psc);

#endif
