#ifndef _SYSTEMCLOCK_H_
#define _SYSTEMCLOCK_H_
#include "stm32f10x.h"

//Inital api
/**
  * @brief  Init Clock System
  * @param  None
  * @retval None
	* @note   Interrupt grouping is initialized here, and the Systick Interrupt has the *HIGHEST* priority!
  */
void SystemClockInit(void);

//Interrupt Handle (inline api)
/**
  * @brief  Init Clock System
  * @param  None
  * @retval None
	* @note   It MUST BE ADDED TO SysTick_Handler in stm32f10x_it.c !
  */
void Systick_IQR(void);

/**
  * @brief  Get the number of milliseconds since SystemClockInit()
  * @param  None
  * @retval uint64_t ms
  */
uint64_t Get_ms(void);

/**
  * @brief  Gets the number of microseconds since SystemClockInit()
  * @param  None
  * @retval uint64_t us
  */
uint64_t Get_us(void);

/**
  * @brief  Delay n ms
  * @param  None
  * @retval None
  */
void delay_ms(uint32_t delaytime);

/**
  * @brief  Delay n us
  * @param  None
  * @retval None
  */
void delay_us(uint32_t delaytime);

#endif
