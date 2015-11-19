#ifndef __SWEEPROBOT_TESTING_RGB_LED_H
#define __SWEEPROBOT_TESTING_RGB_LED_H

#include "sweeprobot_testing.h"

typedef struct{
    int r_state;
    int g_state;
    int b_state;
    u8 validCnt;
    u8 validFlag;
}RGB_LED_TestTypeDef;

void SweepRobot_RGB_LED_Test_Task(void *pdata);
void RGB_LED_TestDataSave(void);

#endif
