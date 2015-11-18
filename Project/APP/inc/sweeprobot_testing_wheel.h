#ifndef __SWEEPROBOT_TEST_WHEEL_H
#define __SWEEPROBOT_TEST_WHEEL_H

#include "sweeprobot_testing.h"

typedef struct{
    int speed;
    int validCnt;
    int validFlag;
}WHEEL_TestTypeDef;

void SweepRobot_Wheel_Test_Task(void *pdata);
void Wheel_TestDataSave(void);

#endif
