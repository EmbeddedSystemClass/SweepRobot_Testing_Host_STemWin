#include "sweeprobot_testing_manul.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"
#include "EJE_SWRB_TEST_ManulDLG.h"

#include "usart.h"
#include "includes.h"

#include <string.h>
#include <stdio.h>

#define SWRB_MANUL_TEST_MANUL_READ_WAIT_TIME    100
#define SWRB_MANUL_TEST_TASK_OSTIMEDLY_TIME_MS  100

#define SWRB_MANUL_TEST_DATA_VALID_CMP_TIMES    3

static u8 gSwrbManulTestListviewDispSNCoord[][2] = {
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L, SWRB_MANUL_TEST_LISTVIEW_ROW_SNUM},         //SNUM_YEAR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL, SWRB_MANUL_TEST_LISTVIEW_ROW_SNUM},        //SNUM_MONTH
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_M, SWRB_MANUL_TEST_LISTVIEW_ROW_SNUM},         //SNUM_DATE
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FR, SWRB_MANUL_TEST_LISTVIEW_ROW_SNUM},        //SNUM_SNUM
};

enum SWRB_MANUL_TEST_DATA_SNUM_POS{
    SWRB_MANUL_TEST_DATA_SNUM_YEAR_POS,
    SWRB_MANUL_TEST_DATA_SNUM_MONTH_POS,
    SWRB_MANUL_TEST_DATA_SNUM_DATE_POS,
    SWRB_MANUL_TEST_DATA_SNUM_SNUM_POS,
    SWRB_MANUL_TEST_DATA_SNUM_SNUM_BOUND,
};

static const char *gSwrbManulTestListviewSNQueryCmd[][1] = {
    {"SNR->YEAR"},
    {"SNR->MNTH"},
    {"SNR->DATE"},
    {"SNR->SN"},
};

static u8 gSwrbManulTestListviewDispDataCoord[][2] = {
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_WHEEL},         //LWHEEL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_WHEEL},         //RWHEEL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_BRUSH},         //LBRUSH
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_BRUSH},         //RBRUSH
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUL_TEST_LISTVIEW_ROW_BRUSH},         //MBRUSH
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUL_TEST_LISTVIEW_ROW_FAN},           //FAN
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_FL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_FR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},          //IFRD_L
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},          //IFRD_R
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_BFL,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},        //IFRD_B_FL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_BFR,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},        //IFRD_B_FR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_B_SL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_BR,SWRB_MANUL_TEST_LISTVIEW_ROW_IFRD},         //IFRD_B_SL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_COLLISION},     //COLLISION_L
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUL_TEST_LISTVIEW_ROW_COLLISION},    //COLLISION_FL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_COLLISION},     //COLLISION_R
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUL_TEST_LISTVIEW_ROW_COLLISION},    //COLLISION_FR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_WHEEL_FLOAT},   //WHEEL_FLOAT_L
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_WHEEL_FLOAT},   //WHEEL_FLOAT_R
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_ASH_TRAY},      //ASH_TRAY_INS
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUL_TEST_LISTVIEW_ROW_ASH_TRAY},     //ASH_TRAY_LVL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_UNIWHEEL},      //UNIWHEEL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_KEY},           //KEY
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_BL,SWRB_MANUL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_B
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_IRDA},          //IRDA_L
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_FL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FR,SWRB_MANUL_TEST_LISTVIEW_ROW_IRDA},         //IRDA_FR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_R,SWRB_MANUL_TEST_LISTVIEW_ROW_IRDA},          //IRDA_R
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_BUZZER},        //BUZZER
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_RGB_LED},       //RGB_LED
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_CHARGE},        //CHARGE_CUR
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_FL,SWRB_MANUL_TEST_LISTVIEW_ROW_CHARGE},       //CHARGE_VOL
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_M,SWRB_MANUL_TEST_LISTVIEW_ROW_CHARGE},        //CHARGE_24V
    {SWRB_MANUL_TEST_LISTVIEW_COLUMN_L,SWRB_MANUL_TEST_LISTVIEW_ROW_INT_VREF},      //INTERNAL_REF_VOL
};

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

static char aSwrbTestData[SWRB_MANUL_TEST_DATA_BOUND][5] = { 0 };
static int  aSwrbTestValue[SWRB_MANUL_TEST_DATA_BOUND] = { 0 };
static u8   aSwrbTestDataValidCnt[SWRB_MANUL_TEST_DATA_BOUND] = { 0 };
static u8   aSwrbTestDataValidFlag[SWRB_MANUL_TEST_DATA_BOUND] = { 0 };

static int voltPercent = 0;
static int lastVoltPercent = 0;
static int VoltCnt = 0;

static void SweepRobot_ManulTestRxDataToDataArray(int rxDataLen)
{
    int i,j,m;
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    i = 0;
    j = 0;
RX_PROC_LOOP:
    if(i<=SWRB_MANUL_TEST_DATA_BOUND){
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

static void SweepRobot_ManulTestDataArrayDisp(void)
{
    int i;
    char *str;

    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN);
    for(i=0;i<SWRB_MANUL_TEST_DATA_BOUND;i++){
        str = mymalloc(SRAMIN, sizeof(char)*10);
        *str = 0;
        mymemcpy(str, aSwrbTestData[i], sizeof(aSwrbTestData[i]));
        LISTVIEW_SetItemText(hItem, gSwrbManulTestListviewDispDataCoord[i][0], gSwrbManulTestListviewDispDataCoord[i][1], str);
        myfree(SRAMIN, str);
    }
}

static void SweepRobot_ManulTestRxDataProc(void)
{
    SweepRobot_ManulTestRxDataToDataArray(USART_RX_STA&USART_CNT_MASK);
}

static void SweepRobot_ManulTestBatteryVoltDisp(void)
{
    float volt = 0;

    if(atof(aSwrbTestData[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS])){
        volt = atof(aSwrbTestData[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS])/atof(aSwrbTestData[SWRB_MANUL_TEST_DATA_INTERNAL_REFVOL_POS])*1.2f*6;
        Edit_Set_FloatValue(hWin_SWRB_MANUL, ID_MANUL_EDIT_VOLT, volt);

        voltPercent = ((volt - SWRB_BATTERY_LOW_BOUND) / (SWRB_BATTERY_HIGH_BOUND - SWRB_BATTERY_LOW_BOUND))*100;
        if(VoltCnt > 5){
            if(voltPercent != lastVoltPercent){
                VoltCnt++;
            }else{
                VoltCnt = 6;
            }
            if(VoltCnt > 10){
                VoltCnt = 6;
                Progbar_Set_Value(hWin_SWRB_MANUL, ID_MANUL_PROGBAR_VOLT, (int)voltPercent);
                lastVoltPercent = voltPercent;
            }
        }else{
            VoltCnt++;
            Progbar_Set_Value(hWin_SWRB_MANUL, ID_MANUL_PROGBAR_VOLT, (int)voltPercent);
        }
    }
}

void SweepRobot_ManulTestDataReset(void)
{
    int i;

    for(i=0;i<SWRB_MANUL_TEST_DATA_BOUND;i++){
        Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[i][0], gSwrbManulTestListviewDispDataCoord[i][1], "0");
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[i][0], gSwrbManulTestListviewDispDataCoord[i][1], GUI_WHITE);
        aSwrbTestDataValidCnt[i] = 0;
        aSwrbTestDataValidFlag[i] = 0;
    }

    voltPercent = 0;
    lastVoltPercent = 0;
    VoltCnt = 0;
}

void SweepRobot_ManulTestGuiReset(void)
{
    int i;

    for(i=ID_MANUL_BUTTON_WHEEL;i<=ID_MANUL_BUTTON_RGB_LED;i++){
        Button_Set_BkColor(hWin_SWRB_MANUL, i, GUI_LIGHTGRAY);
    }
}

static void SweepRobot_ManulTestSNDisp(void)
{
    char *str;
    int i,j;
    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN);

    for(i=SWRB_MANUL_TEST_DATA_SNUM_YEAR_POS;i<SWRB_MANUL_TEST_DATA_SNUM_SNUM_BOUND;i++){
        printf("%s\r\n",*gSwrbManulTestListviewSNQueryCmd[i]);
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            str = mymalloc(SRAMIN, sizeof(char)*5);
            *str = 0;
            mymemcpy(str, USART_RX_BUF, sizeof(USART_RX_BUF));
            switch(i){
                case SWRB_MANUL_TEST_DATA_SNUM_MONTH_POS:
                    sprintf(str, "%02d", atoi(str));
                    break;
                case SWRB_MANUL_TEST_DATA_SNUM_DATE_POS:
                    sprintf(str, "%02d", atoi(str));
                    break;
                case SWRB_MANUL_TEST_DATA_SNUM_SNUM_POS:
                    sprintf(str, "%03d", atoi(str));
                    break;
            }
            LISTVIEW_SetItemText(hItem, gSwrbManulTestListviewDispSNCoord[i][0], gSwrbManulTestListviewDispSNCoord[i][1], str);
            myfree(SRAMIN, str);
            usartRxFlag = 0;
            for(j=0;j<5;j++)
                USART_RX_BUF[j] = 0;
            USART_RX_STA = 0;
        }
    }
}

static int SweepRobot_ManulTest_DataArrayToInt(char dataArray[][5])
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

static int SweepRobot_ManulTest_DataArrayToHex(char dataArray[][5])
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

static void SweepRobot_ManulTest_SingleValueMinMaxCmpProc(int min, int max, enum SWRB_MANUL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManulTest_DataArrayToInt(&aSwrbTestData[pos]);

    if(!aSwrbTestDataValidFlag[pos]){
        if( (min < aSwrbTestValue[pos]) && (max > aSwrbTestValue[pos])){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                    gSwrbManulTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                    gSwrbManulTestListviewDispDataCoord[pos][1], faultColor);
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
    }
}

static void SweepRobot_ManulTest_SingleValueEqualCmpProc(int value, enum SWRB_MANUL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManulTest_DataArrayToInt(&aSwrbTestData[pos]);

    if(!aSwrbTestDataValidFlag[pos]){
        if(aSwrbTestValue[pos] == value){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                    gSwrbManulTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                    gSwrbManulTestListviewDispDataCoord[pos][1], faultColor);
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
    }
}

static void SweepRobot_ManulTest_SingleIrDAValueCmpProc(enum SWRB_MANUL_TEST_DATA_POS pos, GUI_COLOR validColor, GUI_COLOR faultColor)
{
    aSwrbTestValue[pos] = SweepRobot_ManulTest_DataArrayToHex(&aSwrbTestData[pos]);

//    if(!aSwrbTestDataValidFlag[pos]){
        if(IS_IRDA_CODE(aSwrbTestValue[pos])){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                        gSwrbManulTestListviewDispDataCoord[pos][1], validColor);
            aSwrbTestDataValidCnt[pos]++;
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, gSwrbManulTestListviewDispDataCoord[pos][0],\
                                        gSwrbManulTestListviewDispDataCoord[pos][1], faultColor);
            aSwrbTestDataValidCnt[pos] = 0;
        }

        if(aSwrbTestDataValidCnt[pos] > SWRB_MANUL_TEST_DATA_VALID_CMP_TIMES){
            aSwrbTestDataValidFlag[pos] = 1;
        }
//    }
}

static void SweepRobot_ManulTest_WheelDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5, 50, SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5, 50, SWRB_MANUL_TEST_DATA_WHEEL_R_SPEED_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_BrushDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5,50, SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5,50, SWRB_MANUL_TEST_DATA_BRUSH_R_CUR_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5,500, SWRB_MANUL_TEST_DATA_BRUSH_M_CUR_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_FanDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5,500, SWRB_MANUL_TEST_DATA_FAN_CUR_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_IFRDDataProc(void)
{
    enum SWRB_MANUL_TEST_DATA_POS i;

    for(i=SWRB_MANUL_TEST_DATA_IFRD_FL_POS;i<=SWRB_MANUL_TEST_DATA_IFRD_B_SR_POS;i++){
        SweepRobot_ManulTest_SingleValueMinMaxCmpProc(1000, 3000, i, GUI_LIGHTBLUE, GUI_WHITE);
    }
}

static void SweepRobot_ManulTest_CollisionDataProc(void)
{
    enum SWRB_MANUL_TEST_DATA_POS i;

    for(i=SWRB_MANUL_TEST_DATA_COLLISION_L_POS;i<=SWRB_MANUL_TEST_DATA_COLLISION_FR_POS;i++){
        SweepRobot_ManulTest_SingleValueEqualCmpProc(0, i, GUI_LIGHTBLUE, GUI_WHITE);
    }
}

static void SweepRobot_ManulTest_WheelFloatDataProc(void)
{
    enum SWRB_MANUL_TEST_DATA_POS i;

    for(i=SWRB_MANUL_TEST_DATA_WHEEL_FLOAT_L_POS;i<=SWRB_MANUL_TEST_DATA_WHEEL_FLOAT_R_POS;i++){
        SweepRobot_ManulTest_SingleValueEqualCmpProc(0, i, GUI_LIGHTBLUE, GUI_WHITE);
    }
}

static void SweepRobot_ManulTest_AshTrayDataProc(void)
{
    SweepRobot_ManulTest_SingleValueEqualCmpProc(0, SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(50, 3000, SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_UniwheelDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(50, 3000, SWRB_MANUL_TEST_DATA_UNIWHEEL_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_KeyDataProc(void)
{
    SweepRobot_ManulTest_SingleValueEqualCmpProc(1, SWRB_MANUL_TEST_DATA_KEY_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_IrDADataProc(void)
{
    enum SWRB_MANUL_TEST_DATA_POS i;

    for(i=SWRB_MANUL_TEST_DATA_IRDA_B_RxCODE_POS;i<=SWRB_MANUL_TEST_DATA_IRDA_R_RxCODE_POS;i++){
        SweepRobot_ManulTest_SingleIrDAValueCmpProc(i, GUI_LIGHTBLUE, GUI_WHITE);
    }
}

static void SweepRobot_ManulTest_ChargeDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(5, 200, SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(2800, 3500, SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS, GUI_LIGHTBLUE, GUI_WHITE);
    SweepRobot_ManulTest_SingleValueEqualCmpProc(1, SWRB_MANUL_TEST_DATA_CHARGE_24V_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTest_InternalVRefDataProc(void)
{
    SweepRobot_ManulTest_SingleValueMinMaxCmpProc(1400, 1600, SWRB_MANUL_TEST_DATA_INTERNAL_REFVOL_POS, GUI_LIGHTBLUE, GUI_WHITE);
}

static void SweepRobot_ManulTestValueValidCmp(void)
{
    SweepRobot_ManulTest_WheelDataProc();
    SweepRobot_ManulTest_BrushDataProc();
    SweepRobot_ManulTest_FanDataProc();
    SweepRobot_ManulTest_IFRDDataProc();
    SweepRobot_ManulTest_CollisionDataProc();
    SweepRobot_ManulTest_WheelFloatDataProc();
    SweepRobot_ManulTest_AshTrayDataProc();
    SweepRobot_ManulTest_UniwheelDataProc();
    SweepRobot_ManulTest_KeyDataProc();
    SweepRobot_ManulTest_IrDADataProc();
    SweepRobot_ManulTest_ChargeDataProc();
    SweepRobot_ManulTest_InternalVRefDataProc();
}

static void SweepRobot_ManulTestInit(void)
{
    gSwrbTestRuningTaskPrio = SWRB_MANUL_TEST_TASK_PRIO;

    SweepRobot_ManulTestSNDisp();
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_ManulTestProc(void)
{
    u8 i,j;

    printf("MNL->RD\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_MANUL_TEST_MANUL_READ_WAIT_TIME);
    if(usartRxFlag){
        SweepRobot_ManulTestRxDataProc();
        SweepRobot_ManulTestBatteryVoltDisp();
        SweepRobot_ManulTestDataArrayDisp();
        usartRxFlag = 0;
        USART_RX_STA = 0;
    }
    mymemset(USART_RX_BUF, 0, sizeof(char)*USART_REC_LEN);

    SweepRobot_ManulTestValueValidCmp();

    /* Clear Data Array */
    for(i=0;i<SWRB_MANUL_TEST_DATA_BOUND;i++){
        for(j=0;j<5;j++){
            aSwrbTestData[i][j] = 0;
        }
    }
}

static void SweepRobot_ManulTestOverTimeProc(void)
{
    SweepRobot_ManulStartProc();
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

        if(gSwrbTestTaskRunCnt > 65530){
            SweepRobot_ManulTestOverTimeProc();
        }

        OSTimeDlyHMSM(0,0,0,SWRB_MANUL_TEST_TASK_OSTIMEDLY_TIME_MS);
    }
}
