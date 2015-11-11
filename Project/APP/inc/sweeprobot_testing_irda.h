#ifndef __SWEEPROBOT_TESTING_IRDA_H
#define __SWEEPROBOT_TESTING_IRDA_H

#include "sweeprobot_testing.h"

typedef struct{
    u8 code;
    u8 validCnt;
    u8 validFlag;
}IRDA_TestTypeDef;

void SweepRobot_IrDA_Test_Task(void *pdata);

#endif
