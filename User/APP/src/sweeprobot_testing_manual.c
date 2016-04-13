#include "sweeprobot_testing_manual.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"
#include "EJE_SWRB_TEST_ManualDLG.h"

#include "usart.h"
#include "includes.h"

#include <string.h>
#include <stdio.h>

#define SWRB_MANUAL_TEST_MANUL_READ_WAIT_TIME    100
#define SWRB_MANUAL_TEST_TASK_OSTIMEDLY_TIME_MS  100

#define SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES    3

static const char *gSwrbManualTestListviewSNQueryCmd[] = {
    "SNR->YEAR",
    "SNR->MNTH",
    "SNR->DATE",
    "SNR->SN",
};

enum PwrStationSignal {

    PWR_STATION_BACKOFF_SIG_L   = 0x46,
    PWR_STATION_BACKOFF_SIG_R   = 0x45,
    PWR_STATION_HOME_SIG_LL     = 0x41,
    PWR_STATION_HOME_SIG_LS     = 0x44,
    PWR_STATION_HOME_SIG_RL     = 0x43,
    PWR_STATION_HOME_SIG_RS     = 0x42,
    PWR_STATION_HOME_SIG_CENTER = 0x40,
};

u8 gSwrbManualTestListviewDispItemCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_BRUSH},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_FAN},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL_FLOAT},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_ASH_TRAY},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_UNIWHEEL},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_CHARGE},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_BUZZER},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_KEY},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_INT_VREF},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},
};

u8 gSwrbManualTestListviewDispItemFrontIFRDCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},
};

u8 gSwrbManualTestListviewDispItemPowerStationCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},
};

u8 gSwrbManualTestListviewDispItemSystemCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_INT_VREF},
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_ITEM, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},
};

u8 gSwrbManualTestListviewDispDataCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL},         //LWHEEL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL},         //RWHEEL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_BRUSH},         //LBRUSH
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_BRUSH},         //RBRUSH
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUAL_TEST_LISTVIEW_ROW_BRUSH},         //MBRUSH
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUAL_TEST_LISTVIEW_ROW_FAN},           //FAN
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_FL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_FR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},          //IFRD_L
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},          //IFRD_R
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFL,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},        //IFRD_B_FL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFR,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},        //IFRD_B_FR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_B_SL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BR,SWRB_MANUAL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_B_SR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION},     //COLLISION_L
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION},    //COLLISION_FL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION},     //COLLISION_R
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUAL_TEST_LISTVIEW_ROW_COLLISION},    //COLLISION_FR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL_FLOAT},   //WHEEL_FLOAT_L
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_WHEEL_FLOAT},   //WHEEL_FLOAT_R
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_ASH_TRAY},      //ASH_TRAY_INS
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_ASH_TRAY},     //ASH_TRAY_LVL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_UNIWHEEL},      //UNIWHEEL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_KEY},           //KEY
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_B
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},          //IRDA_L
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_FL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_FR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUAL_TEST_LISTVIEW_ROW_IRDA},          //IRDA_R
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_BUZZER},        //BUZZER
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED},      //RGB_LED_RGB
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_CHARGE},        //CHARGE_CUR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_CHARGE},       //CHARGE_VOL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUAL_TEST_LISTVIEW_ROW_CHARGE},        //CHARGE_24V
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_INT_VREF},      //INTERNAL_REF_VOL
};

u8 gSwrbManualTestListviewDispDataRGBLEDCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED},       //RGB_LED_RED
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED},      //RGB_LED_GREEN
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUAL_TEST_LISTVIEW_ROW_RGB_LED},       //RGB_LED_BLUE
};

u8 gSwrbManualTestListviewDispDataFrontIFRDCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},    //FRONT_IFRD_L1
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},   //FRONT_IFRD_L2
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},    //FRONT_IFRD_L3
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},   //FRONT_IFRD_L4
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},   //FRONT_IFRD_R4
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFL,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},  //FRONT_IFRD_R3
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFR,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},  //FRONT_IFRD_R2
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BR,SWRB_MANUAL_TEST_LISTVIEW_ROW_FRONT_IFRD},   //FRONT_IFRD_R1
};

u8 gSwrbManualTestListviewDispDataPowerStationCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},    //PS_LL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},   //PS_LS
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},    //PS_M
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},   //PS_RS
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_R, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},    //PS_RL
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BL, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},   //PS_LB
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFL, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},  //PS_RB
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_BFR, SWRB_MANUAL_TEST_LISTVIEW_ROW_POWER_STATION},  //PS_PWR
};

u8 gSwrbManualTestListviewDispSNCoord[][2] = {
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_L, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},         //SNUM_YEAR
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FL, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},        //SNUM_MONTH
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_M, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},         //SNUM_DATE
    {SWRB_MANUAL_TEST_LISTVIEW_COLUMN_FR, SWRB_MANUAL_TEST_LISTVIEW_ROW_SNUM},        //SNUM_SNUM
};

static char aSwrbTestData[SWRB_MANUAL_TEST_DATA_BOUND][5] = { 0 };
static int  aSwrbTestValue[SWRB_MANUAL_TEST_DATA_BOUND] = { 0 };
static u8   aSwrbTestDataValidCnt[SWRB_MANUAL_TEST_DATA_BOUND] = { 0 };
static u8   aSwrbTestDataValidFlag[SWRB_MANUAL_TEST_DATA_BOUND] = { 0 };

static u8 aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_BOUND] = { 0 };
static u8 aSwrbTestPowerStationValidFlag[SWRB_MANUAL_TEST_DATA_POWER_STATION_BOUND] = { 0 };

static int voltPercent = 0;
static int lastVoltPercent = 0;
static int VoltCnt = 0;

static void SweepRobot_ManualTestRxDataToDataArray(int rxDataLen)
{
    int i,j,m;
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    i = 0;
    j = 0;
RX_PROC_LOOP:
    if(i<=SWRB_MANUAL_TEST_DATA_BOUND){
        for(m=0;j<=rxDataLen;j++,m++){
            if(USART_RX_BUF[j] != ','){
                aSwrbTestData[i][m] = USART_RX_BUF[j];
            }else{
                i++;
                j++;
                goto RX_PROC_LOOP;
            }
        }
    }

    OS_EXIT_CRITICAL();
}

static void SweepRobot_ManualTestDataArrayDisp(void)
{
    int i;
    char *str;

    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN);
    for(i=0;i<SWRB_MANUAL_TEST_DATA_BOUND;i++){
        str = mymalloc(SRAMIN, sizeof(char)*10);
        *str = 0;
        mymemcpy(str, aSwrbTestData[i], sizeof(aSwrbTestData[i]));
        LISTVIEW_SetItemText(hItem, gSwrbManualTestListviewDispDataCoord[i][0], gSwrbManualTestListviewDispDataCoord[i][1], str);
        myfree(SRAMIN, str);
    }
}

static void SweepRobot_ManualTestRxDataProc(void)
{
    SweepRobot_ManualTestRxDataToDataArray(USART_RX_STA&USART_CNT_MASK);
}

void SweepRobot_ManualTestBatteryVoltDisp(void)
{
    double volt = 0;

    if(atof(aSwrbTestData[SWRB_MANUAL_TEST_DATA_CHARGE_VOL_POS])){
#ifdef USE_INT_VREF
        volt = atof(aSwrbTestData[SWRB_MANUAL_TEST_DATA_CHARGE_VOL_POS])/atof(aSwrbTestData[SWRB_MANUAL_TEST_DATA_INTERNAL_REFVOL_POS])*1.2f*6;
#else
        volt = atof(aSwrbTestData[SWRB_MANUAL_TEST_DATA_CHARGE_VOL_POS])/4096*3.3f*6;
#endif
        Edit_Set_FloatValue(hWin_SWRB_MANUAL, ID_MANUAL_EDIT_VOLT, volt);

        voltPercent = (int)(((volt - SWRB_BATTERY_LOW_BOUND) / (SWRB_BATTERY_HIGH_BOUND - SWRB_BATTERY_LOW_BOUND))*100);
        if(VoltCnt > 5){
            if(voltPercent != lastVoltPercent){
                VoltCnt++;
            }else{
                VoltCnt = 6;
            }
            if(VoltCnt > 10){
                VoltCnt = 6;
                Progbar_Set_Value(hWin_SWRB_MANUAL, ID_MANUAL_PROGBAR_VOLT, (int)voltPercent);
                lastVoltPercent = voltPercent;
            }
        }else{
            VoltCnt++;
            Progbar_Set_Value(hWin_SWRB_MANUAL, ID_MANUAL_PROGBAR_VOLT, (int)voltPercent);
        }
    }
}

void SweepRobot_ManualTestDataReset(void)
{
    int i;

    for(i=0;i<SWRB_MANUAL_TEST_DATA_BOUND;i++){
        if(i == SWRB_MANUAL_TEST_DATA_BUZZER_POS){
            Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[i][0], gSwrbManualTestListviewDispDataCoord[i][1], "OFF");
        }else{
            Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[i][0], gSwrbManualTestListviewDispDataCoord[i][1], "0");
        }
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[i][0], gSwrbManualTestListviewDispDataCoord[i][1], GUI_WHITE);
        aSwrbTestDataValidCnt[i] = 0;
        aSwrbTestDataValidFlag[i] = 0;
    }

    for(i=0;i<SWRB_MANUAL_TEST_DATA_RGB_LED_BOUND;i++){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataRGBLEDCoord[i][0], gSwrbManualTestListviewDispDataRGBLEDCoord[i][1], GUI_WHITE);
    }

    for(i=0;i<SWRB_MANUAL_TEST_DATA_FRONT_IFRD_BOUND;i++){
        Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataFrontIFRDCoord[i][0], gSwrbManualTestListviewDispDataFrontIFRDCoord[i][1], "0");
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataFrontIFRDCoord[i][0], gSwrbManualTestListviewDispDataFrontIFRDCoord[i][1], GUI_WHITE);
    }

    for(i=0;i<SWRB_MANUAL_TEST_DATA_POWER_STATION_BOUND;i++){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataPowerStationCoord[i][0], gSwrbManualTestListviewDispDataPowerStationCoord[i][1], GUI_WHITE);
        aSwrbTestPowerStationCnt[i] = 0;
        aSwrbTestPowerStationValidFlag[i] = 0;
    }

    for(i=0;i<SWRB_TEST_STATE_BOUND;i++){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispItemCoord[i][0],gSwrbManualTestListviewDispItemCoord[i][1], GUI_WHITE);
    }
    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispItemFrontIFRDCoord[0][0], gSwrbManualTestListviewDispItemFrontIFRDCoord[0][1], GUI_WHITE);

    SweepRobot_ManualTest_CtrlBtnStateArrayReset();
    Progbar_Set_Value(hWin_SWRB_MANUAL, ID_MANUAL_PROGBAR_PROG, 0);
    voltPercent = 0;
    lastVoltPercent = 0;
    VoltCnt = 0;
}

void SweepRobot_ManualTestGuiReset(void)
{
    int i;

    for(i=ID_MANUAL_BUTTON_WHEEL;i<=ID_MANUAL_BUTTON_RGB_LED;i++){
        Button_Set_BkColor(hWin_SWRB_MANUAL, i, GUI_LIGHTGRAY);
    }
}

void SweepRobot_ManualTestSNDisp(void)
{
    char *str;
    int i,j;
    WM_HWIN hItem;

    str = mymalloc(SRAMIN, sizeof(char)*10);
    for(i=SWRB_MANUAL_TEST_DATA_SNUM_YEAR_POS;i<SWRB_MANUAL_TEST_DATA_SNUM_BOUND;i++){
        printf("%s\r\n",gSwrbManualTestListviewSNQueryCmd[i]);
        GUI_Delay(SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            *str = 0;
            mymemcpy(str, USART_RX_BUF, sizeof(USART_RX_BUF));
            switch(i){
                case SWRB_MANUAL_TEST_DATA_SNUM_YEAR_POS:
                    /* no need to translate str of serial number year */
                    break;
                case SWRB_MANUAL_TEST_DATA_SNUM_MONTH_POS:
                    sprintf(str, "%02d", atoi(str));
                    break;
                case SWRB_MANUAL_TEST_DATA_SNUM_DATE_POS:
                    sprintf(str, "%02d", atoi(str));
                    break;
                case SWRB_MANUAL_TEST_DATA_SNUM_NUM_POS:
                    sprintf(str, "%03d", atoi(str));
                    break;
            }
            hItem = WM_GetDialogItem(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN);
            LISTVIEW_SetItemText(hItem, gSwrbManualTestListviewDispSNCoord[i][0], gSwrbManualTestListviewDispSNCoord[i][1], str);

            usartRxFlag = 0;
            for(j=0;j<5;j++)
                USART_RX_BUF[j] = 0;
            USART_RX_STA = 0;
        }
    }
    myfree(SRAMIN, str);
}

static int SweepRobot_ManualTest_DataArrayToInt(char dataArray[][5])
{
    u8 arrayLen;
    u8 i;
    int tmp=0;

    arrayLen = sizeof(dataArray[0]);

    for(i=0;i<arrayLen;i++){
        if( '0' <= dataArray[0][i]  &&  '9' >= dataArray[0][i] ){
            if(i){
                tmp *= 10;
            }
            tmp +=  dataArray[0][i] - '0';
        }else{
            break;
        }
    }

    return tmp;
}

static int SweepRobot_ManualTest_DataArrayToHex(char dataArray[][5])
{
    u8 arrayLen;
    u8 i;
    int tmp=0;

    arrayLen = sizeof(dataArray[0]);

    for(i=0;i<arrayLen;i++){
        if( '0' <= dataArray[0][i]  &&  '9' >= dataArray[0][i] ){
            i?(tmp<<=4):tmp;
            tmp +=  dataArray[0][i] - '0';
        }else if( 'a' <= dataArray[0][i]  &&  'f' >= dataArray[0][i] ){
            i?(tmp<<=4):tmp;
            tmp +=  dataArray[0][i] - 'a'+10;
        }else if( 'A' <= dataArray[0][i]  &&  'F' >= dataArray[0][i] ){
            i?(tmp<<=4):tmp;
            tmp +=  dataArray[0][i] - 'A'+10;
        }else
            break;
    }

    return tmp;
}

static void SweepRobot_ManualTest_SingleValueMinMaxCmpProc(int min, int max, enum SWRB_MANUAL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManualTest_DataArrayToInt(&aSwrbTestData[pos]);

    if(!aSwrbTestDataValidFlag[pos]){
        if( (min < aSwrbTestValue[pos]) && (max > aSwrbTestValue[pos])){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                    gSwrbManualTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                    gSwrbManualTestListviewDispDataCoord[pos][1], faultColor);
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
    }
}

static void SweepRobot_ManualTest_SingleValueEqualCmpProc(int value, enum SWRB_MANUAL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManualTest_DataArrayToInt(&aSwrbTestData[pos]);

    if(!aSwrbTestDataValidFlag[pos]){
        if(aSwrbTestValue[pos] == value){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                    gSwrbManualTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                    gSwrbManualTestListviewDispDataCoord[pos][1], faultColor);
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
    }
}

static void SweepRobot_ManualTest_SingleIrDAValueCmpProc(enum SWRB_MANUAL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManualTest_DataArrayToHex(&aSwrbTestData[pos]);

//    if(!aSwrbTestDataValidFlag[pos]){
        if(IS_IRDA_CODE(aSwrbTestValue[pos])){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                        gSwrbManualTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            if(!aSwrbTestDataValidFlag[pos]){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, gSwrbManualTestListviewDispDataCoord[pos][0],\
                                            gSwrbManualTestListviewDispDataCoord[pos][1], faultColor);
            }
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
//    }
}

static void SweepRobot_ManualTest_PwrStationCodeCmpProc(GUI_COLOR validColor, GUI_COLOR faultColor)
{
    int i;

    /* IrDA values have already been calculated in IrDA process */
    for(i=SWRB_MANUAL_TEST_DATA_IRDA_B_RxCODE_POS;i<=SWRB_MANUAL_TEST_DATA_IRDA_R_RxCODE_POS;i++){
        switch(aSwrbTestValue[i]){
            case PWR_STATION_HOME_SIG_LL:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_LL_POS]++;
                continue;
            case PWR_STATION_HOME_SIG_LS:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_LS_POS]++;
                continue;
            case PWR_STATION_HOME_SIG_CENTER:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_CNTR_POS]++;
                continue;
            case PWR_STATION_HOME_SIG_RS:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_RS_POS]++;
                continue;
            case PWR_STATION_HOME_SIG_RL:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_RL_POS]++;
                continue;
            case PWR_STATION_BACKOFF_SIG_L:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_LB_POS]++;
                continue;
            case PWR_STATION_BACKOFF_SIG_R:
                aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_RB_POS]++;
                continue;
            default: continue;
        }
    }

    for(i=SWRB_MANUAL_TEST_DATA_POWER_STATION_LL_POS;i<=SWRB_MANUAL_TEST_DATA_POWER_STATION_RB_POS;i++){
        if(!aSwrbTestPowerStationValidFlag[i]){
            if(aSwrbTestPowerStationCnt[i] > SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES){
                aSwrbTestPowerStationValidFlag[i] = 1;
                Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[i][0],\
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[i][1],\
                                                           validColor);
            }else{
                Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[i][0],\
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[i][1],\
                                                           faultColor);
            }
        }else{
            continue;
        }
    }
}

static void SweepRobot_ManualTest_PwrStationPwrCmpProc(GUI_COLOR validColor, GUI_COLOR faultColor)
{
    if(!aSwrbTestPowerStationValidFlag[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS]){
        if(aSwrbTestValue[SWRB_MANUAL_TEST_DATA_CHARGE_24V_POS]){
            aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS]++;
        }else{
            aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS] = 0;
        }

        if(aSwrbTestPowerStationCnt[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS] > SWRB_MANUAL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestPowerStationValidFlag[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS] = 1;
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS][0],\
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS][1],\
                                                           validColor);
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS][0],\
                                                           gSwrbManualTestListviewDispDataPowerStationCoord[SWRB_MANUAL_TEST_DATA_POWER_STATION_PWR_POS][1],\
                                                           faultColor);
        }
    }
}

static void SweepRobot_ManualTest_WheelDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5, 50, SWRB_MANUAL_TEST_DATA_WHEEL_L_SPEED_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5, 50, SWRB_MANUAL_TEST_DATA_WHEEL_R_SPEED_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_BrushDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5,50, SWRB_MANUAL_TEST_DATA_BRUSH_L_CUR_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5,50, SWRB_MANUAL_TEST_DATA_BRUSH_R_CUR_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5,500, SWRB_MANUAL_TEST_DATA_BRUSH_M_CUR_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_FanDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5,500, SWRB_MANUAL_TEST_DATA_FAN_CUR_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_IFRDDataProc(void)
{
    enum SWRB_MANUAL_TEST_DATA_POS i;

    for(i=SWRB_MANUAL_TEST_DATA_IFRD_FL_POS;i<=SWRB_MANUAL_TEST_DATA_IFRD_R_POS;i++){
        SweepRobot_ManualTest_SingleValueMinMaxCmpProc(1000, 3000, i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
    for(i=SWRB_MANUAL_TEST_DATA_IFRD_B_FL_POS;i<=SWRB_MANUAL_TEST_DATA_IFRD_B_FR_POS;i++){
        SweepRobot_ManualTest_SingleValueMinMaxCmpProc(500, 3500, i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
    for(i=SWRB_MANUAL_TEST_DATA_IFRD_B_SL_POS;i<=SWRB_MANUAL_TEST_DATA_IFRD_B_SR_POS;i++){
        SweepRobot_ManualTest_SingleValueMinMaxCmpProc(200, 3800, i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
}

static void SweepRobot_ManualTest_CollisionDataProc(void)
{
    enum SWRB_MANUAL_TEST_DATA_POS i;

    for(i=SWRB_MANUAL_TEST_DATA_COLLISION_L_POS;i<=SWRB_MANUAL_TEST_DATA_COLLISION_FR_POS;i++){
        SweepRobot_ManualTest_SingleValueEqualCmpProc(0, i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
}

static void SweepRobot_ManualTest_WheelFloatDataProc(void)
{
    enum SWRB_MANUAL_TEST_DATA_POS i;

    for(i=SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS;i<=SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_R_POS;i++){
        SweepRobot_ManualTest_SingleValueEqualCmpProc(0, i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
}

static void SweepRobot_ManualTest_AshTrayDataProc(void)
{
    SweepRobot_ManualTest_SingleValueEqualCmpProc(0, SWRB_MANUAL_TEST_DATA_ASH_TRAY_INS_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(50, 3000, SWRB_MANUAL_TEST_DATA_ASH_TRAY_LVL_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_UniwheelDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(50, 3000, SWRB_MANUAL_TEST_DATA_UNIWHEEL_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_KeyDataProc(void)
{
    SweepRobot_ManualTest_SingleValueEqualCmpProc(1, SWRB_MANUAL_TEST_DATA_KEY_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_IrDADataProc(void)
{
    enum SWRB_MANUAL_TEST_DATA_POS i;

    for(i=SWRB_MANUAL_TEST_DATA_IRDA_B_RxCODE_POS;i<=SWRB_MANUAL_TEST_DATA_IRDA_R_RxCODE_POS;i++){
        SweepRobot_ManualTest_SingleIrDAValueCmpProc(i, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    }
}

static void SweepRobot_ManualTest_ChargeDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(5, 200, SWRB_MANUAL_TEST_DATA_CHARGE_CUR_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(2800, 3500, SWRB_MANUAL_TEST_DATA_CHARGE_VOL_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
    SweepRobot_ManualTest_SingleValueEqualCmpProc(1, SWRB_MANUAL_TEST_DATA_CHARGE_24V_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTest_PowerStationDataProc(void)
{
    SweepRobot_ManualTest_PwrStationCodeCmpProc(SWRB_MANUAL_TEST_OK_BK_COLOR, SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    SweepRobot_ManualTest_PwrStationPwrCmpProc(SWRB_MANUAL_TEST_OK_BK_COLOR, SWRB_MANUAL_TEST_FAULT_BK_COLOR);
}

static void SweepRobot_ManualTest_InternalVRefDataProc(void)
{
    SweepRobot_ManualTest_SingleValueMinMaxCmpProc(1400, 1600, SWRB_MANUAL_TEST_DATA_INTERNAL_REFVOL_POS, SWRB_MANUAL_TEST_OK_BK_COLOR, GUI_WHITE);
}

static void SweepRobot_ManualTestValueValidCmp(void)
{
    SweepRobot_ManualTest_WheelDataProc();
    SweepRobot_ManualTest_BrushDataProc();
    SweepRobot_ManualTest_FanDataProc();
    SweepRobot_ManualTest_IFRDDataProc();
    SweepRobot_ManualTest_CollisionDataProc();
    SweepRobot_ManualTest_WheelFloatDataProc();
    SweepRobot_ManualTest_AshTrayDataProc();
    SweepRobot_ManualTest_UniwheelDataProc();
    SweepRobot_ManualTest_KeyDataProc();
    SweepRobot_ManualTest_IrDADataProc();
    SweepRobot_ManualTest_ChargeDataProc();
    SweepRobot_ManualTest_PowerStationDataProc();
    SweepRobot_ManualTest_InternalVRefDataProc();
}

static void SweepRobot_ManualTestInit(void)
{
    gSwrbTestRuningTaskPrio = SWRB_MANUAL_TEST_TASK_PRIO;

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

void SweepRobot_ManualTestDataQuery(void)
{
    printf("MNL->RD\r\n");
    GUI_Delay(SWRB_MANUAL_TEST_MANUL_READ_WAIT_TIME);
    if(usartRxFlag){
        SweepRobot_ManualTestRxDataProc();
        SweepRobot_ManualTestBatteryVoltDisp();
        usartRxFlag = 0;
        USART_RX_STA = 0;
    }
    mymemset(USART_RX_BUF, 0, sizeof(char)*USART_RX_LEN);
}

static void SweepRobot_ManualTestProc(void)
{
    u8 i,j;

    printf("MNL->RD\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_MANUAL_TEST_MANUL_READ_WAIT_TIME);
    if(usartRxFlag){
        SweepRobot_ManualTestRxDataProc();
        SweepRobot_ManualTestDataArrayDisp();
        SweepRobot_ManualTestBatteryVoltDisp();
        usartRxFlag = 0;
        USART_RX_STA = 0;
    }
    mymemset(USART_RX_BUF, 0, sizeof(char)*USART_RX_LEN);

    SweepRobot_ManualTestValueValidCmp();

    /* Clear Data Array */
    for(i=0;i<SWRB_MANUAL_TEST_DATA_BOUND;i++){
        for(j=0;j<5;j++){
            aSwrbTestData[i][j] = 0;
        }
    }
}

//static void SweepRobot_ManualTestTimeOutProc(void)
//{

//}

void SweepRobot_ManualTestTask(void *pdata)
{
    while(1){
        gSwrbTestTaskRunCnt++;

        if(gSwrbTestTaskRunCnt == 1){
            SweepRobot_ManualTestInit();
        }

        if(gSwrbTestTaskRunCnt > 1){
            SweepRobot_ManualTestProc();
        }

//        if(gSwrbTestTaskRunCnt > 65530){
//            SweepRobot_ManualTestTimeOutProc();
//        }

        OSTimeDlyHMSM(0,0,0,SWRB_MANUAL_TEST_TASK_OSTIMEDLY_TIME_MS);
    }
}
