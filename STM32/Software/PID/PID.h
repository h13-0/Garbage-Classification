#ifndef _PID_H_
#define _PID_H_
#include "stdint.h"
typedef struct 
{
	double setpoint;      //设定目标
	
	double proportion ;   //比例常数
	double integral;      //积分常数
	double derivative;    //微分常数
	
	double sum_error;     //误差累计->用于积分
	
	double error;         //最近一次误差->用于微分
}PIDtypedef;


void incPIDinit(void);   

int16_t incPIDcalc(PIDtypedef* PIDx,double currentpoint); 

void PID_setpoint(PIDtypedef*PIDx,unsigned short setvalue);  

#endif
