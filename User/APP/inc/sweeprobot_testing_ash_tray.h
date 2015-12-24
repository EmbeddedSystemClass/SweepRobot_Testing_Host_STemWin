#ifndef __SWEEPROBOT_TESTING_ASH_TRAY_H
#define __SWEEPROBOT_TESTING_ASH_TRAY_H

#include "stm32f4xx.h"

typedef struct{
    int value;
    int onValue;
    int offValue;
    u8 validCnt;
    u8 validFlag;
}ASH_TRAY_TestTypeDef;

void SweepRobot_AshTrayTestTask(void *pdata);
void ASH_TRAY_TestDataSave(void);
void SweepRobot_AshTrayTestInsCtrlMoveToTestPos(void);
void SweepRobot_AshTrayTestInsCtrlMoveToIdlePos(void);
void SweepRobot_AshTrayTestInsCtrlShutDown(void);

#endif
