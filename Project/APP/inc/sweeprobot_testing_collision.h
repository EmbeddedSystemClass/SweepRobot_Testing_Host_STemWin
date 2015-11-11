#ifndef __SWEEPROBOT_TESTING_COLLISION_H
#define __SWEEPROBOT_TESTING_COLLISION_H

#include "sweeprobot_testing.h"

typedef struct{
    u8 value;
    u8 validCnt;
    u8 validFlag;
}COLLISION_TestTypeDef;

void SweepRobot_Collision_Test_Task(void *pdata);

#endif
