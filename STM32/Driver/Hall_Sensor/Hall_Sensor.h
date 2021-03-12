#ifndef _HALL_SENSOR_H_
#define _HALL_SENSOR_H_

#include "stm32f10x.h"

#define FRONT_LIMIT_SENDOR_PORT GPIOA
#define FRONT_LIMIT_SENSOR_PIN GPIO_Pin_11

#define BEHIND_LIMIT_SENSOR_PORT GPIOA
#define BEHIND_LIMIT_SENSOR_PIN GPIO_Pin_12

#define RCC_PeriphClockEnable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE)

void Hall_Sensors_Init(void);

uint8_t Front_Triggered(void);

uint8_t Behind_Triggered(void);

void Front_Limit_IRQHandler(void);

void Behind_Limit_IRQHandler(void);

#endif
