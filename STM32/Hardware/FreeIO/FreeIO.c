#include "FreeIO.h"

#include "stm32f10x.h"

//C14 C15Ӧ����ֻ�ܶ�ѡһ, 13����
void FreeC13_15(void)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE );
    PWR_BackupAccessCmd( ENABLE );/* �����޸�RTC�ͺ󱸼Ĵ���*/
    RCC_LSEConfig( RCC_LSE_OFF ); /* �ر��ⲿ����ʱ��,PC14+PC15����������ͨIO*/
    BKP_TamperPinCmd(DISABLE);  /* �ر����ּ�⹦��,PC13����������ͨIO*/
		PWR_BackupAccessCmd(DISABLE);/* ��ֹ�޸�RTC�ͺ󱸼Ĵ���*/
/*
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          
    GPIO_Init(GPIOC, &GPIO_InitStructure);
*/
    
    //BKP_ITConfig(DISABLE);       /* ��ֹTAMPER �ж�*/
}

void FreeJtag(void)
{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //�л���SWJ���ԣ��ͷ�PA15��PB4
		DBGMCU->CR &= 0xFFFFFFDF;   //��ֹ�첽���٣��ͷ�PB3
}
