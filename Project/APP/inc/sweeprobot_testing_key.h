#ifndef __SWEEPROBOT_TESTING_KEY_H
#define __SWEEPROBOT_TESTING_KEY_H

#include "sweeprobot_testing.h"

typedef struct{
    u8 value;
    u8 validCnt;
    u8 validFlag;
}KEY_TestTypeDef;

void SweepRobot_Key_Test_Task(void *pdata);

#endif
