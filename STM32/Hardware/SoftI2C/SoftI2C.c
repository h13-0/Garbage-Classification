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

#include "SoftI2C.h"
#include "SystemClock.h"

/* 初始化SMBUS */
void vSMBus_Init()
{
 	GPIO_InitTypeDef GPIO_InitStructure ;
 
 	RCC_APB2PeriphClockCmd(SMBUS_APB2Periph_GPIOx,ENABLE) ;		   

	GPIO_InitStructure.GPIO_Pin  = SMBUS_SCL|SMBUS_SDA ;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SMBUS_GPIOx, &GPIO_InitStructure) ;		

	SMBUS_WRITE_SCL = 0 ;
	SMBUS_WRITE_SDA = 0 ;
}

/* 发送SMBUS起始信号 */
static void vSMBus_Start()
{
	SMBUS_SDA_OUT() ;
	
	SMBUS_WRITE_SDA = 1 ;
	SMBUS_WRITE_SCL = 1 ;
	delay_us(5) ;
	SMBUS_WRITE_SDA = 0 ;
	delay_us(5) ;
	SMBUS_WRITE_SCL = 0 ;	
}

/* 发送SMBUS停止信号 */
static void vSMBus_Stop()
{
	SMBUS_SDA_OUT() ;
	
	SMBUS_WRITE_SCL = 0 ;
	SMBUS_WRITE_SDA = 0 ;
	delay_us(5) ;
	SMBUS_WRITE_SCL = 1 ;
	SMBUS_WRITE_SDA = 1 ;
	delay_us(5) ;
}

/* 等待从机应答信号,0应答失败，1应答成功。 */
static uint8_t ucSMBus_WaitACK()
{
	uint8_t ucFlag = 0 ;
	uint8_t ucTime = 0 ;
	
	SMBUS_SDA_IN() 
	
	SMBUS_WRITE_SDA = 1 ;
	delay_us(5) ;
	SMBUS_WRITE_SCL = 1 ;
	delay_us(5) ;
	
	/* 在5*20微秒 内等待应答信号 */
	for ( ; ucTime < 200; ucTime++)
	{
		delay_us(5) ;
		
		if ( SMBUS_READ_SDA == 0)
		{
			break ;
		}
	}
	
	if ( ucTime >= 200)
	{
		vSMBus_Stop() ;
		ucFlag = 0 ;	
	}
	else
		ucFlag = 1 ;
	
	SMBUS_WRITE_SCL = 0 ;
	
	return ucFlag ;
}

/* 向发送SMBUS从机发送非应答信号 */
/* 手册上说应答信号是SDA为0，但是实际测试中确是1。*/
//static void vSMBus_SendNAck()
//{	
//	SMBUS_WRITE_SCL = 0 ;
//	SMBUS_SDA_OUT() ;
//	SMBUS_WRITE_SDA = 0 ; 
//	delay_us(5) ;
//	SMBUS_WRITE_SCL = 1 ;
//	delay_us(5) ;
//	SMBUS_WRITE_SCL = 0 ;
//}
	
/* 向发送SMBUS从机发送应答信号 */
/* 手册上说应答信号是SDA为0，但是实际测试中确是1。*/
static void vSMBus_SendAck()
{
	SMBUS_WRITE_SCL = 0 ;
	SMBUS_SDA_OUT() ;
	SMBUS_WRITE_SDA = 1 ; 
	delay_us(5) ;
	SMBUS_WRITE_SCL = 1 ;
	delay_us(5) ;
	SMBUS_WRITE_SCL = 0 ;	
}

/* 向SMBUS从机发送一个字节，0发送失败，1发送成功 */
static uint8_t ucSMBus_SendByte(uint8_t ucByte)
{
	uint8_t i = 0 ;
	uint8_t ucFlag = 0 ;
	
    SMBUS_SDA_OUT() ; 
	
    SMBUS_WRITE_SCL = 0 ;
	
    for ( ; i < 8 ; i++)
    {              
        SMBUS_WRITE_SDA = ( ucByte & 0x80) >> 7 ;	
        ucByte <<= 1 ; 	 
		
        SMBUS_WRITE_SCL = 1 ;
        delay_us(5) ;
		SMBUS_WRITE_SCL = 0 ;	
        delay_us(5) ;
    }	
	
    ucFlag = ucSMBus_WaitACK() ;
	
	return ucFlag ;
}

/* 从SMBUS从机读一个字节 */
static uint8_t ucSMBus_ReadByte()
{
	uint8_t i = 0 ;
	uint8_t ucByte = 0 ;
	
	SMBUS_SDA_IN() ;
	
    for ( ; i < 8; i++)
	{
        SMBUS_WRITE_SCL=0; 
        delay_us(5) ;
		SMBUS_WRITE_SCL=1;
        ucByte <<= 1 ;
		
        if( SMBUS_READ_SDA)
			ucByte++ ;   
		
		delay_us(5) ; 
    }
	
	vSMBus_SendAck() ;
 
    return ucByte ;
}

static uint8_t ucSMBUS_CRC8(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength)
{
	uint8_t crc ;
	uint8_t i ;
	
    crc = 0 ;

    crc ^= ( ucSlaveAddr << 1) ;
    for ( i = 0; i < 8; i++)
    {
		if (crc & 0x80)
	    {
		    crc = ( crc << 1) ^ 0x07 ;
	    }
	    else 
	    {
		    crc <<= 1 ;
	    }
    }
	
	crc ^= ucRegAddr ;
    for ( i = 0; i < 8; i++)
    {
		if (crc & 0x80)
	    {
		    crc = ( crc << 1) ^ 0x07 ;
	    }
	    else 
	    {
		    crc <<= 1 ;
	    }
    }
	
    while(uiRegDataBufLength--)
    {
       crc ^= *pucRegDataBuf++ ;
       for ( i = 0; i < 8; i++)
       {
           if (crc & 0x80)
           {
               crc = ( crc << 1) ^ 0x07 ;
           }
           else 
		   {
			   crc <<= 1 ;
		   }
       }
    }
	
    return crc ;
}

/* 向从机写数据流 */
uint8_t ucSMBUS_StreamWrite(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength)
{
	uint8_t i ;
    uint8_t ucCrc ;
	
	/* 计算CRC8 */
	ucCrc = ucSMBUS_CRC8(ucSlaveAddr,ucRegAddr,pucRegDataBuf,uiRegDataBufLength) ;
	
    vSMBus_Start() ;    
	
	if ( ucSMBus_SendByte( ( ucSlaveAddr << 1) | 0) == 0)	return 0 ;		
   
    if ( ucSMBus_SendByte(ucRegAddr) == 0)	return 0 ;		
	
	for ( i = 0; i < uiRegDataBufLength; i++)
	{
		if ( ucSMBus_SendByte(pucRegDataBuf[i]) == 0) return 0 ;			
	}    
    
	/* 发送CRC8 */
	if ( ucSMBus_SendByte(ucCrc) == 0)	return 0 ;
	
    vSMBus_Stop() ;	 
	
    return 1 ;	   
}

/* 从从机读入数据流 */
uint8_t ucSMBUS_StreamRead(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength)
{
    uint8_t i ;
		//uint8_t crc ;
	
 	vSMBus_Start() ; 
	
	if ( ucSMBus_SendByte( ( ucSlaveAddr << 1) | 0) == 0)	return 0 ;
	
	if ( ucSMBus_SendByte( ucRegAddr) == 0)	return 0 ;
    
    vSMBus_Start() ;

    if ( ucSMBus_SendByte( ( ucSlaveAddr << 1) | 1) == 0)	return 0 ;
	
    for ( i = uiRegDataBufLength; i > 0; i-- )
    {
        *pucRegDataBuf = ucSMBus_ReadByte() ;		                        	
        pucRegDataBuf++ ;
    }
    
	/* 读CRC8 */
	ucSMBus_ReadByte() ;                                  
	
    vSMBus_Stop() ;	                                                            
	
	return 1 ;
}



