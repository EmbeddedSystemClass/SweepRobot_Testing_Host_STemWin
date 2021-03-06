#ifndef __EJE_SWRB_TEST_MANUALDLG_H
#define __EJE_SWRB_TEST_MANUALDLG_H

#include "Dialog.h"
#include "stm32f4xx.h"

enum SWRB_MANUAL_TEST_LISTVIEW_COLUMN{
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFL,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFR,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BR,
    SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BOUND,
};

enum SWRB_MANUAL_TEST_LISTVIEW_ROW{
    SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_BRUSH,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_FAN,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL_FLOAT,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_ASH_TRAY,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_UNIWHEEL,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_CHARGE,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_BUZZER,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_KEY,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_INT_VREF,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM,
    SWRB_MANUAL_TEST_LISTVIEW_ROW_BOUND,
};

enum SWRB_MANUAL_TEST_DATA_POS{

    SWRB_MANUAL_TEST_DATA_WHEEL_L_SPEED_POS,
    SWRB_MANUAL_TEST_DATA_WHEEL_R_SPEED_POS,

    SWRB_MANUAL_TEST_DATA_BRUSH_L_CUR_POS,
    SWRB_MANUAL_TEST_DATA_BRUSH_R_CUR_POS,
    SWRB_MANUAL_TEST_DATA_BRUSH_M_CUR_POS,

    SWRB_MANUAL_TEST_DATA_FAN_CUR_POS,

    SWRB_MANUAL_TEST_DATA_IFRD_FL_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_FR_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_L_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_R_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_B_FL_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_B_FR_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_B_SL_POS,
    SWRB_MANUAL_TEST_DATA_IFRD_B_SR_POS,

    SWRB_MANUAL_TEST_DATA_COLLISION_L_POS,
    SWRB_MANUAL_TEST_DATA_COLLISION_FL_POS,
    SWRB_MANUAL_TEST_DATA_COLLISION_R_POS,
    SWRB_MANUAL_TEST_DATA_COLLISION_FR_POS,

    SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS,
    SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_R_POS,

    SWRB_MANUAL_TEST_DATA_ASH_TRAY_INS_POS,
    SWRB_MANUAL_TEST_DATA_ASH_TRAY_LVL_POS,

    SWRB_MANUAL_TEST_DATA_UNIWHEEL_POS,

    SWRB_MANUAL_TEST_DATA_KEY_POS,

    SWRB_MANUAL_TEST_DATA_IRDA_B_RxCODE_POS,
    SWRB_MANUAL_TEST_DATA_IRDA_L_RxCODE_POS,
    SWRB_MANUAL_TEST_DATA_IRDA_FL_RxCODE_POS,
    SWRB_MANUAL_TEST_DATA_IRDA_FR_RxCODE_POS,
    SWRB_MANUAL_TEST_DATA_IRDA_R_RxCODE_POS,

    SWRB_MANUAL_TEST_DATA_BUZZER_POS,

    SWRB_MANUAL_TEST_DATA_RGB_LED_RGB_POS,

    SWRB_MANUAL_TEST_DATA_CHARGE_CUR_POS,
    SWRB_MANUAL_TEST_DATA_CHARGE_VOL_POS,
    SWRB_MANUAL_TEST_DATA_CHARGE_24V_POS,

    SWRB_MANUAL_TEST_DATA_INTERNAL_REFVOL_POS,

    SWRB_MANUAL_TEST_DATA_BOUND,
};

enum SWRB_MANUAL_TEST_DATA_RGB_LED_POS{
    SWRB_MANUAL_TEST_DATA_RGB_LED_RED_POS,
    SWRB_MANUAL_TEST_DATA_RGB_LED_GREEN_POS,
    SWRB_MANUAL_TEST_DATA_RGB_LED_BLUE_POS,
    SWRB_MANUAL_TEST_DATA_RGB_LED_BOUND,
};

enum SWRB_MANUAL_TEST_DATA_FRONT_IFRD_POS{
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_L1_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_L2_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_L3_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_L4_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_R4_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_R3_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_R2_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_R1_POS,
    SWRB_MANUAL_TEST_DATA_FRONT_IFRD_BOUND,
};

enum SWRB_MANUAL_TEST_DATA_POWER_STATION_POS{
    SWRB_MANUAL_TEST_DATA_POWER_STATION_LL_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_LS_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_CNTR_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_RS_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_RL_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_LB_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_RB_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS,
    SWRB_MANUAL_TEST_DATA_POWER_STATION_BOUND,
};

extern WM_HWIN CreateManualTestDLG(void);

#endif
