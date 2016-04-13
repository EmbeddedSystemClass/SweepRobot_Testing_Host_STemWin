#ifndef __SWEEPROBOT_TESTING_KEY_H
#define __SWEEPROBOT_TESTING_KEY_H

#include "stm32f4xx.h"

typedef struct{
    int pushValue;
    int releaseValue;
    int value;
    u8 validCnt;
    u8 validFlag;
}KEY_TestTypeDef;

void SweepRobot_KeyTestTask(void *pdata);
void KEY_TestDataSave(void);

#endif
