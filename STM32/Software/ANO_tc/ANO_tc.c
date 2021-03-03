/*************************************************************/
//������λ��V6ͨ��Э��
//
//��������Ϊ֡��;
//V6�ٷ�Э���ֲ�&��λ�����ص�ַ:
//  
//
//Ŀǰ��֧�ֵ�֡����:
//  ANO_SENSER
//
//Ŀǰ��δ֧�ֵ�֡����:
//
//Э����µ�ַ:
//  ���˲���:www.h13studio.com
//  QQȺ:    592543194
//
//stm32���̼ܹ�&���ļ��ϼ����µ�ַͬ��;
//
//����:h13
//����:2020-05-04
/*************************************************************/
#include "ANO_tc.h"

/*************************************************************/
//��������: putchar
//����:
//  USART_TypeDef * USARTx -> ����ָ��Ŀ�괮�ں�
//  uint8_t  cha           -> һ�ֽ�����
//
//����:
//  ��һ�ֽ������ϴ�����
//����ֵ:
//  ��
/*************************************************************/
void putchar(USART_TypeDef * USARTx,uint8_t cha)
{
	  USART_SendData(USARTx, cha);//�򴮿�1��������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

/*************************************************************/
//��������: putdata
//����:
//  USART_TypeDef * USARTx -> ����ָ��Ŀ�괮�ں�
//  uint8_t* head          -> ���溬�� "������" �� "���ݳ���" ��ָ��
//  uint8_t* data          -> ���溬�� "����" ��ָ��
//  uint8_t len            -> dataָ��ָ���ڴ�ĳ���
//
//����:
//  ��������ϵ��������β��ϴ�����
//����ֵ:
//  ��
/*************************************************************/
void putdata(USART_TypeDef * USARTx,uint8_t* head,uint8_t* data,uint8_t len)
{
	  uint8_t t=0,sum=(uint8_t)(0xAA+S_ADDR+D_ADDR);
	  putchar(USARTx,0xAA);  //����֡ͷ
	  putchar(USARTx,S_ADDR);//�����豸
	  putchar(USARTx,D_ADDR);//Ŀ���豸
	  
	  for(t=0;t<2;t++)
	  {
			  putchar(USARTx,head[t]);
			  sum += head[t];
	  }
	  
	  for(t=0;t<len;t++)
	  {
	      putchar(USARTx,data[t]);
			  sum += data[t];
	  }
		putchar(USARTx,sum);      //У���
}

/*************************************************************/
//��������: ANO_SENSER
//����:
//  USART_TypeDef * USARTx            -> ����ָ��Ŀ�괮�ں�
//  ANO_SENSER_Typedef ANO_SENSER_MSG -> ���溬�� "������" "���ݳ���" "����" ��ָ��
//
//����:
//  �ϴ�����������
//����ֵ:
//  ��
/*************************************************************/
void ANO_SENSER(USART_TypeDef * USARTx,ANO_SENSER_Typedef ANO_SENSER_MSG)
{
    uint8_t head[2];        //������*1+���ݳ���*1; ����*18
	
    head[0] = 0x02;          //������
	  head[1] = 18;            //���ݳ��� 2*9=18
		
	  putdata(USARTx,head,(uint8_t *)(&ANO_SENSER_MSG),18);
}

