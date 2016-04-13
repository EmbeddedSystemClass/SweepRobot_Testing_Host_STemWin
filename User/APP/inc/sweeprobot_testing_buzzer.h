#ifndef __SWEEPROBOT_TESTING_BUZZER_H
#define __SWEEPROBOT_TESTING_BUZZER_H

#include "stm32f4xx.h"

typedef struct{
    int state;
    u8 validCnt;
    u8 validFlag;
}BUZZER_TestTypeDef;

void SweepRobot_BuzzerTestTask(void *pdata);
void BUZZER_TestDataSave(void);
void BUZZER_TestStateSet(u8 stateValue);
void BUZZER_TestValidFlagSet(u8 flagValue);

#endif
