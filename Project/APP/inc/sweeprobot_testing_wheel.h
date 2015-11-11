#ifndef __SWEEPROBOT_TEST_WHEEL_H
#define __SWEEPROBOT_TEST_WHEEL_H

#include "sweeprobot_testing.h"

typedef struct{
    u8 speed;
    u8 validCnt;
    u8 validFlag;
}WHEEL_TestTypeDef;

void SweepRobot_Wheel_Test_Task(void *pdata);

#endif
