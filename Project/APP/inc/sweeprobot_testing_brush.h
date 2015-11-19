#ifndef __SWEEPROBOT_TESTING_BRUSH_H
#define __SWEEPROBOT_TESTING_BRUSH_H

#include "sweeprobot_testing.h"

typedef struct{
    int current;
    u8 validCnt;
    u8 validFlag;
}BRUSH_TestTypeDef;

void SweepRobot_BrushTestTask(void *pdata);
void Brush_TestDataSave(void);

#endif
