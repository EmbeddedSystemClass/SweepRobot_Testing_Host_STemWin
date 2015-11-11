#ifndef __SWEEPROBOT_TESTING_BRUSH_H
#define __SWEEPROBOT_TESTING_BRUSH_H

#include "sweeprobot_testing.h"

typedef struct{
    u16 current;
    u8 validCnt;
    u8 validFlag;
}BRUSH_TestTypeDef;

void SweepRobot_Brush_Test_Task(void *pdata);

#endif
