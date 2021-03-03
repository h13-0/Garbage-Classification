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

#ifndef __SMBUS_H
#define __SMBUS_H	 

#include "sys.h"

/* SMBUS����������Դ��STM32�ϵ�ӳ�� */
#define SMBUS_APB2Periph_GPIOx		RCC_APB2Periph_GPIOB
#define SMBUS_GPIOx 				GPIOB
#define SMBUS_SCL 					GPIO_Pin_7
#define SMBUS_SDA 					GPIO_Pin_6

/* ����SDA����ģʽ */
#define SMBUS_SDA_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}
#define SMBUS_SDA_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}

/* ��дSMBUS���ź� */ 
#define SMBUS_WRITE_SCL    PBout(7) 		
#define SMBUS_WRITE_SDA    PBout(6) 			 
#define SMBUS_READ_SDA     PBin(6) 	

/*
 * ��������vSMBus_Init
 * ����  ����ʼ��SMBUS
 * ����  ����
 * ���  ����
 * ����  ����
 * ����  ���� 
 * ����  ����
 */
void vSMBus_Init(void) ;

/* ��ӻ�д������ */
uint8_t ucSMBUS_StreamWrite(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

/* �Ӵӻ����������� */
uint8_t ucSMBUS_StreamRead(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

#endif


