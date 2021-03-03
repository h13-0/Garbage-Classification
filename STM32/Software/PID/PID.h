#ifndef _PID_H_
#define _PID_H_
#include "stdint.h"
typedef struct 
{
	double setpoint;      //�趨Ŀ��
	
	double proportion ;   //��������
	double integral;      //���ֳ���
	double derivative;    //΢�ֳ���
	
	double sum_error;     //����ۼ�->���ڻ���
	
	double error;         //���һ�����->����΢��
}PIDtypedef;


void incPIDinit(void);   

int16_t incPIDcalc(PIDtypedef* PIDx,double currentpoint); 

void PID_setpoint(PIDtypedef*PIDx,unsigned short setvalue);  

#endif
