#ifndef __SWEEPROBOT_TESTING_CHARGE_H
#define __SWEEPROBOT_TESTING_CHARGE_H

#include "stm32f4xx.h"

typedef struct{
    int current;
    int voltage;
    int charge24vState;
    u8 curValidCnt;
    u8 volValidCnt;
    u8 charge24vValidCnt;
    u8 curValidFlag;
    u8 volValidFlag;
    u8 charge24vValidFlag;
}CHARGE_TestTypeDef;

void SweepRobot_ChargeTestTask(void *pdata);
void CHARGE_TestDataSave(void);
void SweepRobot_Charge24VOn(void);
void SweepRobot_Charge24VOff(void);

#endif
