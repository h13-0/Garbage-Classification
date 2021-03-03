/***************************************************************************************************
* Copyright (C),    //��Ȩ
* File name:key.h 	
* Author:Charmander     Version:0.1    Date:2020.9.1    //���ߡ��汾��������ڣ����棩 
* Description:    
* Other:   
* Contact: 228303760��qqname�� Obligation�� 228303760@qq.com
* TaoBao: ��Ϊ���ӿƼ�
* Function List:   
*			void KEY_Init(void) ; //IO��ʼ��
*			u8 KEY_Scan(u8) ;  	  //����ɨ�躯��	
* History:    //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
*      1. Data��
*         Author��Charmander
*          Modification��
***********************************************************************************************************/

#include "SoftI2C.h"
#include "SystemClock.h"

/* ��ʼ��SMBUS */
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

/* ����SMBUS��ʼ�ź� */
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

/* ����SMBUSֹͣ�ź� */
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

/* �ȴ��ӻ�Ӧ���ź�,0Ӧ��ʧ�ܣ�1Ӧ��ɹ��� */
static uint8_t ucSMBus_WaitACK()
{
	uint8_t ucFlag = 0 ;
	uint8_t ucTime = 0 ;
	
	SMBUS_SDA_IN() 
	
	SMBUS_WRITE_SDA = 1 ;
	delay_us(5) ;
	SMBUS_WRITE_SCL = 1 ;
	delay_us(5) ;
	
	/* ��5*20΢�� �ڵȴ�Ӧ���ź� */
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

/* ����SMBUS�ӻ����ͷ�Ӧ���ź� */
/* �ֲ���˵Ӧ���ź���SDAΪ0������ʵ�ʲ�����ȷ��1��*/
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
	
/* ����SMBUS�ӻ�����Ӧ���ź� */
/* �ֲ���˵Ӧ���ź���SDAΪ0������ʵ�ʲ�����ȷ��1��*/
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

/* ��SMBUS�ӻ�����һ���ֽڣ�0����ʧ�ܣ�1���ͳɹ� */
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

/* ��SMBUS�ӻ���һ���ֽ� */
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

/* ��ӻ�д������ */
uint8_t ucSMBUS_StreamWrite(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength)
{
	uint8_t i ;
    uint8_t ucCrc ;
	
	/* ����CRC8 */
	ucCrc = ucSMBUS_CRC8(ucSlaveAddr,ucRegAddr,pucRegDataBuf,uiRegDataBufLength) ;
	
    vSMBus_Start() ;    
	
	if ( ucSMBus_SendByte( ( ucSlaveAddr << 1) | 0) == 0)	return 0 ;		
   
    if ( ucSMBus_SendByte(ucRegAddr) == 0)	return 0 ;		
	
	for ( i = 0; i < uiRegDataBufLength; i++)
	{
		if ( ucSMBus_SendByte(pucRegDataBuf[i]) == 0) return 0 ;			
	}    
    
	/* ����CRC8 */
	if ( ucSMBus_SendByte(ucCrc) == 0)	return 0 ;
	
    vSMBus_Stop() ;	 
	
    return 1 ;	   
}

/* �Ӵӻ����������� */
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
    
	/* ��CRC8 */
	ucSMBus_ReadByte() ;                                  
	
    vSMBus_Stop() ;	                                                            
	
	return 1 ;
}



