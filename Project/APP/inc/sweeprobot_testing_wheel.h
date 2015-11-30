#ifndef __SWEEPROBOT_TEST_WHEEL_H
#define __SWEEPROBOT_TEST_WHEEL_H

#include "stm32f4xx.h"

typedef struct{
    int speed;
    int validCnt;
    int validFlag;
}WHEEL_TestTypeDef;

void SweepRobot_WheelTestTask(void *pdata);
void Wheel_TestDataSave(void);

#endif
