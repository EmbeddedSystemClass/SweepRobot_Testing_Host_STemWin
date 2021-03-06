#ifndef __SWEEPROBOT_TESTING_FAN_H
#define __SWEEPROBOT_TESTING_FAN_H

#include "stm32f4xx.h"

typedef struct{
    int current;
    u8 validCnt;
    u8 validFlag;
}FAN_TestTypeDef;

void SweepRobot_FanTestTask(void *pdata);
void Fan_TestDataSave(void);

#endif
