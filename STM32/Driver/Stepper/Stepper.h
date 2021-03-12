#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "stm32f10x.h"

typedef struct 
{
		GPIO_TypeDef* DIR_Port;
		uint16_t DIR_Pin;
		GPIO_TypeDef* STEP_Port;
		uint16_t STEP_Pin;
}Stepper_TypeDef;

//Configs
#define Stepper_ClockEnable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE)

/**
 * @brief: Init Stepper
 * @param: Port and Pin for Stepper.
 * @return: Stepper_TypeDef stepper.
 */

Stepper_TypeDef Stepper_Init(GPIO_TypeDef* DIR_Port, uint16_t DIR_Pin, GPIO_TypeDef* STEP_Port, uint16_t STEP_Pin);


/**********Automatic APIs***********/

/**
 * @brief: Turn the stepper counterclockwise.
 * @param:
 * 		Stepper_TypeDef* stepper -> The stepper to be operated.
 * 		uint16_t steps -> The number of steps.
 *  	uint16_t delay -> The interval between each *half* step.
 * @note: 
 */
void Stepper_Forward(Stepper_TypeDef* stepper, uint16_t steps, uint16_t delayus);

/**
 * @brief: Turn the stepper clockwise.
 * @param:
 * 		Stepper_TypeDef* stepper -> The stepper to be operated.
 * 		uint16_t steps -> The number of steps.
 *  	uint16_t delay -> The interval between each *half* step.
 * @note: 
 */
void Stepper_Backward(Stepper_TypeDef* stepper, uint16_t steps, uint16_t delayus);


/**********Manual APIs***********/

/**
 * @brief: Switch the stepper to counterclockwise Mode.
 * @param: Stepper_TypeDef* stepper -> The stepper to be operated.
 */
void Stepper_ForwardMode(Stepper_TypeDef* stepper);

/**
 * @brief: Switch the stepper to clockwise Mode.
 * @param: Stepper_TypeDef* stepper -> The stepper to be operated.
 */
void Stepper_BackwardMode(Stepper_TypeDef* stepper);

/**
 * @brief: Run Single Step(inline function).
 * @param: 
 * 		Stepper_TypeDef* stepper -> The stepper to be operated.
 * 		delayus -> The interval between each *half* step.
 */
void Stepper_SingleStep(Stepper_TypeDef* stepper, uint16_t delayus);

#endif
