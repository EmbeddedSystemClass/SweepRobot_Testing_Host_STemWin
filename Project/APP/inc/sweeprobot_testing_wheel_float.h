#ifndef __SWEEPROBOT_TESTING_WHEEL_FLOAT_H
#define __SWEEPROBOT_TESTING_WHEEL_FLOAT_H

#include "sweeprobot_testing.h"

typedef struct{
    u8 value;
    u8 validCnt;
    u8 validFlag;
}WHEEL_FLOAT_TestTypeDef;

void SweepRobot_WheelFloatTestTask(void *pdata);
void WHEEL_FLOAT_TestDataSave(void);

#endif
