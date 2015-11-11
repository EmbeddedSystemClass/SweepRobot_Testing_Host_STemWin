#ifndef __SWEEPROBOT_TESTING_FAN_H
#define __SWEEPROBOT_TESTING_FAN_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 current;
    u8 validCnt;
    u8 validFlag;
}FAN_TestTypeDef;

void SweepRobot_Fan_Test_Task(void *pdata);

#endif
