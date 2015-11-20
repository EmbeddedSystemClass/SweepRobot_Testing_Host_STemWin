#ifndef __SWEEPROBOT_TESTING_KEY_H
#define __SWEEPROBOT_TESTING_KEY_H

#include "sweeprobot_testing.h"

typedef struct{
    int value;
    u8 validCnt;
    u8 validFlag;
}KEY_TestTypeDef;

void SweepRobot_KeyTestTask(void *pdata);
void KEY_TestDataSave(void);
void SweepRobot_KeyTestCtrlOn(void);
void SweepRobot_KeyTestCtrlOff(void);

#endif
