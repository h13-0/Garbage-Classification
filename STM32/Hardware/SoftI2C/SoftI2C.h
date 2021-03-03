/***************************************************************************************************
* Copyright (C),    //版权
* File name:key.h 	
* Author:Charmander     Version:0.1    Date:2020.9.1    //作者、版本、完成日期（初版） 
* Description:    
* Other:   
* Contact: 228303760（qqname： Obligation） 228303760@qq.com
* TaoBao: 有为电子科技
* Function List:   
*			void KEY_Init(void) ; //IO初始化
*			u8 KEY_Scan(u8) ;  	  //按键扫描函数	
* History:    //修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
*      1. Data：
*         Author：Charmander
*          Modification：
***********************************************************************************************************/

#ifndef __SMBUS_H
#define __SMBUS_H	 

#include "sys.h"

/* SMBUS总线引脚资源在STM32上的映射 */
#define SMBUS_APB2Periph_GPIOx		RCC_APB2Periph_GPIOB
#define SMBUS_GPIOx 				GPIOB
#define SMBUS_SCL 					GPIO_Pin_7
#define SMBUS_SDA 					GPIO_Pin_6

/* 设置SDA引脚模式 */
#define SMBUS_SDA_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}
#define SMBUS_SDA_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}

/* 读写SMBUS引脚宏 */ 
#define SMBUS_WRITE_SCL    PBout(7) 		
#define SMBUS_WRITE_SDA    PBout(6) 			 
#define SMBUS_READ_SDA     PBin(6) 	

/*
 * 函数名：vSMBus_Init
 * 描述  ：初始化SMBUS
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 * 调用  ：无 
 * 举例  ：无
 */
void vSMBus_Init(void) ;

/* 向从机写数据流 */
uint8_t ucSMBUS_StreamWrite(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

/* 从从机读入数据流 */
uint8_t ucSMBUS_StreamRead(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

#endif


