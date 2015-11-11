#ifndef __SWEEPROBOT_TESTING_CHARGE_H
#define __SWEEPROBOT_TESTING_CHARGE_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 current;
    u16 voltage;
    u8 curValidCnt;
    u8 volValidCnt;
    u8 curValidFlag;
    u8 volValidFlag;
}CHARGE_TestTypeDef;

void SweepRobot_Charge_Test_Task(void *pdata);

#endif
