#include "Commands.h"
/**********C语言标准库**********/
#include "string.h"
#include "stdlib.h"

/**********STM32标准库**********/
#include "stm32f10x.h"


/**********项目所需库***********/
#include "PID.h"
#include "usart.h"
#include "SystemClock.h"

//#define STM32PROJECTDEBUG

//这里的return的数值在这个工程里面其实没什么意义(但是return很有意义), 但是我懒得改了
uint8_t Command(uint8_t* Rec,uint8_t len)
{
#ifdef STM32PROJECTDEBUG
	if(len)
		printf("Echo: %d %d %s\r\n",len,strlen((char*)Rec),Rec);
#endif

		return 0;
}
