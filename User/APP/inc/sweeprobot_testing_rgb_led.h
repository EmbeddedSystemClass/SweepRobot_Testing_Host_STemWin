#ifndef __SWEEPROBOT_TESTING_RGB_LED_H
#define __SWEEPROBOT_TESTING_RGB_LED_H

#include "stm32f4xx.h"

enum RGB_LED_CHAN{
    RGB_LED_RED = 1,
    RGB_LED_GREEN = 2,
    RGB_LED_BLUE = 4,
    RGB_LED_RGB = 7,
};

typedef struct{
    int r_state;
    int g_state;
    int b_state;
    u8 validCnt;
    u8 validFlag;
}RGB_LED_TestTypeDef;

void SweepRobot_RGBLEDTestTask(void *pdata);
void RGB_LED_TestDataSave(void);
u8 RGB_LED_TestValidCntGet(void);
void RGB_LED_TestRgbStateSet(int state, int value);

#endif
