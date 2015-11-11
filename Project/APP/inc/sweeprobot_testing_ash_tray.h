#ifndef __SWEEPROBOT_TESTING_ASH_TRAY_H
#define __SWEEPROBOT_TESTING_ASH_TRAY_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 value;
    u16 onValue;
    u16 offValue;
    u8 validCnt;
    u8 validFlag;
}ASH_TRAY_TestTypeDef;

void SweepRobot_Ash_Tray_Test_Task(void *pdata);

#endif
