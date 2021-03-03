/*
@版本：2019-09-04:1.0
@联系：minuy@qq.com
只需要在每次串口中断中调用void Usart_Receive(unsigned char cha)函数，
把收到的值传给这个函数就好，然后可以直接读出按键、摇杆和滑块的值
*/

#include "BlueRocker.h"
#include "usart.h"

void BlueRocker_init(BlueRockerTypeDef* BlueRocker)
{
	(*BlueRocker).rockerVal = 0;
  (*BlueRocker).keyVal = 0;
  (*BlueRocker).seekBarVal = 0;
}

uint8_t Rocker_Data_Analyze(BlueRockerTypeDef* BlueRocker,uint8_t * str,uint8_t len)
{
    if(str[0] == 'y')//判断是不是指令数据
    {
        switch(str[1])
        {
        case 'd'://摇杆数据
            (*BlueRocker).rockerVal = str[2]+str[3];
            return 1;

        case 'b'://按键数据
            (*BlueRocker).keyVal = str[2];
            return 2;

        case 'v'://滑块数据
            (*BlueRocker).seekBarVal = str[2];
            return 3;
				
        default://其他命令
            return 0;
        }
    }
		return 0;
}


//串口接收函数，放在串口中断中，cha：串口每次收到的值
void Usart_Receive(BlueRockerTypeDef* BlueRocker,unsigned char cha)
{
	  printf("%c",cha);
    static unsigned char str[LEN];
    static unsigned char sta = 0;//最高两位分别标志接收到0x0A和0x0D
    if((sta&0x80)==0)//接收未完成
    {
        if(sta&0x40)//接收到了0x0A
        {
            if(cha!=0x0D)
            {
                sta=0;//接收错误,重新开始
            } else {
                sta|=0x80;//接收完成
                sta<<=2;
                sta>>=2;
                Rocker_Data_Analyze(BlueRocker,str,sta);
                sta = 0;
            }
        } else {//还没收到0X0A
            if(cha==0x0A) {
                sta|=0x40;
            } else {
                str[sta&0X3F] = cha;
                sta++;
                if(sta>(LEN-1))sta=0;//接收数据错误,重新开始接收
            }
        }
    }
}

