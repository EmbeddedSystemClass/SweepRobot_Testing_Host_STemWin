#ifndef __SWRB_TEST_DRIVER_H
#define __SWRB_TEST_DRIVER_H

#include "sweeprobot_testing_conf.h"

void SweepRobot_CollisionTestGPIOInit(void);
void SweepRobot_CollisionCtrlOn(enum CollisionChan chan);
void SweepRobot_CollisionCtrlOff(enum CollisionChan chan);

void SweepRobot_WheelFloatTestGPIOInit(void);
void SweepRobot_WheelFloatCtrlMoveToTestPos(void);
void SweepRobot_WheelFloatCtrlMoveToIdlePos(void);
void SweepRobot_WheelFloatCtrlShutDown(void);

void SweepRobot_AshTrayTestGPIOInit(void);
void SweepRobot_AshTrayTestInsCtrlMoveToTestPos(void);
void SweepRobot_AshTrayTestInsCtrlMoveToIdlePos(void);
void SweepRobot_AshTrayTestInsCtrlShutDown(void);

void SweepRobot_KeyTestGPIOInit(void);
void SweepRobot_KeyTestCtrlTestPos(void);
void SweepRobot_KeyTestCtrlIdlePos(void);

#endif
