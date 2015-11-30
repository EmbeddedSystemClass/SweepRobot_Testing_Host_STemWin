#ifndef __SWEEPROBOT_TESTING_COLLISION_H
#define __SWEEPROBOT_TESTING_COLLISION_H

#include "stm32f4xx.h"

typedef struct{
    int value;
    u8 validCnt;
    u8 validFlag;
}COLLISION_TestTypeDef;

void SweepRobot_CollisionTestTask(void *pdata);
void Collision_TestDataSave(void);
void SweepRobot_CollisionCtrlOn(void);
void SweepRobot_CollisionCtrlOff(void);

#endif
