#ifndef __SWEEPROBOT_TESTING_MANUL_H
#define __SWEEPROBOT_TESTING_MANUL_H

#include "stm32f4xx.h"

void SweepRobot_ManulTestTask(void *pdata);
void Manul_TestDataSave(void);
void SweepRobot_ManulIndicateBtnToggle(void);
void SweepRobot_ManulTestDataReset(void);

#endif
