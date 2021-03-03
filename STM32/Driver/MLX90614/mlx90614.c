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

#include "mlx90614.h"
#include "SoftI2C.h"
#include "stdio.h"
#include "SystemClock.h"
/********************* define ***************************/
typedef void  (* vMLX90614_BusInitHandler)() ;

typedef uint8_t (* ucMLX90614_BusWriteHandler)(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

typedef uint8_t (* ucMLX90614_BusReadHandler)(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

/********************** var ****************************/
vMLX90614_BusInitHandler vMLX90614_BusInit = vSMBus_Init ;
ucMLX90614_BusReadHandler ucMLX90614_BusRead = ucSMBUS_StreamRead ;
ucMLX90614_BusWriteHandler ucMLX90614_BusWrite = ucSMBUS_StreamWrite ;

/* ��ʼ��MLX90614,�ɹ�1��ʧ��0�� */
uint8_t vMLX90614_Init(sMlx90614Typ *psMlx90614,uint8_t ucAddr)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	
	if ( psMlx90614 != NULL)
	{
		psMlx90614->ucAddr = ucAddr ;
		
		vMLX90614_BusInit() ;
		
		ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_SLAVE_ADDR_EADDR, ucTempBuf, 2) ;
		
		if ( psMlx90614->ucAddr == ucTempBuf[0]) ucResult = 1 ;
	}
	
	return ucResult ;
}

/* ��ȡ�����¶ȣ������ض�ȡ���,�ɹ�1��ʧ��0�� */
uint8_t ucMLX90614_GetTA(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TA_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->Ta = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* ��ȡ�����¶�1�������ض�ȡ��� */
uint8_t ucMLX90614_GetTO1(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TOBJ1_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->To1 = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* ��ȡ����2�¶ȣ������ض�ȡ��� */
uint8_t ucMLX90614_GetTO2(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TOBJ2_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->To2 = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* ���÷����ʣ������ض�ȡ��� */
uint8_t ucMLX90614_SetEmssivity(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] = {0x00,0x00} ;
//	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		/* �����Ϊʲô�˴�����ʹ��ȫ�ֵ�ַ */
		if ( ucMLX90614_BusWrite(MLX90614_SMBUS_ALLADDR, MLX90614_EMISSIVITY_COEFFICIENT_EADDR, ucTempBuf, 2))
		{
			ucTempBuf[0] = (uint8_t)( (uint16_t)( 0xFFFF * psMlx90614->Emssivity) & 0x00FF) ;
			ucTempBuf[1] = (uint8_t)( (uint16_t)( 0xFFFF * psMlx90614->Emssivity) >> 8) ;
				
			delay_ms(1000) ;
			
//			/* �����Ϊʲô�˴�����ʹ��ȫ�ֵ�ַ */
			if ( ucMLX90614_BusWrite(MLX90614_SMBUS_ALLADDR, MLX90614_EMISSIVITY_COEFFICIENT_EADDR, ucTempBuf, 2))
			{
				ucResult = 1 ;
			}
		}		
	}
	
	return ucResult ;
}

/* ���÷����ʣ������ض�ȡ��� */
float ucMLX90614_TempComp(sMlx90614Typ *psMlx90614)
{
	float Tl ;
	float Th ;
	float Tbody ;
	
	if ( psMlx90614->Ta < 25)
	{
		Tl = 32.66 + ( 0.186 * ( psMlx90614->Ta - 25)) ; 
		Th = 34.84 + ( 0.148 * ( psMlx90614->Ta - 25)) ;
	}
	else
	{
		Tl = 32.66 + ( 0.086 *( psMlx90614->Ta - 25)) ; 
		Th = 34.84 + ( 0.100 * ( psMlx90614->Ta - 25)) ;
	}
	
	if ( psMlx90614->To1 < Tl)
	{
		Tbody = 36.3 + ( ( 0.551658273 + 0.021525068 * psMlx90614->Ta) * (psMlx90614->To1 - Tl)) ;
	}
	else if ( Th < psMlx90614->To1)
	{
		Tbody = 36.8 + ( ( 0.829320618 + 0.002364434 * psMlx90614->Ta) * (psMlx90614->To1 -Th)) ;
	}
	else
	{
		Tbody = 36.3 + (0.5 / ( ( Th - Tl) * ( psMlx90614->To1 - Tl))) ;
	}
	
	return Tbody ;
}







