#ifndef __SWEEPROBOT_TESTING_CHARGE_H
#define __SWEEPROBOT_TESTING_CHARGE_H

#include "sweeprobot_testing.h"

typedef struct{
    int current;
    int voltage;
    u8 curValidCnt;
    u8 volValidCnt;
    u8 curValidFlag;
    u8 volValidFlag;
}CHARGE_TestTypeDef;

void SweepRobot_ChargeTestTask(void *pdata);
void CHARGE_TestDataSave(void);

#endif
