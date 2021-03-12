#ifndef _USART1_IRQ_H_
#define _USART1_IRQ_H_

#include "stm32f10x.h"

#define USART1_BUF_LEN  			 32

#define USART1_READLINE        1

void USART1_Handler(void);

#if USART1_READLINE
/**
 * @brief: Gets USART1 buffer data.
 * @param: uint8_t* buffer.
 * @return: length of 
 */
uint16_t USART1_ReadLine(uint8_t* buf);
#endif

#endif
