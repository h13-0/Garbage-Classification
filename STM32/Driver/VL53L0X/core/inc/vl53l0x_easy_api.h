#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_it.h"
#include "sys.h"
#include "SystemClock.h"

//VL53L0X�������ϵ�Ĭ��IIC��ַΪ0X52(���������λ)
#define VL53L0X_Addr 0x52

//SDA SCL������ü�vl53l0x_i2c.h

//Xshut(Rst)�������
#define RCC_Enable() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE)
#define Xshut_Port GPIOB
#define Xshut_Pin  GPIO_Pin_5
#define VL53L0X_Xshut PBout(5)

//ʹ��2.8V IO��ƽģʽ
#define USE_I2C_2V8  1

//����ģʽ
#define Default_Mode   0// Ĭ��
#define HIGH_ACCURACY  1//�߾���
#define LONG_RANGE     2//������
#define HIGH_SPEED     3//����

//APIs
//��ʼ��
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//��ʼ��vl53l0x

//���ò���ģʽ
//'mode' �ο����� '����ģʽ'
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode);

//���ξ������(�꺯��)
#define vl53l0x_start_single_test(dev,pdata) VL53L0X_PerformSingleRangingMeasurement(dev, pdata)

//У׼
#define adjust_num 5//У׼�������
//У׼����:
//ÿ��ģ��һ��ʱ����У׼һ�μ���, У׼��Ϻ�ѱ����������ն˷���ֵд������м��ɡ�
//�²�һ��8��������Ҫ�ٴ�У׼
VL53L0X_Error vl53l0x_adjust(VL53L0X_Dev_t *dev);

//vl53l0x��λ����
//dev:�豸I2C�����ṹ��
void vl53l0x_reset(VL53L0X_Dev_t *dev);

#endif
