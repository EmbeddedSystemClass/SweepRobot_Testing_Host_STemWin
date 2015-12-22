#ifndef __SWEEPROBOT_TEST_WHEEL_H
#define __SWEEPROBOT_TEST_WHEEL_H

#include "stm32f4xx.h"

#define SWRB_WHEEL_CHAN_NUM     2

enum WheelChan{
    
    WHEEL_CHAN_L,
    WHEEL_CHAN_R,
};

typedef struct{
    int speed;
    int validCnt;
    int validFlag;
}WHEEL_TestTypeDef;

void SweepRobot_WheelTestTask(void *pdata);
void Wheel_TestDataSave(void);

#endif
