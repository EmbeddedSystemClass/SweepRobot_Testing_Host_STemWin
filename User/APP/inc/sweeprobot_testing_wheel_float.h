#ifndef __SWEEPROBOT_TESTING_WHEEL_FLOAT_H
#define __SWEEPROBOT_TESTING_WHEEL_FLOAT_H

#include "stm32f4xx.h"

#define SWRB_WHEEL_FLOAT_CHAN_NUM    2

enum WheelFloagChan{
    WHEEL_FLOAT_CHAN_L,
    WHEEL_FLOAT_CHAN_R, 
};

/* TODO: Improve Test Typedef definition of Wheel Float Task */
//typedef struct{
//    int value;
//    u8 validCnt;
//    u8 ValideFlag;
//}WHEEL_FLOAT_TEST_DATA_Typedef;

//typedef struct{
//    WHEEL_FLOAT_TEST_DATA_Typedef up;
//    WHEEL_FLOAT_TEST_DATA_Typedef down;
//}WHEEL_FLOAT_TEST_POS_Typedef;

typedef struct{
    int upValue;
    int downValue;
    int value;
    u8 upValidCnt;
    u8 downValidCnt;
    u8 validCnt;
    u8 downValidFlag;
    u8 validFlag;
}WHEEL_FLOAT_TestTypeDef;

void SweepRobot_WheelFloatTestTask(void *pdata);
void SweepRobot_WheelFloatTestDataSave(void);

#endif
