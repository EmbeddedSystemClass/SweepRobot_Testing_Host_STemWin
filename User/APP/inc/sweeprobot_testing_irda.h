#ifndef __SWEEPROBOT_TESTING_IRDA_H
#define __SWEEPROBOT_TESTING_IRDA_H

#include "stm32f4xx.h"

typedef struct{
    int code;
    u8 validCnt;
    u8 validFlag;
}IRDA_TestTypeDef;

void SweepRobot_IrDATestTask(void *pdata);
void IRDA_TestDataSave(void);

#endif
