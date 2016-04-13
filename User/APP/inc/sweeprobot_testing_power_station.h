#ifndef __SWEEPROBOT_TESTING_POWER_STATION_H
#define __SWEEPROBOT_TESTING_POWER_STATION_H

#include "stm32f4xx.h"

#define SWRB_TEST_PS_TX_SIG_NUM  7

enum CHARGE_RX_Chan{
    CHARGE_RX_Chan_Voltage,
    CHARGE_RX_Chan_Current,
    CHARGE_RX_Chan_24Vstate,
};

typedef struct{
    int voltage;
    int current;
}Charge_t;

typedef struct{
    int code;
    int state;
    Charge_t charge;
    u8 validCnt;
    u8 validFlag;
}POWERSTATION_TestTypeDef;

void SweepRobot_PowerStationTestTask(void *pdata);
void PowerStation_TestDataSave(void);
void SweepRobot_PowerStationIndicateBtnToggle(void);
void SweepRobot_PowerStationTestGraphClear(void);

#endif
