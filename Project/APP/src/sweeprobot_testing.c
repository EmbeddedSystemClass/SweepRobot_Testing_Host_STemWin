#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"

#include "usart.h"
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <string.h>

#define SWRB_TEST_ACQUIRED_DATA_LEN_MAX  60

u8 usartRxFlag = 0;
int usartRxNum = 0;

enum CryptoMode{
    DecryptMode,
    EncryptMode,
};
static char *gEncryptStr;

enum SWRB_TEST_SELECT gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;
enum SWRB_TEST_MODE gSwrbTestMode = SWRB_TEST_MODE_IDLE;
enum SWRB_TEST_SET_STATE gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;
enum SWRB_TEST_TASK_PRIO gSwrbTestRuningTaskPrio;
u32 gSwrbTestStateMap = 0;
u32 lastSwrbTestStateMap = 0;
u16 gSwrbTestTaskRunCnt = 0;
int gSwrbTestValidTaskCnt;
int gSwrbTestValidTaskCntTotal;
int gSwrbTestAcquiredData[SWRB_TEST_ACQUIRED_DATA_LEN_MAX] = {0};

typedef void (*TestCBFunc_t)(void);

static TestCBFunc_t gLedTaskCB = NULL;

static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;

static RTC_TimeTypeDef rtcTime;
static RTC_DateTypeDef rtcDate;

//static WM_MESSAGE *pWmMsg;

static void Start_Task(void *pdata);
static void emWin_Maintask(void *pdata);
static void Touch_Task(void *pdata);
static void Led_Task(void *pdata);
static void Key_Task(void *pdata);
static void Rtc_Task(void *pdata);
//static void Usart_Task(void *pdata);
static void SWRB_TestCtrlTask(void *pdata);
/* TODO: Add Exception Check */
//static void SWRB_ExceptionCheckTask(void *pdata);
static void SweepRobot_TestInitProc(void);
static void SWRB_ValidTestTaskCntGet(void);
static FRESULT SWRB_TestDataFileCrypt(enum CryptoMode mode);

#define TEST_LED_TASK_CB_REG(f)             do{gLedTaskCB=f;}while(0)
#define TEST_LED_TASK_CB_DEREG()            do{gLedTaskCB=NULL;}while(0)

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK RTC_TASK_STK[RTC_STK_SIZE];
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
OS_STK LED_TASK_STK[LED_STK_SIZE];
OS_STK SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE];
OS_STK SWRB_TEST_EXCEPTION_CHECK_TASK_STK[SWRB_TEST_EXCEPTION_CHECK_STK_SIZE];
OS_STK SWRB_WHEEL_TEST_TASK_STK[SWRB_WHEEL_TEST_STK_SIZE];
OS_STK SWRB_BRUSH_TEST_TASK_STK[SWRB_BRUSH_TEST_STK_SIZE];
OS_STK SWRB_FAN_TEST_TASK_STK[SWRB_FAN_TEST_STK_SIZE];
OS_STK SWRB_IFRD_TEST_TASK_STK[SWRB_IFRD_TEST_STK_SIZE];
OS_STK SWRB_COLLISION_TEST_TASK_STK[SWRB_COLLISION_TEST_STK_SIZE];
OS_STK SWRB_WHEEL_FLOAT_TEST_TASK_STK[SWRB_WHEEL_FLOAT_TEST_STK_SIZE];
OS_STK SWRB_ASH_TRAY_TEST_TASK_STK[SWRB_ASH_TRAY_TEST_STK_SIZE];
OS_STK SWRB_UNIWHEEL_TEST_TASK_STK[SWRB_UNIWHEEL_TEST_STK_SIZE];
OS_STK SWRB_KEY_TEST_TASK_STK[SWRB_KEY_TEST_STK_SIZE];
OS_STK SWRB_IRDA_TEST_TASK_STK[SWRB_IRDA_TEST_STK_SIZE];
OS_STK SWRB_BUZZER_TEST_TASK_STK[SWRB_BUZZER_TEST_STK_SIZE];
OS_STK SWRB_RGB_LED_TEST_TASK_STK[SWRB_RGB_LED_TEST_STK_SIZE];
OS_STK SWRB_CHARGE_TEST_TASK_STK[SWRB_CHARGE_TEST_STK_SIZE];
OS_STK SWRB_POWER_STATION_TASK_STK[SWRB_POWER_STATION_TEST_STK_SIZE];//23

void OS_Task_Create(void)
{
    OSTaskCreate(Start_Task,
                (void*)0,
                (OS_STK*)&START_TASK_STK[START_STK_SIZE-1],
                START_TASK_PRIO);
}

void Start_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;

    OSStatInit();

    OS_ENTER_CRITICAL();
    OSTaskCreate(emWin_Maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWIN_TASK_PRIO);
    OS_EXIT_CRITICAL();

    OSTaskDel(OS_PRIO_SELF);
}

static void SweepRobot_TestCkbStateSet(u8 state)
{
    u16 i;

    for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++)
        Checkbox_Set_State(hWin_SWRB_MAIN, i, state);
}

void emWin_Maintask(void *pdata)
{
    OS_CPU_SR cpu_sr;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);

//    MainTask();

    GUI_Init();

    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_MULTIBUF_Enable(1);

    OS_ENTER_CRITICAL();

    hWin_SWRB_START = CreateEJE_SWRB_TEST_StartDLG();
    hWin_SWRB_MAIN = CreateEJE_SWRB_TEST_MainDLG();
    hWin_SWRB_POWER_STATION = CreateEJE_SWRB_TEST_PowerStationDLG();
    hWin_SWRB_SNSETTING = CreateSNSettingDLG();
    hWin_SWRB_TIMESETTING = CreateTimeSettingDLG();

    OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(Rtc_Task,(void*)0,(OS_STK*)&RTC_TASK_STK[RTC_STK_SIZE-1],RTC_TASK_PRIO);
    OSTaskCreate(SWRB_TestCtrlTask,(void*)0,(OS_STK*)&SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE-1],SWRB_TEST_CTRL_TASK_PRIO);
    /* TODO: Add Exception Check */
//    OSTaskCreate(SWRB_ExceptionCheckTask, (void*)0,(OS_STK*)&SWRB_TEST_EXCEPTION_CHECK_TASK_STK[SWRB_TEST_EXCEPTION_CHECK_STK_SIZE-1],SWRB_TEST_EXCEPTION_CHECK_TASK_PRIO);

    OS_EXIT_CRITICAL();

    gSwrbTestValidTaskCnt = 0;

    SweepRobot_TestCkbStateSet(1);
    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_SNSETTING, ID_SNSET_LISTWHEEL_YEAR, ID_SNSET_LISTWHEEL_MONTH, ID_SNSET_LISTWHEEL_DAY);
    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_TIMESETTING, ID_TIMESET_LISTWHEEL_YEAR, ID_TIMESET_LISTWHEEL_MONTH, ID_TIMESET_LISTWHEEL_DAY);

    while(1)
    {
        if(gSwrbTestMode == SWRB_TEST_MODE_SET){
            SWRB_SET_ListwheelSnapPosUpdate();
            SWRB_SET_EditTextUpdate();
        }

        GUI_Exec();
        OSTimeDlyHMSM(0,0,0,20);
    }
}

void Touch_Task(void *pdata)
{
    TP_Init();

	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,5);
	}
}

void Led_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;

    TEST_LED_TASK_CB_DEREG();

	while(1)
	{
		LED0 = !LED0;

        if(gLedTaskCB!=NULL){
            OS_ENTER_CRITICAL();
            gLedTaskCB();
            OS_EXIT_CRITICAL();
        }

		OSTimeDlyHMSM(0,0,0,500);
	}
}

void Key_Task(void *pdata)
{
	while(1){
		if(gkeyCode){
			gkeyCodeGetFinishFlag = 1;
		}else{
			gkeyCode = KEY_Scan(0);
		}
		OSTimeDlyHMSM(0,0,0,5);
	}
}

void Rtc_Task(void *pdata)
{
    while(1){

        RTC_GetDate(RTC_Format_BIN, &rtcDate);
        RTC_GetTime(RTC_Format_BIN, &rtcTime);

        RTC_TIME_Disp(&rtcDate, &rtcTime);

        OSTimeDlyHMSM(0,0,1,0);
    }
}

void SWRB_TestDataFileWriteString(char *str)
{
    SWRB_TestDataFileOpen(FA_WRITE|FA_OPEN_ALWAYS);
    f_puts(str,file);
    f_close(file);
}

void SWRB_TestDataFileWriteData(char *headstr, int data, u8 CRflag)
{
    char *dataStr;

    dataStr = mymalloc(SRAMIN, sizeof(char)*10);
    mymemset(dataStr, 0, sizeof(char)*10);
    if(CRflag){
        sprintf(dataStr, "%s%d\r\n", headstr, data);
    }else{
        sprintf(dataStr, "%s%d", headstr, data);
    }
    f_puts(dataStr,file);
    myfree(SRAMIN, dataStr);
}

void SWRB_TestDataFileWriteDate(char *headStr, RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    char *dateStr;

    dateStr = mymalloc(SRAMIN, sizeof(char)*40);
    *dateStr = 0;

    SWRB_TestDataFileOpen(FA_WRITE|FA_OPEN_ALWAYS);

    sprintf(dateStr, "\r\n%s:20%d/%d/%d %d:%d:%d\r\n", headStr, date->RTC_Year, date->RTC_Month, date->RTC_Date, time->RTC_Hours, time->RTC_Minutes, time->RTC_Seconds);
    f_puts(dateStr, file);
    f_close(file);

    myfree(SRAMIN, dateStr);
}

FRESULT SWRB_TestDataFileCrypt(enum CryptoMode mode)
{
    FRESULT flErr;
    int i;
    int fileLength, leftFileLength;

    flErr = flErr;
    leftFileLength = leftFileLength;

    SWRB_TestDataFileOpen(FA_READ|FA_OPEN_ALWAYS);

    fileLength = f_size(file);
    leftFileLength = fileLength%8;

    f_close(file);

    gEncryptStr = mymalloc(SRAMIN, sizeof(char)*10);

    if(fileLength>>3){
        for(i=0;i<(fileLength>>3);i++){
            flErr = SWRB_TestDataFileOpen(FA_READ|FA_WRITE);

            if(flErr == FR_OK){
                f_lseek(file, 8*i);

                *gEncryptStr = 0;
                flErr = f_read(file, gEncryptStr, 8, &br);

                if(mode == EncryptMode){
                    SWRB_StrEncrypt(gEncryptStr);
                }else{
                    SWRB_StrDecrypt(gEncryptStr);
                }

                f_lseek(file, 8*i);
                flErr = f_write(file, gEncryptStr, 8, &bw);

                f_close(file);
            }else{
               return flErr;
            }
        }
    }else{

    }
    myfree(SRAMIN, gEncryptStr);

    return flErr;
}

static void SWRB_TEST_BUTTON_CTRL_Start(void)
{
    switch(gSwrbTestSelectFlag){
        case SWRB_TEST_SELECT_NONE:
            break;
        case SWRB_TEST_SELECT_PCB:
            SweepRobot_PCBTestStartProc();
            break;
        case SWRB_TEST_SELECT_POWER_STATION:
            SweepRobot_PowerStationTestStartProc();
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Set(void)
{
    switch(gSwrbTestSelectFlag){
        case SWRB_TEST_SELECT_NONE:
            break;
        case SWRB_TEST_SELECT_PCB:
            SweepRobot_PCBTestSetProc();
            break;
        case SWRB_TEST_SELECT_POWER_STATION:
            ;
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Stop(void)
{
    switch(gSwrbTestSelectFlag){
        case SWRB_TEST_SELECT_NONE:
            SweepRobot_StartDlgPCBBtnClickProc();
            break;
        case SWRB_TEST_SELECT_PCB:
            SweepRobot_PCBTestStopProc();
            break;
        case SWRB_TEST_SELECT_POWER_STATION:
            SweepRobot_PowerStationTestStopProc();
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Exit(void)
{
    switch(gSwrbTestSelectFlag){
        case SWRB_TEST_SELECT_NONE:
            SweepRobot_StartDlgPowerStationBtnClickPorc();
            break;
        case SWRB_TEST_SELECT_PCB:
            SweepRobot_PCBTestExitProc();
            break;
        case SWRB_TEST_SELECT_POWER_STATION:
            SweepRobot_PowerStationTestExitProc();
            break;
    }
}

void SWRB_TestCtrlTask(void *pdata)
{
    u8 i;
    OS_CPU_SR cpu_sr;

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SweepRobot_TestInitProc();
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "PLEASE PRESS SET TO SET SERIAL NUMBER BEFORE TEST\r\n");

    OS_ENTER_CRITICAL();

    OSTaskCreate(SweepRobot_WheelTestTask,(void*)0,(OS_STK*)&SWRB_WHEEL_TEST_TASK_STK[SWRB_WHEEL_TEST_STK_SIZE-1],SWRB_WHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_BrushTestTask,(void*)0,(OS_STK*)&SWRB_BRUSH_TEST_TASK_STK[SWRB_BRUSH_TEST_STK_SIZE-1],SWRB_BRUSH_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_FanTestTask,(void*)0,(OS_STK*)&SWRB_FAN_TEST_TASK_STK[SWRB_FAN_TEST_STK_SIZE-1],SWRB_FAN_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IFRDTestTask,(void*)0,(OS_STK*)&SWRB_IFRD_TEST_TASK_STK[SWRB_IFRD_TEST_STK_SIZE-1],SWRB_IFRD_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_CollisionTestTask,(void*)0,(OS_STK*)&SWRB_COLLISION_TEST_TASK_STK[SWRB_COLLISION_TEST_STK_SIZE-1],SWRB_COLLISION_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_WheelFloatTestTask,(void*)0,(OS_STK*)&SWRB_WHEEL_FLOAT_TEST_TASK_STK[SWRB_WHEEL_FLOAT_TEST_STK_SIZE-1],SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_AshTrayTestTask,(void*)0,(OS_STK*)&SWRB_ASH_TRAY_TEST_TASK_STK[SWRB_ASH_TRAY_TEST_STK_SIZE-1],SWRB_ASH_TRAY_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_UniWheel_Test_Task,(void*)0,(OS_STK*)&SWRB_UNIWHEEL_TEST_TASK_STK[SWRB_UNIWHEEL_TEST_STK_SIZE-1],SWRB_UNIWHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_KeyTestTask,(void*)0,(OS_STK*)&SWRB_KEY_TEST_TASK_STK[SWRB_KEY_TEST_STK_SIZE-1],SWRB_KEY_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IrDATestTask,(void*)0,(OS_STK*)&SWRB_IRDA_TEST_TASK_STK[SWRB_IRDA_TEST_STK_SIZE-1],SWRB_IRDA_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_BuzzerTestTask,(void*)0,(OS_STK*)&SWRB_BUZZER_TEST_TASK_STK[SWRB_BUZZER_TEST_STK_SIZE-1],SWRB_BUZZER_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_RGBLEDTestTask,(void*)0,(OS_STK*)&SWRB_RGB_LED_TEST_TASK_STK[SWRB_RGB_LED_TEST_STK_SIZE-1],SWRB_RGB_LED_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_ChargeTestTask,(void*)0,(OS_STK*)&SWRB_CHARGE_TEST_TASK_STK[SWRB_CHARGE_TEST_STK_SIZE-1],SWRB_CHARGE_TEST_TASK_PRIO);

    for(i=SWRB_WHEEL_TEST_TASK_PRIO;i<SWRB_TEST_TASK_PRIO_END_BOUND;i++){
        OSTaskSuspend(i);
    }

    OSTaskCreate(SweepRobot_PowerStationTestTask,(void*)0,(OS_STK*)&SWRB_POWER_STATION_TASK_STK[SWRB_POWER_STATION_TEST_STK_SIZE-1],SWRB_POWER_STATION_TEST_TASK_PRIO);
    OSTaskSuspend(SWRB_POWER_STATION_TEST_TASK_PRIO);

    OSTaskCreate(Touch_Task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
    OSTaskCreate(Key_Task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);

    OS_EXIT_CRITICAL();

    while(1){
        if(gkeyCodeGetFinishFlag){
            switch(gkeyCode){
                /* TEST START/PAUSE/RESUME PRESSED*/
                case 1:
                    SWRB_TEST_BUTTON_CTRL_Start();
                    break;
                /* TEST SET PRESSED */
                case 2:
                    SWRB_TEST_BUTTON_CTRL_Set();
                    break;
                /* TEST STOP PRESSED */
                case 3:
                    SWRB_TEST_BUTTON_CTRL_Stop();
                    break;
                /* TEST EXIT PRESSED */
                case 4:
                    SWRB_TEST_BUTTON_CTRL_Exit();
                    break;
                default:
                    break;
            }
            gkeyCode = 0;
            gkeyCodeGetFinishFlag = 0;
        }
        OSTimeDlyHMSM(0,0,0,40);
    }
}

/* TODO: Add Exception Check */
/*
void SWRB_ExceptionCheckTask(void *pdata)
{
    while(1){

        if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        }

        OSTimeDlyHMSM(0,0,1,0);
    }
}
*/

void SweepRobot_PCBTestStartProc(void)
{
    char *str;
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE || gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE){
            MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  "TEST RESUMED\r\n");
        }else{
            SWRB_TestDataFileWriteSN(hWin_SWRB_SNSETTING);
            SWRB_TestDataFileWriteDate("Test Start Time", &rtcDate, &rtcTime);
            
            MultiEdit_Set_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "\r\n");
            str = mymalloc(SRAMIN, sizeof(char)*50);
            sprintf(str, "\r\nTest Start Time:20%d/%d/%d %d:%d:%d\r\n",\
                rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
            MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
            myfree(SRAMIN, str);
            Progbar_Set_Value(0);
        }

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        SWRB_TestCheckboxDisable();

        TEST_LED_TASK_CB_REG(SweepRobot_MainTestIndicateBtnToggle);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, GUI_LIGHTRED);
//        Button_Set_Text(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, "PAUSE");
        BUTTON_Disp_Pause_CHNStr(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, 18, 43);
        SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_EXIT);
        SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_SET);
        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_STOP);
        printf("TEST->ON\r\n");

        OS_ENTER_CRITICAL();
        OSTaskResume(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        /* FIXME:  Task count would be wrong if click checkbox when uncommented */
//        SWRB_TestCheckboxEnable();

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_GREEN);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, GUI_USER_204153051);
//        Button_Set_Text(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, "RESUME");
        BUTTON_Disp_Resume_CHNStr(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, 18, 43);

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        printf("LWHEEL->SPEED=0\r\n");
        printf("RWHEEL->SPEED=0\r\n");
        printf("LBRUSH->SPEED=0\r\n");
        printf("RBRUSH->SPEED=0\r\n");
        printf("MBRUSH->SPEED=0\r\n");
        printf("FAN->SPEED=0\r\n");
        printf("SENSOR->IFRD_LED=0\r\n");
        printf("SENSOR->B_SWITCH=0\r\n");
        printf("CHARGE->OFF\r\n");
        printf("IRDA->OFF\r\n");
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "PRESS RESUME TO RESUME TEST\r\n");
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "TEST PAUSED\r\n");
    }
}

void SweepRobot_PCBTestSetProc(void)
{
    WM_HWIN hItem;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
        gSwrbTestMode = SWRB_TEST_MODE_SET;
        gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;

        hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SNSET_BUTTON_SNSET);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
        BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);

        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START);

        SWRB_ListWheelLastItemPosGet(hWin_SWRB_SNSETTING);

        WM_HideWin(hWin_SWRB_MAIN);
        WM_ShowWin(hWin_SWRB_SNSETTING);
    }
}

void SweepRobot_TestSetSNPressedProc(void)
{
    WM_HWIN hItem;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
        gSwrbTestMode = SWRB_TEST_MODE_SET;
        gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;

        hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SNSET_BUTTON_SNSET);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
        BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);

        SWRB_ListWheelLastItemPosGet(hWin_SWRB_SNSETTING);

        WM_HideWin(hWin_SWRB_MAIN);
        WM_ShowWin(hWin_SWRB_SNSETTING);
    }
}

void SweepRobot_PCBTestStopProc(void)
{
    OS_CPU_SR cpu_sr;
    char *str;
    int i;

    if(gSwrbTestMode == SWRB_TEST_MODE_RUN || gSwrbTestMode == SWRB_TEST_MODE_PAUSE){

        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        SWRB_ValidTestTaskCntGet();
        SWRB_TestCheckboxEnable();

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, "START");
        BUTTON_Disp_Start_CHNStr(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, 18, 43);
        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_EXIT);
        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_SET);
        SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_STOP);
        
        str = mymalloc(SRAMIN, sizeof(char)*50);
        sprintf(str, "\r\nTest Stop Time:20%d/%d/%d %d:%d:%d\r\n",\
        rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
        myfree(SRAMIN, str);

        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "PRESS START TO START TEST\r\n");

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }

        SweepRobot_TestInitProc();
    }
}

void SweepRobot_PCBTestExitProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;

        SWRB_TestCheckboxEnable();

        mf_close();

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        SweepRobot_TestInitProc();
        printf("TEST->OFF\r\n");

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, "START");
        BUTTON_Disp_Start_CHNStr(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, 18, 43);

        MultiEdit_Set_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "PRESS START TO ENTER TEST MODE AND START TEST\r\n");

        WM_HideWin(hWin_SWRB_MAIN);
        WM_ShowWin(hWin_SWRB_START);
    }
}

void SweepRobot_TestInitProc(void)
{
    int i;

    SweepRobot_Charge24VOff();
    SweepRobot_KeyTestCtrlIdlePos();
    SweepRobot_CollisionCtrlOff(COLLISION_CHAN_ALL);
    SweepRobot_WheelFloatCtrlIdlePos();
    SweepRobot_AshTrayTestInsCtrlIdlePos();

    printf("TEST->ON\r\n");
    printf("CHARGE->OFF\r\n");
    printf("IRDA->OFF\r\n");
    printf("LWHEEL->SPEED=0\r\n");
    printf("RWHEEL->SPEED=0\r\n");
    printf("LBRUSH->SPEED=0\r\n");
    printf("RBRUSH->SPEED=0\r\n");
    printf("MBRUSH->SPEED=0\r\n");
    printf("FAN->SPEED=0\r\n");
    printf("SENSOR->IFRD_LED=0\r\n");
    printf("SENSOR->B_SWITCH=0\r\n");

    for(i=ID_MAIN_EDIT_U1;i<=ID_MAIN_EDIT_D8;i++){
        Edit_Set_Value(hWin_SWRB_MAIN, i, 0);
    }

    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_WHEEL, "WHEEL");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_BRUSH, "BRUSH");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, "FAN");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IFRD, "IFRD");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_COLLISION, "COLLISION");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_ASH_TRAY, "ASH TRAY");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_UNIWHEEL, "UNIWHEEL");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_KEY, "KEY");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IRDA, "IRDA");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_BUZZER, "BUZZER");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_RGB_LED, "RGB LED");
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_CHARGE, "CHARGE");

    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);
}

static void SWRB_ValidTestTaskCntGet(void)
{
    int i;
    WM_HWIN hItem;

    gSwrbTestValidTaskCnt = 0;
    for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
        if(CHECKBOX_GetState(hItem)){
            gSwrbTestValidTaskCnt++;
        }
    }
    gSwrbTestValidTaskCntTotal = gSwrbTestValidTaskCnt;
}

void SWRB_NextTestTaskResumePreAct(u8 taskPrio)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    OSTaskResume(taskPrio+1);
    OSTaskSuspend(OS_PRIO_SELF);
    OS_EXIT_CRITICAL();
}

static void SWRB_TestDataFileEncryptoProc(FunctionalState encryptoState)
{
    int i;
    FRESULT flErr;
    WM_HWIN hItem;

    if(ENABLE == encryptoState){
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "\r\n>>>Start Encrypting TestData<<<\r\n");

        for(i=ID_MAIN_BUTTON_START;i<=ID_MAIN_BUTTON_EXIT;i++){
            hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
            WM_DisableWindow(hItem);
        }

        flErr = SWRB_TestDataFileCrypt(EncryptMode);
//            SWRB_TestDataFileCrypt(DecryptMode);

        for(i=ID_MAIN_BUTTON_START;i<=ID_MAIN_BUTTON_EXIT;i++){
            hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
            WM_EnableWindow(hItem);
        }

        if(flErr == FR_OK){
            MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "\r\n***TestData Encrypting Finished*****\r\n");
        }else{
            MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, "\r\n***TestData Encrypting Fault*****\r\n");
        }
    }
}

void SWRB_NextTestTaskResumePostAct(u8 taskPrio)
{
    int i;
    char *str;
    OS_CPU_SR cpu_sr;

    gSwrbTestValidTaskCnt--;

    Progbar_Set_Percent();

    if(gSwrbTestValidTaskCnt){
        OS_ENTER_CRITICAL();
        OSTaskResume(taskPrio+1);
        OS_EXIT_CRITICAL();
    }else{
        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        SweepRobot_TestInitProc();

        RTC_GetDate(RTC_Format_BIN, &rtcDate);
        RTC_GetTime(RTC_Format_BIN, &rtcTime);
        SWRB_TestDataFileWriteDate("Test finish time", &rtcDate, &rtcTime);
        
        str = mymalloc(SRAMIN, sizeof(char)*50);
        sprintf(str, "\r\nTest Finish Time:20%d/%d/%d %d:%d:%d\r\n",\
        rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
        myfree(SRAMIN, str);

        str = "\r\n***TEST FINISHED***\r\n";
        SWRB_TestDataFileWriteString(str);
        
        /* Encrypt Test Data File when set enable */
        SWRB_TestDataFileEncryptoProc(DISABLE);

        SWRB_ValidTestTaskCntGet();
        SWRB_TestCheckboxEnable();

        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, "START");
        BUTTON_Disp_Start_CHNStr(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START, 18, 43);
        for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }
        
        TEST_LED_TASK_CB_DEREG();
        
        SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_STOP);
        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_EXIT);
        SWRB_WM_EnableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_SET);

        SWRB_ListWheelSNInc(&hWin_SWRB_SNSETTING);
    }

    OS_ENTER_CRITICAL();
    OSTaskSuspend(OS_PRIO_SELF);
    OS_EXIT_CRITICAL();
}

void SweepRobot_StartDlgPCBBtnClickProc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_PCB;

    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);

    SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_START);
    SWRB_WM_DisableWindow(hWin_SWRB_MAIN, ID_MAIN_BUTTON_STOP);

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_MAIN);
}

void SweepRobot_StartDlgPowerStationBtnClickPorc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_POWER_STATION;

    gSwrbTestRuningTaskPrio = SWRB_POWER_STATION_TEST_TASK_PRIO;
    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_POWER_STATION);
}

void SweepRobot_StartDlgDecryptoBtnClickProc(void)
{
    
}

void SweepRobot_PowerStationTestStartProc(void)
{
    char *str;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE || gSwrbTestMode == SWRB_TEST_MODE_PAUSE){

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        printf("TEST->ON\r\n");
        printf("POWER_STATION->ON\r\n");

        TEST_LED_TASK_CB_REG(SweepRobot_PowerStationIndicateBtnToggle);

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_LIGHTRED);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "PAUSE");
        BUTTON_Disp_Pause_CHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);
        SWRB_WM_EnableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_STOP);

        SWRB_WM_DisableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_EXIT);
        SWRB_WM_DisableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_SET);

        str = ">>>PowerStation<<<\r\nTest Start\r\n";
        MultiEdit_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_MULTIEDIT_MAIN, str);

        OSTaskResume(SWRB_POWER_STATION_TEST_TASK_PRIO);

    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        printf("POWER_STATION->OFF\r\n");

        TEST_LED_TASK_CB_DEREG();

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_USER_204153051);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "RESUME");
        BUTTON_Disp_Resume_CHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);

        str = ">>>PowerStation<<<\r\nTest Paused\r\n";
        MultiEdit_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_MULTIEDIT_MAIN, str);

        OSTaskSuspend(SWRB_POWER_STATION_TEST_TASK_PRIO);
    }else{

    }
}

void SweepRobot_PowerStationTestStopProc(void)
{
    int i;
    char *str;
    WM_HWIN hItem;
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode != SWRB_TEST_MODE_IDLE){
        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        gSwrbTestTaskRunCnt = 0;

        printf("POWER_STATION->OFF\r\n");
        printf("TEST->OFF\r\n");

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "START");
        BUTTON_Disp_Start_CHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);
        SWRB_WM_DisableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_STOP);

        SWRB_WM_EnableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_EXIT);
        SWRB_WM_EnableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_SET);
        TEST_LED_TASK_CB_DEREG();

        str = ">>>PowerStation<<<\r\nTest Stop\r\n";
        MultiEdit_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_MULTIEDIT_MAIN, str);

        for(i=ID_PS_CHECKBOX_LL;i<=ID_PS_CHECKBOX_24V;i++){
            hItem = WM_GetDialogItem(hWin_SWRB_POWER_STATION, i);
            WM_EnableWindow(hItem);
            Checkbox_Set_State(hWin_SWRB_POWER_STATION, i, 0);
            Checkbox_Set_Box_Back_Color(hWin_SWRB_POWER_STATION, i, GUI_LIGHTGRAY, CHECKBOX_CI_DISABLED);
            WM_DisableWindow(hItem);
        }

        OS_ENTER_CRITICAL();
        SweepRobot_PowerStationTestGraphClear();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_PowerStationTestExitProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;

        printf("TEST->OFF\r\n");
        WM_HideWin(hWin_SWRB_POWER_STATION);
        WM_ShowWin(hWin_SWRB_START);
    }
}
