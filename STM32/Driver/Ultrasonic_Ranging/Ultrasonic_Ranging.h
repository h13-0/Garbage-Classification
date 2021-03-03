#ifndef _ULTRASONIC_RANGING_H_
#define _ULTRASONIC_RANGING_H_

#define Trig_Port GPIOA
#define Trig_Pin  GPIO_Pin_5

#define Echo_Port GPIOA
#define Echo_Pin  GPIO_Pin_4

#define RCC_PeriphClockEnable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE)

#define Speed_Of_Sound 340.0
//#define Max_Distance   4000 //mm


void Ultrasonic_Init(void);

//虽然说Echo可以用中断 但是感觉没必要
double Get_Distance_Value(void);

#endif
