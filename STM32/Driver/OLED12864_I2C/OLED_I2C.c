#include "OLED_I2C.h"
#include "SystemClock.h"
#include "codetab.h"
#include "SystemClock.h"

#define timelimit 200
uint8_t errtime = 0;

void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);//使用I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//700K，峰值850K，对应刷新率73，800K能稳定,700K对应63.5

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
	errtime = 0;
	
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}
	
	errtime = 0;
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//EV5,主模式
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}

	errtime = 0;
	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}

	errtime = 0;
	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}

	I2C_SendData(I2C1, data);//发送数据
	errtime = 0;
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}


//--------------------------------------------------------------
// Prototype      : void I2C_WriteData(uint8_t addr,uint8_t *data,unsigned char length);
// Calls          : 
// Parameters     : addr,寄存器地址，表示是命令包还是数据包;*data,数据包数组;length,数据包长度
// Description    : 连续写数据
//--------------------------------------------------------------

void I2C_WriteData(uint8_t addr,uint8_t *data,unsigned char length)
{
	unsigned char t = 0;
	errtime = 0;
	
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}
	
	errtime = 0;
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//EV5,主模式
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}

	errtime = 0;
	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}

	errtime = 0;
	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		errtime ++;
		if(errtime > timelimit)
		{
			errfunction();
			return;
		}
	}
	
	
	while(t < length)
	{
		I2C_SendData(I2C1, data[t]);//发送数据
		errtime = 0;
	  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//等待应答位
		{
			errtime ++;
		  if(errtime > timelimit)
		  {
			  errfunction();
			  return;
		  }
		}
		t++;
	}
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}



void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_WriteByte(0x00, I2C_Command);     //0x00对应二进制为0000000 即写命令时 第9位为0
}

void OLED_Init(void)
{
	delay_ms(100); //这里的延时很重要
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char n,fill[128];
	for(n = 0;n < 128;n ++)
	  fill[n] = fill_Data;
	for(n=0;n<8;n++)
	{
	  WriteCmd(0xb0+n);		//page0-page1
		WriteCmd(0x00);		//low column start address
	  WriteCmd(0x10);		//high column start address
	  OLED_SetPos(0,n);
	  I2C_WriteData(0x40,fill,128);
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[],unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j0 = 0,j1 = 0,x0 = x,x1 = x,y0,t0 = 0,str0[128];//一行最多放下128个像素，因为Keil不支持变长数组，所以很恶心....
	switch(TextSize)
	{
		case 1:                    //6*8，6个字节
		{
			while(1)
			{
				if(x0 > 122)                     //检测是否换行
				{
					OLED_SetPos(x,y);
				  I2C_WriteData(0x40,str0,t0);
					t0 = 0;
					x = 0;
					x0 = 0;
					y++;
					if (y > 7)
						return;                      //超过屏幕限制 退出函数
				}
				if(ch[j0] == '\0')               //检测是否输出完毕
				{
					if (t0 != 0)
					{
						OLED_SetPos(x,y);
					  I2C_WriteData(0x40,str0,t0);
					}	
					return;                        //输出完毕 退出函数
				}
				c = ch[j0] - 32;
				for(i=0;i<6;i++)
				{
				  str0[t0++] = F6x8[c][i];
				}
				x0 += 6;
				j0++;
			}
		}
		case 2:                    //16*8,对于16*8的，如果想一次性输出并且不想再多创建一个数组 只能牺牲代码简洁性了。写这一个case的代码的时候我心态也是崩溃的...
		{
			/*先检测是否需要换行*/
		  if(x > 118)
			{
					y += 2;
				  x = 0;
				  x0 = 0;
				  x1 = 0;
				  if (y > 6)
						return;                    //超过屏幕范围 结束函数
			}					
			
			y0 = y+1;
			while(1)
			{
				/*先处理上半部分*/
				c = ch[j0] - 32;
				for(i=0;i<8;i++)
				  str0[t0++] = F8x16[c][i];     //将字体上半部分打包
				j0++;
				x0 += 8;
				
			  if(x0 > 118)                      //检测是否需要换行
				{
					OLED_SetPos(x,y);
				  I2C_WriteData(0x40,str0,t0);
					t0 = 0;    
          x0 = 0;					
					y += 2;
					if (y > 6)
					  goto a;
				}

				if(ch[j0] == '\0')               //检测上半部分是否输出完毕
			  {
					a:
				  if (t0 != 0)
				  {
				  	OLED_SetPos(x,y);
				    I2C_WriteData(0x40,str0,t0);
						t0 = 0;
				  }	
				  
					/*开始输出下半部分*/
					while(1)
					{
					  c = ch[j1] - 32;
				    for(i=0;i<8;i++)
				      str0[t0++] = F8x16[c][i+8];     //将字体下半部分打包
				    j1++;
				    x1 += 8;
						
						if(x1 > 118)                      //检测是否需要换行
			    	{
					    OLED_SetPos(x,y0);
				      I2C_WriteData(0x40,str0,t0);
				    	t0 = 0;   
              x1 = 0;							
				    	y0 += 2;
							if(y0 > 7)
								return;
				    }
						
						if(ch[j1] == '\0') 
						{
							if (t0 != 0)
				      {
				  	    OLED_SetPos(x,y0);
				        I2C_WriteData(0x40,str0,t0);
				      }
							return;
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0,t = 0,str0[16];
	unsigned int  adder=32*N;
	
	for(wm = 0;wm < 16;wm++)
	{
		str0[t++]=F16x16[adder];
		adder += 1;
	}
	OLED_SetPos(x , y);
	I2C_WriteData(0x40,str0,t);
	
	t = 0;
	for(wm = 0;wm < 16;wm++)
	{
		str0[t++]=F16x16[adder];
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	I2C_WriteData(0x40,str0,t);
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned char x,y;
	x = x1 - x0;
	for(y=y0;y<y1;y++)
	{
	  OLED_SetPos(x0,y);
	  I2C_WriteData(0x40,BMP,x);
		BMP += x;
	}
}

//--------------------------------------------------------------
// Prototype      : void errfunction();
// Calls          : 
// Parameters     : 
// Description    : 检测到显示屏错误后自动重启显示屏(不重启单片机)。即使屏幕掉线再插上也可以正常使用
//--------------------------------------------------------------
void errfunction()
{
	delay_ms(500);
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();
}
