#ifndef _Intel8080_h
#define _Intel8080_h
#include <stm32f10x_gpio.h> 

//为了效率,暂且只支持使用同一组IO做并行输出
//输出的IO组
#define Port GPIO

//支持IO复用功能,可能会降低效率 (暂时没写完,开启也没用)
//#define SupportIOReuse

//设置GPIO模式,一般设置推挽就好,在需要其他电平的时候可以选择为上下拉模式
#define I8080_GPIO_Mode GPIO_Mode_Out_PP;


//声明函数
void I8080_Writebyte(uint8_t);
void I8080_Init(void);
uint8_t I8080_Readbyte(void);


/*
typedef enum
{
	
}I8080_Mode;

typedef struct
{
		GPIO_TypeDef*		 	GPIOx;
		GPIOMode_TypeDef 	Mode;
		GPIOSpeed_TypeDef Speed;
		
}I8080_TypeDef;
*/













#endif
