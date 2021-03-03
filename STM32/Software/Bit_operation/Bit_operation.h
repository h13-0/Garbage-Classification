#ifndef _BITOPERATION_H_
#define _BITOPERATION_H_

//获取变量var的第n位
#define GET8BIT0(var) (var&0x80)
#define GET8BIT1(var) (var&0x40)
#define GET8BIT2(var) (var&0x20)
#define GET8BIT3(var) (var&0x10)
#define GET8BIT4(var) (var&0x08)
#define GET8BIT5(var) (var&0x04)
#define GET8BIT6(var) (var&0x02)
#define GET8BIT7(var) (var&0x01)

//设置变量var的第n位为1
#define SET8BIT0(var) (var|=0x80)
#define SET8BIT1(var) (var|=0x40)
#define SET8BIT2(var) (var|=0x20)
#define SET8BIT3(var) (var|=0x10)
#define SET8BIT4(var) (var|=0x08)
#define SET8BIT5(var) (var|=0x04)
#define SET8BIT6(var) (var|=0x02)
#define SET8BIT7(var) (var|=0x01)

//设置变量var的第n位为0
#define RESET8BIT0(var) (var&=0x7f)
#define RESET8BIT1(var) (var&=0xbf)
#define RESET8BIT2(var) (var&=0xdf)
#define RESET8BIT3(var) (var&=0xef)
#define RESET8BIT4(var) (var&=0xf7)
#define RESET8BIT5(var) (var&=0xfb)
#define RESET8BIT6(var) (var&=0xfd)
#define RESET8BIT7(var) (var&=0xfe)

#endif
