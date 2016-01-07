#ifndef __SWEEPROBOT_TESTING_MANUL_H
#define __SWEEPROBOT_TESTING_MANUL_H

#include "stm32f4xx.h"

enum SWRB_MANUL_TEST_DATA_SNUM_POS{
    SWRB_MANUL_TEST_DATA_SNUM_YEAR_POS,
    SWRB_MANUL_TEST_DATA_SNUM_MONTH_POS,
    SWRB_MANUL_TEST_DATA_SNUM_DATE_POS,
    SWRB_MANUL_TEST_DATA_SNUM_NUM_POS,
    SWRB_MANUL_TEST_DATA_SNUM_BOUND,
};

extern u8 gSwrbManulTestListviewDispNameCoord[][2];
extern u8 gSwrbManulTestListviewDispNameFrontIFRDCoord[][2];
extern u8 gSwrbManulTestListviewDispNamePowerStationCoord[][2];
extern u8 gSwrbManulTestListviewDispDataCoord[][2];
extern u8 gSwrbManulTestListviewDispDataRGBLEDCoord[][2];
extern u8 gSwrbManulTestListviewDispDataFrontIFRDCoord[][2];
extern u8 gSwrbManulTestListviewDispNameSystemCoord[][2];
extern u8 gSwrbManulTestListviewDispSNCoord[][2];

void SweepRobot_ManulTestTask(void *pdata);
void Manul_TestDataSave(void);
void SweepRobot_ManulIndicateBtnToggle(void);
void SweepRobot_ManulTestDataReset(void);
void SweepRobot_ManulTestGuiReset(void);
void SweepRobot_ManulTestBatteryVoltDisp(void);
void SweepRobot_ManulTestDataQuery(void);
void SweepRobot_ManulTestSNDisp(void);

#endif
