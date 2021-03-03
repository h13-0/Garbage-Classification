/*************************************************************/
//匿名上位机V6通信协议
//
//函数名即为帧名;
//V6官方协议手册&上位机下载地址:
//  
//
//目前已支持的帧名称:
//  ANO_SENSER
//
//目前尚未支持的帧名称:
//
//协议更新地址:
//  个人博客:www.h13studio.com
//  QQ群:    592543194
//
//stm32工程架构&库文件合集更新地址同上;
//
//作者:h13
//日期:2020-05-04
/*************************************************************/
#include "ANO_tc.h"

/*************************************************************/
//函数名称: putchar
//参数:
//  USART_TypeDef * USARTx -> 用于指定目标串口号
//  uint8_t  cha           -> 一字节数据
//
//作用:
//  将一字节数据上传串口
//返回值:
//  无
/*************************************************************/
void putchar(USART_TypeDef * USARTx,uint8_t cha)
{
	  USART_SendData(USARTx, cha);//向串口1发送数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
}

/*************************************************************/
//函数名称: putdata
//参数:
//  USART_TypeDef * USARTx -> 用于指定目标串口号
//  uint8_t* head          -> 储存含有 "功能字" 和 "数据长度" 的指针
//  uint8_t* data          -> 储存含有 "数据" 的指针
//  uint8_t len            -> data指针指向内存的长度
//
//作用:
//  将处理完毕的数据修饰并上传串口
//返回值:
//  无
/*************************************************************/
void putdata(USART_TypeDef * USARTx,uint8_t* head,uint8_t* data,uint8_t len)
{
	  uint8_t t=0,sum=(uint8_t)(0xAA+S_ADDR+D_ADDR);
	  putchar(USARTx,0xAA);  //发送帧头
	  putchar(USARTx,S_ADDR);//发送设备
	  putchar(USARTx,D_ADDR);//目标设备
	  
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
		putchar(USARTx,sum);      //校验和
}

/*************************************************************/
//函数名称: ANO_SENSER
//参数:
//  USART_TypeDef * USARTx            -> 用于指定目标串口号
//  ANO_SENSER_Typedef ANO_SENSER_MSG -> 储存含有 "功能字" "数据长度" "数据" 的指针
//
//作用:
//  上传传感器数据
//返回值:
//  无
/*************************************************************/
void ANO_SENSER(USART_TypeDef * USARTx,ANO_SENSER_Typedef ANO_SENSER_MSG)
{
    uint8_t head[2];        //功能字*1+数据长度*1; 数据*18
	
    head[0] = 0x02;          //功能字
	  head[1] = 18;            //数据长度 2*9=18
		
	  putdata(USARTx,head,(uint8_t *)(&ANO_SENSER_MSG),18);
}

