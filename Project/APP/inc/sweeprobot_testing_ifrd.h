#ifndef __SWEEPROBOT_TESTING_IFRD_H
#define __SWEEPROBOT_TESTING_IFRD_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 onValue;
    u16 offValue;
    u8 validCnt;
    u8 validFlag;
}IFRD_TestTypeDef;

void SweepRobot_IFRD_Test_Task(void *pdata);

#endif
