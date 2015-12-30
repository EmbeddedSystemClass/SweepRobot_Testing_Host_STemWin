#ifndef __SWEEPROBOT_TESTING_FRONT_IFRD_H
#define __SWEEPROBOT_TESTING_FRONT_IFRD_H

#include "stm32f4xx.h"

#define SWRB_FRONT_IFRD_CHAN_NUM   8

typedef struct{
    int onValue;
    int offValue;
    u8 validCnt;
    u8 validFlag;
}FRONT_IFRD_TestTypeDef;

void SweepRobot_FrontIFRDTestTask(void *pdata);
void FrontIFRD_TestDataSave(void);

#endif
