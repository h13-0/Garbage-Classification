#ifndef holder_h
#define holder_h

#include "PID.h"

//Ӳ������
//TIM3_Channel1 -> Row -> һ�����
//TIM3_Channel2 -> Col -> �������

//�궨������
#define Permissible_Error 0.5  //��д�������ĽǶȾ���ֵ

//APIs

/*******************************************************************/
//*����* ��̨PWM IO��ʼ������:
//����:
//  ��
//
//����:
//  ��ʼ����ʱ����������
//
//��ע:
//  ��̨ˮƽ��ת����ӵ�TIM3_Channel1
//  ��̨��ֱ��ת����ӵ�TIM3_Channel2
/*******************************************************************/
void HolderIOinit(void);


/*******************************************************************/
//*��ѡ* ��̨PID��ʼ������:
//����:
//  ��
//
//����:
//  ��ʼ����̨����PID ���ѡ���ٶȿ��Ʒ�����Ҫ
/*******************************************************************/
void HolderPIDinit(void);


/*******************************************************************/
//��̨�ٶȿ��ƺ���:
//����:
//  int16_t target; ��Чֵ:-1000 ~ 1000
//
//����:
//  ������̨�ٶ�
/*******************************************************************/
void setRowSpeed(int16_t target);
void setColSpeed(int16_t target);


/*******************************************************************/
//��̨�Ƕȿ��ƺ���:
//����:
//  double target; ��Чֵ: -180 ~ 180
//
//����:
//  ������̨�Ƕ�
/*******************************************************************/
void setRowAngle(double target);
void setColAngle(double target);


/*******************************************************************/
//*��ѡ* ��̨����ʵ�ֺ���
//����:
//  CurrentRow -> ��ǰˮƽ�Ƕ� ��MPU6050��ò�����
//  CurrentCol -> ��ǰ��ֱ�Ƕ� ��MPU6050��ò�����
//
//��ע:
//  ������Ҫ������̨�˶����߱�����̬ʱ **��100hz��Ƶ��** ���ô˺���
/*******************************************************************/
void executeHolder(double CurrentCol,double CurrentRow);

//Debug APIs
void setColP(double CP);
void setColI(double CI);
void setColD(double CD);

void setRowP(double RP);
void setRowI(double RI);
void setRowD(double RD);

#endif