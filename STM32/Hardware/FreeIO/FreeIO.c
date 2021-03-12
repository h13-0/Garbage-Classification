#include "FreeIO.h"

#include "stm32f10x.h"

//C14 C15应该是只能二选一, 13随意
void FreeC13_15(void)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE );
    PWR_BackupAccessCmd( ENABLE );/* 允许修改RTC和后备寄存器*/
    RCC_LSEConfig( RCC_LSE_OFF ); /* 关闭外部低速时钟,PC14+PC15可以用作普通IO*/
    BKP_TamperPinCmd(DISABLE);  /* 关闭入侵检测功能,PC13可以用作普通IO*/
		PWR_BackupAccessCmd(DISABLE);/* 禁止修改RTC和后备寄存器*/
/*
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          
    GPIO_Init(GPIOC, &GPIO_InitStructure);
*/
    
    //BKP_ITConfig(DISABLE);       /* 禁止TAMPER 中断*/
}

void FreeJtag(void)
{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //切换到SWJ调试，释放PA15，PB4
		DBGMCU->CR &= 0xFFFFFFDF;   //禁止异步跟踪，释放PB3
}
