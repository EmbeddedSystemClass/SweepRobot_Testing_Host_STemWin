#ifndef __SWEEPROBOT_TESTING_MANUAL_H
#define __SWEEPROBOT_TESTING_MANUAL_H

#include "stm32f4xx.h"

enum SWRB_MANUAL_TEST_DATA_SNUM_POS{
    SWRB_MANUAL_TEST_DATA_SNUM_YEAR_POS,
    SWRB_MANUAL_TEST_DATA_SNUM_MONTH_POS,
    SWRB_MANUAL_TEST_DATA_SNUM_DATE_POS,
    SWRB_MANUAL_TEST_DATA_SNUM_NUM_POS,
    SWRB_MANUAL_TEST_DATA_SNUM_BOUND,
};

extern u8 gSwrbManualTestListviewDispItemCoord[][2];
extern u8 gSwrbManualTestListviewDispItemFrontIFRDCoord[][2];
extern u8 gSwrbManualTestListviewDispItemPowerStationCoord[][2];
extern u8 gSwrbManualTestListviewDispDataCoord[][2];
extern u8 gSwrbManualTestListviewDispDataRGBLEDCoord[][2];
extern u8 gSwrbManualTestListviewDispDataFrontIFRDCoord[][2];
extern u8 gSwrbManualTestListviewDispItemSystemCoord[][2];
extern u8 gSwrbManualTestListviewDispSNCoord[][2];

void SweepRobot_ManualTestTask(void *pdata);
void Manual_TestDataSave(void);
void SweepRobot_ManualIndicateBtnToggle(void);
void SweepRobot_ManualTestDataReset(void);
void SweepRobot_ManualTestGuiReset(void);
void SweepRobot_ManualTestBatteryVoltDisp(void);
void SweepRobot_ManualTestDataQuery(void);
void SweepRobot_ManualTestSNDisp(void);

#endif
