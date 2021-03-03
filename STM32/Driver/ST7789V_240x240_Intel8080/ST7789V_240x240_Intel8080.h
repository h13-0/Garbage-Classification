#ifndef _240x240_8Bit_h
#define _240x240_8Bit_h

#include "sys.h"
#include "stm32f10x_gpio.h"

#define RD PBout(11)
#define WR PBout(12)
#define RS PBout(13)
#define CS PBout(14)
#define RST PBout(15)

//#define DBL GPIOA -> ODR;

#define ROW 240
#define COL 240

//用8*16字体有点小
//#define CHAR_FONT_W8_H16
//#define FONT_W 8
//#define FONT_H 16

//16*21应该当标题
#define CHAR_FONT_W16_H21
#define FONT_W 16
#define FONT_H 21



#define STRING_FUNCTION
#define EXTEND_TEST

//RRRRRGGGGGGBBBBB
#define BLACK 	0x0000
#define WHITE 	0xffff
#define GREEN 	0x07e0
#define BLUE 		0x001f
#define GRAY50 	0x8410


//unsigned char ascii[];

void LCD_Init(void);
void DispFrame(void);
void DispBand(void);
void DispColor(unsigned int color);
void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);

#ifdef EXTEND_TEST
void DispScaleHor2(void);
void DispGrayHor32(void);
void DispGrayHor16(void);
void DispScaleVer_Blue(void);
#endif
#endif
