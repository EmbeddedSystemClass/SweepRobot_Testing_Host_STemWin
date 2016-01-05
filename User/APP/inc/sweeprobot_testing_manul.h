#ifndef __SWEEPROBOT_TESTING_MANUL_H
#define __SWEEPROBOT_TESTING_MANUL_H

#include "stm32f4xx.h"

extern u8 gSwrbManulTestListviewDispDataCoord[][2];
extern u8 gSwrbManulTestListviewDispDataRGBLEDCoord[][2];

void SweepRobot_ManulTestTask(void *pdata);
void Manul_TestDataSave(void);
void SweepRobot_ManulIndicateBtnToggle(void);
void SweepRobot_ManulTestDataReset(void);
void SweepRobot_ManulTestGuiReset(void);
void SweepRobot_ManulTestSNDisp(void);

#endif
