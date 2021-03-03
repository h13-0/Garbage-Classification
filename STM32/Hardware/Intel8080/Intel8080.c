#include "Intel8080.h"

#include "stm32f10x_gpio.h"

void I8080_Init()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = I8080_GPIO_Mode;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		
		GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void I8080_Writebyte(uint8_t data){
		GPIOA -> ODR = data;
		//GPIO_Write(GPIOA,data);
}

uint8_t I8080_Readbyte(){
		return GPIOA -> IDR;
}
