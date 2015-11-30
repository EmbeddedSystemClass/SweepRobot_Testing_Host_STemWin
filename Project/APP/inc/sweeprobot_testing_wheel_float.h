#ifndef __SWEEPROBOT_TESTING_WHEEL_FLOAT_H
#define __SWEEPROBOT_TESTING_WHEEL_FLOAT_H

#include "stm32f4xx.h"

typedef struct{
    u8 value;
    u8 validCnt;
    u8 validFlag;
}WHEEL_FLOAT_TestTypeDef;

void SweepRobot_WheelFloatTestTask(void *pdata);
void WHEEL_FLOAT_TestDataSave(void);
void SweepRobot_WheelFloatCtrlTestPos(void);
void SweepRobot_WheelFloatCtrlIdlePos(void);
void SweepRobot_WheelFloatCtrlOff(void);

#endif
