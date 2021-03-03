#ifndef _VL53L0X_IT_H_
#define _VL53L0X_IT_H_
#include "vl53l0x_easy_api.h"
//上下限距离值 单位:mm
#define Thresh_Low  60
#define Thresh_High 150

void vl53l0x_interrupt_test(VL53L0X_Dev_t *dev);


#endif
