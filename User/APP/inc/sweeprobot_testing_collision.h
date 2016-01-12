#ifndef __SWEEPROBOT_TESTING_COLLISION_H
#define __SWEEPROBOT_TESTING_COLLISION_H

#include "stm32f4xx.h"

#define SWRB_COLLISION_CHAN_NUM  4

enum CollisionChan{
    COLLISION_CHAN_L,
    COLLISION_CHAN_FL,
    COLLISION_CHAN_R,
    COLLISION_CHAN_FR,
    COLLISION_CHAN_ALL,
};

typedef struct{
    int onValue;
    int offValue;
    u8 onValidCnt;
    u8 offValidCnt;
    u8 onValidFlag;
    u8 validFlag;
}COLLISION_TestTypeDef;

void SweepRobot_CollisionTestTask(void *pdata);
void Collision_TestDataSave(void);
void SweepRobot_CollisionRelayCtrlOn(enum CollisionChan chan);
void SweepRobot_CollisionRelayCtrlOff(enum CollisionChan chan);

#endif
