#ifndef __SWEEPROBOT_TESTING_BRUSH_H
#define __SWEEPROBOT_TESTING_BRUSH_H

#include "stm32f4xx.h"

#define SWRB_BRUSH_CHAN_NUM     3

enum BrushChan{
    BRUSH_CHAN_L,
    BRUSH_CHAN_R,
    BRUSH_CHAN_M,
};

typedef struct{
    int current;
    u8 validCnt;
    u8 validFlag;
}BRUSH_TestTypeDef;

void SweepRobot_BrushTestTask(void *pdata);
void Brush_TestDataSave(void);

#endif
