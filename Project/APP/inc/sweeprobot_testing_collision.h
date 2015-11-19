#ifndef __SWEEPROBOT_TESTING_COLLISION_H
#define __SWEEPROBOT_TESTING_COLLISION_H

#include "sweeprobot_testing.h"

typedef struct{
    int value;
    u8 validCnt;
    u8 validFlag;
}COLLISION_TestTypeDef;

void SweepRobot_CollisionTestTask(void *pdata);
void Collision_TestDataSave(void);

#endif
