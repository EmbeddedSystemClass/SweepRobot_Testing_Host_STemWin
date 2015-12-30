#ifndef __SWEEPROBOT_TESTING_WHEEL_FLOAT_H
#define __SWEEPROBOT_TESTING_WHEEL_FLOAT_H

#include "stm32f4xx.h"

#define SWRB_WHEEL_FLOAT_CHAN_NUM    2

enum WheelFloagChan{
    WHEEL_FLOAT_CHAN_L,
    WHEEL_FLOAT_CHAN_R, 
};

typedef struct{
    u8 value;
    u8 validCnt;
    u8 validFlag;
}WHEEL_FLOAT_TestTypeDef;

void SweepRobot_WheelFloatTestTask(void *pdata);
void SweepRobot_WheelFloatTestDataSave(void);
void SweepRobot_WheelFloatCtrlMoveToTestPos(void);
void SweepRobot_WheelFloatCtrlMoveToIdlePos(void);
void SweepRobot_WheelFloatCtrlShutDown(void);

#endif