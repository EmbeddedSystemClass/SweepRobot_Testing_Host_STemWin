#ifndef __SWEEPROBOT_TESTING_IFRD_H
#define __SWEEPROBOT_TESTING_IFRD_H

#include "sweeprobot_testing.h"

typedef struct{
    int onValue;
    int offValue;
    u8 validCnt;
    u8 validFlag;
}IFRD_TestTypeDef;

void SweepRobot_IFRDTestTask(void *pdata);
void IFRD_TestDataSave(void);

#endif
