#ifndef __SWEEPROBOT_TESTING_UNIWHEEL_H
#define __SWEEPROBOT_TESTING_UNIWHEEL_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 onValue;
    u16 offValue;
    u8 validCnt;
    u8 validFlag;
}UNIWHEEL_TestTypeDef;

void SweepRobot_UniWheel_Test_Task(void *pdata);

#endif
