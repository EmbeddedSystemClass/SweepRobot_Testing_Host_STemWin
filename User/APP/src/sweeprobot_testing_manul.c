#include "sweeprobot_testing_manul.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"

#include "usart.h"
#include "includes.h"

enum SWRB_MANUL_TEST_DATA_POS{
    
    SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS,
    SWRB_MANUL_TEST_DATA_WHEEL_R_SPEED_POS,
    
    SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS,
    SWRB_MANUL_TEST_DATA_BRUSH_R_CUR_POS,
    SWRB_MANUL_TEST_DATA_BRUSH_M_CUR_POS,
    
    SWRB_MANUL_TEST_DATA_FAN_CUR_POS,
    
    SWRB_MANUL_TEST_DATA_IFRD_FL_POS,
    SWRB_MANUL_TEST_DATA_IFRD_FR_POS,
    SWRB_MANUL_TEST_DATA_IFRD_L_POS,
    SWRB_MANUL_TEST_DATA_IFRD_R_POS,
    SWRB_MANUL_TEST_DATA_IFRD_B_FL_POS,
    SWRB_MANUL_TEST_DATA_IFRD_B_FR_POS,
    SWRB_MANUL_TEST_DATA_IFRD_B_SL_POS,
    SWRB_MANUL_TEST_DATA_IFRD_B_SR_POS,
    
    SWRB_MANUL_TEST_DATA_COLLISION_L_POS,
    SWRB_MANUL_TEST_DATA_COLLISION_FL_POS,
    SWRB_MANUL_TEST_DATA_COLLISION_R_POS,
    SWRB_MANUL_TEST_DATA_COLLISION_FR_POS,
    
    SWRB_MANUL_TEST_DATA_WHEEL_FLOAT_L_POS,
    SWRB_MANUL_TEST_DATA_WHEEL_FLOAT_R_POS,
    
    SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS,
    SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS,
    
    SWRB_MANUL_TEST_DATA_UNIWHEEL_POS,
    
    SWRB_MANUL_TEST_DATA_KEY_POS,
    
    SWRB_MANUL_TEST_DATA_IRDA_B_RxCODE_POS,
    SWRB_MANUL_TEST_DATA_IRDA_L_RxCODE_POS,
    SWRB_MANUL_TEST_DATA_IRDA_FL_RxCODE_POS,
    SWRB_MANUL_TEST_DATA_IRDA_FR_RxCODE_POS,
    SWRB_MANUL_TEST_DATA_IRDA_R_RxCODE_POS,
    
    SWRB_MANUL_TEST_DATA_BUZZER_OK_POS,
    
    SWRB_MANUL_TEST_DATA_RGB_LED_OK_POS,
    
    SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS,
    SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS,
    SWRB_MANUL_TEST_DATA_CHARGE_24V_POS,
    
    SWRB_MANUL_TEST_DATA_INTERNAL_REFVOL_POS,
    
    SWRB_MANUL_TEST_DATA_BOUND,
};

typedef struct{
    WHEEL_TestTypeDef wheel[SWRB_WHEEL_CHAN_NUM];
    BRUSH_TestTypeDef brush[SWRB_BRUSH_CHAN_NUM];
    FAN_TestTypeDef fan;
    IFRD_TestTypeDef ifrd[SWRB_IFRD_CHAN_NUM];
    COLLISION_TestTypeDef collision[SWRB_COLLISION_CHAN_NUM];
    WHEEL_FLOAT_TestTypeDef wheel_float[SWRB_WHEEL_FLOAT_CHAN_NUM];
    ASH_TRAY_TestTypeDef ash_tray;
    UNIWHEEL_TestTypeDef uniwheel;
    KEY_TestTypeDef key;
    IRDA_TestTypeDef irda[SWRB_IRDA_CHAN_NUM];
    BUZZER_TestTypeDef buzzer;
    RGB_LED_TestTypeDef rgb_led;
    CHARGE_TestTypeDef charge;
    POWERSTATION_TestTypeDef powerstation;
}MANUL_TestTypeDef;

static MANUL_TestTypeDef manul;

static void SweepRobot_ManulTestInit(void)
{
//    int i;
//    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_MANUL_TEST_TASK_PRIO;
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_ManulTestProc(void)
{
    
}

void SweepRobot_ManulTestTask(void *pdata)
{
    while(1){
        gSwrbTestTaskRunCnt++;

        if(gSwrbTestTaskRunCnt == 1){
            SweepRobot_ManulTestInit();
        }

        if(gSwrbTestTaskRunCnt > 1){
            SweepRobot_ManulTestProc();
        }

        OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
    }
}
