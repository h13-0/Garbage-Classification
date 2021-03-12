#include "stm32f10x.h"
#include "string.h"
#include "USART1_IRQ.h"
#include "usart.h"

//Receive Buf
static uint8_t USART1_RX_BUF[USART1_BUF_LEN];     //接收缓冲, 最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
static uint16_t USART1_RX_STA = 0;       //接收状态标记	  

//ReadLine Buf

//USART1
#if USART1_READLINE
static uint8_t USART1_ReadLine_BUF[USART1_BUF_LEN];
static uint16_t USART1_ReadLine_LEN = 0;
#endif

inline void USART1_Handler(void)
{
    u8 Res;
    Res = USART_ReceiveData(USART1);	//读取接收到的数据
		//Write Your Raw APIs here.
		//

    if((USART1_RX_STA&0x8000)==0)//接收未完成
    {
        if(USART1_RX_STA&0x4000)//接收到了0x0d
        {
            if(Res!=0x0a)
                USART1_RX_STA = 0;//接收错误,重新开始
            else
            {
                USART1_RX_STA |= 0x8000;	//接收完成了
							
								#if USART1_READLINE
								memcpy(USART1_ReadLine_BUF, USART1_RX_BUF, USART1_BUF_LEN);
								USART1_ReadLine_LEN = USART1_RX_STA&0X3FFF;
								#endif
							
								//Write Your Command APIs here.
								//Command(USART1_RX_BUF, USART1_RX_STA&0X3FFF);
							
								//Clear BUFFER
								memset(USART1_RX_BUF, 0, USART1_BUF_LEN);
								USART1_RX_STA = 0;
            }
        }
        else //还没收到0X0D
        {
            if(Res == 0x0d)
								USART1_RX_STA|=0x4000;
            else
            {
                USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
                USART1_RX_STA++;
                if(USART1_RX_STA > (USART1_BUF_LEN - 1))
                {
                    USART1_RX_STA=0;//接收数据错误,重新开始接收
                    memset(USART1_RX_BUF, 0, USART1_BUF_LEN);
                }
            }
        }
    }
}

#if USART1_READLINE
uint16_t USART1_ReadLine(uint8_t* buf)
{
		if(USART1_ReadLine_LEN)
		{
				memcpy(buf, USART1_ReadLine_BUF, USART1_BUF_LEN);
				uint16_t len = USART1_ReadLine_LEN;
				USART1_ReadLine_LEN = 0;
				//返回数据长度
				return len;
		} else {
				return 0;
		}
}
#endif
