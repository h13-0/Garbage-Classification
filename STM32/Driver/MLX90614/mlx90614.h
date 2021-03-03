/***************************************************************************************************
* Copyright (C),    //版权
* File name:key.h 	
* Author:Charmander     Version:0.1    Date:2020.9.1    //作者、版本、完成日期（初版） 
* Description:    
* Other:   
* Contact: 228303760（qqname： Obligation） 228303760@qq.com
* TaoBao: 有为电子科技
* Function List:   
*			void KEY_Init(void) ; //IO初始化
*			u8 KEY_Scan(u8) ;  	  //按键扫描函数	
* History:    //修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
*      1. Data：
*         Author：Charmander
*          Modification：
***********************************************************************************************************/

#ifndef __MLX90614_H
#define __MLX90614_H	 

#include "sys.h"
/******************** Macro definitions **********************/

/********************** SMBUS从机地址 ************************/
/* 全局地址 */
#define MLX90614_SMBUS_ALLADDR 0x00
/* 默认地址 */
#define MLX90614_SMBUS_ADDR 0x5A

/******************** EEPROM地址(32*16) **********************/
/* 最大物体温度 */
#define MLX90614_TO_MAX_EADDR 								0x20
/* 最小物体温度 */
#define MLX90614_TO_MIN_EADDR 								0x21
/* PWM控制 */
#define MLX90614_PWM_CTRL_EADDR 							0x22
/* 环境温度范围 */
#define MLX90614_TA_RANGE_EADDR 							0x23
/* 发射功率 */
#define MLX90614_EMISSIVITY_COEFFICIENT_EADDR 				0x24
/* 配置寄存器 */
#define MLX90614_CONFIG_REG_EADDR 							0x25
/* SMBUS从机地址（默认0x5A） */
#define MLX90614_SLAVE_ADDR_EADDR 							0x2E
#define MLX90614_ID1_EADDR 									0x3C
#define MLX90614_ID2_EADDR 									0x3D
#define MLX90614_ID3_EADDR 									0x3E
#define MLX90614_ID4_EADDR 									0x3F

/********************** RAM地址(32*17) ***********************/
/* Raw data IR channel 1 */
#define MLX90614_RAWIR1_RADDR 								0x04
/* Raw data IR channel 2 */
#define MLX90614_RAWIR2_RADDR 								0x05
#define MLX90614_TA_RADDR 									0x06
#define MLX90614_TOBJ1_RADDR 								0x07
#define MLX90614_TOBJ2_RADDR 								0x08

/********************** READ FLAG地址(1*16) *******************/
/* 读标志位地址 */
#define MLX90614_READFLAG_ADDR                          	0xF0

/********************** SLEEP MODE地址(1*16) ******************/
#define MLX90614_SLEEP_ADDR                          		0xFF

/******************** EEPROM内容(32*16) **********************/
/* PWM拓展模式(该模式下的周期位“单模式”下的两倍) */
#define MLX90614_PWM_CTRL_EXTERNDED_MODE 					0x0000
/* PWM单模式（T = 1.024*P 毫秒，P为PWMCTRL寄存器的15-9位） */
#define MLX90614_PWM_CTRL_SINGLE_MODE 						0x0001
/* PWM失能 */
#define MLX90614_PWM_CTRL_PWM_DISEN 						0x0000
/* PWM使能 */
#define MLX90614_PWM_CTRL_PWM_EN 							0x0001
/* SDA开漏模式 */
#define MLX90614_PWM_CTRL_SDA_OD 							0x0000
/* SDA推挽模式 */
#define MLX90614_PWM_CTRL_SDA_PP 							0x0001
/* PWM模式 */
#define MLX90614_PWM_CTRL_PWM_MODE 							0x0000
/* 热继电器模式 */
#define MLX90614_PWM_CTRL_RELAY_MODE 						0x0001
/* IIR参数 100单位% */
#define MLX90614_COF_IIR_100 								0x0004
#define MLX90614_COF_IIR_80 								0x0005
#define MLX90614_COF_IIR_67 								0x0006
#define MLX90614_COF_IIR_57 								0x0007
#define MLX90614_COF_IIR_50 								0x0000
#define MLX90614_COF_IIR_25 								0x0001
#define MLX90614_COF_IIR_17 								0x0002
#define MLX90614_COF_IIR_13 								0x0003
/* RST = Repeat Sensor Test */
#define MLX90614_COF_RST_OFF 								0x0000
#define MLX90614_COF_RSR_ON     				    		0x0001
/* TATO */
#define MLX90614_COF_TATO_TATO1 							0x0000
#define MLX90614_COF_TATO_TATO2    				    		0x0001
#define MLX90614_COF_TATO_TO2 								0x0002
#define MLX90614_COF_TATO_TO1TO2    				    	0x0003
/* IR */
#define MLX90614_COF_IR_SINGLE 								0x0000
#define MLX90614_COF_IR_DUAL     				    		0x0001
/* Positive/Negative sign of Ks */
#define MLX90614_COF_SIGNOFKS_POSITIVE 						0x0000
#define MLX90614_COF_SIGNOFKS_NEGATIVE     				    0x0001
/* FIR参数 */
#define MLX90614_COF_FIR_8 									0x0000
#define MLX90614_COF_FIR_16 								0x0001
#define MLX90614_COF_FIR_32 								0x0002
#define MLX90614_COF_FIR_64 								0x0003
#define MLX90614_COF_FIR_128 								0x0004
#define MLX90614_COF_FIR_256 								0x0005
#define MLX90614_COF_FIR_512 								0x0006
#define MLX90614_COF_FIR_1024 								0x0007
/* GAIN(增益) */
#define MLX90614_COF_GAIN_1 								0x0000
#define MLX90614_COF_GAIN_3 								0x0001
#define MLX90614_COF_GAIN_6 								0x0002
#define MLX90614_COF_GAIN_12 								0x0003
#define MLX90614_COF_GAIN_25 								0x0004
#define MLX90614_COF_GAIN_50 								0x0005
#define MLX90614_COF_GAIN_100 								0x0006
/* Positive/Negative sign of Kt2 */
#define MLX90614_COF_SIGNOFKT2_POSITIVE 					0x0000
#define MLX90614_COF_SIGNOFKT2_NEGATIVE     				0x0001
/* ST = Sensor Test */
#define MLX90614_COF_ST_ENABLE								0x0000
#define MLX90614_COF_ST_DISABLE     						0x0001

/********************** READ FLAG内容(1*16) ******************/
#define MLX90614_READFLAG_EEBUSY_N 							0x0000
#define MLX90614_READFLAG_EEBUSY_Y 							0x0001

#define MLX90614_READFLAG_EEDEAD_N 							0x0000
#define MLX90614_READFLAG_EEDEAD_Y 							0x0001

#define MLX90614_READFLAG_INIT_N 							0x0001
#define MLX90614_READFLAG_INIT_Y 							0x0000
/******************** EEPROM掩码(32*16) **********************/
#define MLX90614_PWM_CTRL_MODE_MASK 						0x0001
#define MLX90614_PWM_CTRL_MODE_POS     						0
#define MLX90614_PWM_CTRL_EN_MASK 							0x0002
#define MLX90614_PWM_CTRL_EN_POS     						1
#define MLX90614_PWM_CTRL_SDAMODE_MASK 						0x0004
#define MLX90614_PWM_CTRL_SDAMODE_POS 						2
#define MLX90614_PWM_CTRL_TRPWMB_MASK 						0x0008
#define MLX90614_PWM_CTRL_TRPWMB_POS 						3
#define MLX90614_PWM_CTRL_REPNUM_MASK 						0x00F0
#define MLX90614_PWM_CTRL_REPNUM_POS 						4
#define MLX90614_COF_IIR_MASK 								0x0007
#define MLX90614_COF_IIR_POS     				    		0
/* RST = Repeat Sensor Test */
#define MLX90614_COF_RST_MASK 								0x0008
#define MLX90614_COF_RSR_POS     				    		3
#define MLX90614_COF_TATO_MASK 								0x0030
#define MLX90614_COF_TATO_POS     				    		4
#define MLX90614_COF_IR_MASK 								0x0040
#define MLX90614_COF_IR_POS     				    		6
#define MLX90614_COF_SIGNOFKS_MASK 							0x0080
#define MLX90614_COF_SIGNOFKS_POS     				    	7
#define MLX90614_COF_FIR_MASK 								0x0700
#define MLX90614_COF_FIR_POS     				    		8
#define MLX90614_COF_GAIN_MASK 								0x3100
#define MLX90614_COF_GAIN_POS     				    		11
#define MLX90614_COF_SIGNOFKT2_MASK 						0x4000
#define MLX90614_COF_SIGNOFKT2_POS     				    	14
/* ST = Sensor Test */
#define MLX90614_COF_ST_MASK 								0x8000
#define MLX90614_COF_ST_POS     				    		15

/********************** READ FLAG掩码(1*16) ********************/
#define MLX90614_READFLAG_EEBUSY_MASK 						0x0080
#define MLX90614_READFLAG_EEBUSY_POS 						7

#define MLX90614_READFLAG_EEDEAD_MASK 						0x0020
#define MLX90614_READFLAG_EEDEAD_POS 						5

#define MLX90614_READFLAG_INIT_MASK 						0x0010
#define MLX90614_READFLAG_INIT_POS 							4

/******************** 获得/设置位宏函数 **********************/
#define MLX90614_GET_BITS(reg,bitname) ( ( reg & bitname##_MASK) \
											>> bitname##_POS)

#define MLX90614_SET_BITS(reg,bitname,val) ( ( reg & ( ~bitname##_MASK)) \
											| ( ( val << bitname##_POS) \
												& bitname##_MASK))
/*
 * 函数名：sMlx90614Typ
 * 描述  ：MLX90614类名
 * 成员  ：
 *		   ucAddr 设备地址
 *		   Ta 环境温度（摄氏度）
 *		   To1 物体温度1（摄氏度）
 *		   To2 物体温度2（摄氏度）（仅在xbx系列中存在）
 *		   Emssivity 发射率 （0.1--1）
 */
typedef struct
{
	uint8_t ucAddr ;
	
	float Ta ;
	float To1 ;
	float To2 ;
	float Emssivity ;
}sMlx90614Typ ;

uint8_t vMLX90614_Init(sMlx90614Typ *psMlx90614,uint8_t ucAddr) ;
/* 获取环境温度，并返回读取结果,成功1，失败0。 */
uint8_t ucMLX90614_GetTA(sMlx90614Typ *psMlx90614) ;
/* 获取物体温度1，并返回读取结果 */
uint8_t ucMLX90614_GetTO1(sMlx90614Typ *psMlx90614) ;
/* 获取物体2温度，并返回读取结果 */
uint8_t ucMLX90614_GetTO2(sMlx90614Typ *psMlx90614) ;
/* 设置发射率，并返回读取结果 */
uint8_t ucMLX90614_SetEmssivity(sMlx90614Typ *psMlx90614) ;
float ucMLX90614_TempComp(sMlx90614Typ *psMlx90614) ;

#endif







