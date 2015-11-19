#ifndef __SWEEPROBOT_TESTING_BUZZER_H
#define __SWEEPROBOT_TESTING_BUZZER_H

#include "sweeprobot_testing.h"

typedef struct{
    int state;
    u8 validCnt;
    u8 validFlag;
}BUZZER_TestTypeDef;

void SweepRobot_Buzzer_Test_Task(void *pdata);
void BUZZER_TestDataSave(void);

#endif
