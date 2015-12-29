#ifndef __SWEEPROBOT_TESTING_STEPMOTOR_TASK_H
#define __SWEEPROBOT_TESTING_STEPMOTOR_TASK_H

#include "stm32f4xx.h"

//#define SWRB_IFRD_CHAN_NUM   8

//typedef struct{
//    int onValue;
//    int offValue;
//    u8 validCnt;
//    u8 validFlag;
//}IFRD_TestTypeDef;

//void SweepRobot_IFRDTestTask(void *pdata);
//void IFRD_TestDataSave(void);

void SweepRobot_StepMotorTask(void *pdata);

#endif
