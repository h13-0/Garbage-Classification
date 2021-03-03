/*
@�汾��2019-09-04:1.0
@��ϵ��minuy@qq.com
ֻ��Ҫ��ÿ�δ����ж��е���void Usart_Receive(unsigned char cha)������
���յ���ֵ������������ͺã�Ȼ�����ֱ�Ӷ���������ҡ�˺ͻ����ֵ
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
    if(str[0] == 'y')//�ж��ǲ���ָ������
    {
        switch(str[1])
        {
        case 'd'://ҡ������
            (*BlueRocker).rockerVal = str[2]+str[3];
            return 1;

        case 'b'://��������
            (*BlueRocker).keyVal = str[2];
            return 2;

        case 'v'://��������
            (*BlueRocker).seekBarVal = str[2];
            return 3;
				
        default://��������
            return 0;
        }
    }
		return 0;
}


//���ڽ��պ��������ڴ����ж��У�cha������ÿ���յ���ֵ
void Usart_Receive(BlueRockerTypeDef* BlueRocker,unsigned char cha)
{
	  printf("%c",cha);
    static unsigned char str[LEN];
    static unsigned char sta = 0;//�����λ�ֱ��־���յ�0x0A��0x0D
    if((sta&0x80)==0)//����δ���
    {
        if(sta&0x40)//���յ���0x0A
        {
            if(cha!=0x0D)
            {
                sta=0;//���մ���,���¿�ʼ
            } else {
                sta|=0x80;//�������
                sta<<=2;
                sta>>=2;
                Rocker_Data_Analyze(BlueRocker,str,sta);
                sta = 0;
            }
        } else {//��û�յ�0X0A
            if(cha==0x0A) {
                sta|=0x40;
            } else {
                str[sta&0X3F] = cha;
                sta++;
                if(sta>(LEN-1))sta=0;//�������ݴ���,���¿�ʼ����
            }
        }
    }
}

