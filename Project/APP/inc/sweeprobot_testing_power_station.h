#ifndef __SWEEPROBOT_TESTING_POWER_STATION_H
#define __SWEEPROBOT_TESTING_POWER_STATION_H

#include "stm32f4xx.h"

typedef struct{
    int code;
    int state;
    u8 validCnt;
    u8 validFlag;
}PowerStation_TestTypeDef;

void SweepRobot_PowerStationTestTask(void *pdata);
void PowerStation_TestDataSave(void);
void SweepRobot_PowerStationIndicateBtnToggle(void);

#endif
