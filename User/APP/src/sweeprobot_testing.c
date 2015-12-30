/******************** (C) COPYRIGHT 2007 EJE ***********************************************************
* File Name          : EJE_SWRB_TEST_DLG_Conf.h
* Author             : MeredithRowe@163.com
* Version            : V1.0
* Date               : 12/01/2015
* Description        : SweepRobot Test task creat and task control function
*********************************************************************************************************/

#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"

#include "usart.h"
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <string.h>

#define SWRB_TEST_ACQUIRED_DATA_LEN_MAX  60

static char* gLoginPassWord = "123";

u8 usartRxFlag = 0;
int usartRxNum = 0;

enum CryptoMode{

    DecryptMode,
    EncryptMode,
};
static char *gEncryptStr;

enum SWRB_TEST_SELECT gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;
enum SWRB_TEST_MODE gSwrbTestMode = SWRB_TEST_MODE_IDLE;
enum SWRB_TEST_RUN_STATE gSwrbTestRunState = SWRB_TEST_RUN_STATE_NORMAL;
enum SWRB_TEST_SET_STATE gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;
enum SWRB_TEST_TASK_PRIO gSwrbTestRuningTaskPrio;
FunctionalState gSwrbTestDataSaveState = ENABLE;
u32 gSwrbTestStateMap = 0;
u16 gSwrbTestTaskRunCnt = 0;
int gSwrbTestValidTaskCnt;
int gSwrbTestValidTaskCntTotal;
int gSwrbTestAcquiredData[SWRB_TEST_ACQUIRED_DATA_LEN_MAX] = {0};

static u8 aSwrbManulTestState[SWRB_TEST_STATE_BOUND] = { 0 };

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
static void SWRB_TestCtrlTask(void *pdata);
/* TODO: Add Exception Check */
//static void SWRB_ExceptionCheckTask(void *pdata);
static void SweepRobotTest_PCBTestInitProc(void);
static FRESULT SWRB_TestDataFileCrypt(enum CryptoMode mode);
static void SWRB_TestFinishProc(void);
static void SWRB_PCBTestWarningDlgHide(void);

#define TEST_LED_TASK_CB_REG(f)             do{gLedTaskCB=f;}while(0)
#define TEST_LED_TASK_CB_DEREG()            do{gLedTaskCB=NULL;}while(0)

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK RTC_TASK_STK[RTC_STK_SIZE];
OS_STK EMWIN_TASK_STK[EMWIN_STK_SIZE];
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
OS_STK SWRB_MANUL_TASK_STK[SWRB_MANUL_TEST_STK_SIZE];
OS_STK SWRB_STEPMOTOR_TASK_STK[SWRB_STEPMOTOR_TASK_STK_SIZE];

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
    OSTaskCreate(emWin_Maintask,(void*)0,(OS_STK*)&EMWIN_TASK_STK[EMWIN_STK_SIZE-1],EMWIN_TASK_PRIO);
    OS_EXIT_CRITICAL();

    OSTaskDel(OS_PRIO_SELF);
}

static void emWin_TaskInit(void)
{
    OS_CPU_SR cpu_sr;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);

//    MainTask();

    GUI_Init();

    GUI_EnableAlpha(ENABLE);

    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_MULTIBUF_Enable(ENABLE);

    OS_ENTER_CRITICAL();

    hWin_SWRB_SNSETTING = CreateSNSettingDLG();
    hWin_SWRB_TIMESETTING = CreateTimeSettingDLG();
    hWin_SWRB_TESTSEL = CreateTestSelSettingDLG();
    hWin_SWRB_LOGIN = CreateLoginDLG();
    hWin_SWRB_NUMPAD = CreateNumPadDLG();
    hWin_SWRB_MANUL = CreateManulTestDLG();
    hWin_SWRB_STEPMOTOR = CreatewinStepMotor();
    hWin_SWRB_PCBTEST = CreateEJE_SWRB_TEST_PCBTestDLG();
//    hWin_SWRB_WARNING = CreateWarningDLG();
    hWin_SWRB_POWER_STATION = CreateEJE_SWRB_TEST_PowerStationDLG();
#ifdef _SHOW_SLAM_DLG
    hWin_SWRB_SLAM = CreateEJE_SWRB_TEST_SLAMDLG();
#endif
    hWin_SWRB_START = CreateEJE_SWRB_TEST_StartDLG();

    OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(Rtc_Task,(void*)0,(OS_STK*)&RTC_TASK_STK[RTC_STK_SIZE-1],RTC_TASK_PRIO);
    OSTaskCreate(SWRB_TestCtrlTask,(void*)0,(OS_STK*)&SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE-1],SWRB_TEST_CTRL_TASK_PRIO);
    /* TODO: Add Exception Check */
//    OSTaskCreate(SWRB_ExceptionCheckTask, (void*)0,(OS_STK*)&SWRB_TEST_EXCEPTION_CHECK_TASK_STK[SWRB_TEST_EXCEPTION_CHECK_STK_SIZE-1],SWRB_TEST_EXCEPTION_CHECK_TASK_PRIO);

    OS_EXIT_CRITICAL();

    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_SNSETTING, ID_SNSET_LISTWHEEL_YEAR, ID_SNSET_LISTWHEEL_MONTH, ID_SNSET_LISTWHEEL_DATE);
    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_TIMESETTING, ID_TIMESET_LISTWHEEL_YEAR, ID_TIMESET_LISTWHEEL_MONTH, ID_TIMESET_LISTWHEEL_DAY);
}

void emWin_Maintask(void *pdata)
{
    emWin_TaskInit();

    while(1)
    {
        if(gSwrbTestMode == SWRB_TEST_MODE_SET){
            SWRB_SET_ListwheelSnapPosUpdate();
            SWRB_SET_EditTextUpdate();
        }

#ifdef _SHOW_SLAM_DLG
        WM_InvalidateWindow(hWin_SWRB_SLAM);
#endif

        GUI_Exec();
        OSTimeDlyHMSM(0,0,0,10);
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

        if(gSwrbTestSelectFlag == SWRB_TEST_SELECT_PCB)
            SWRB_RTC_TIME_Disp(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_DATE, &rtcDate, &rtcTime);
        else if(gSwrbTestSelectFlag == SWRB_TEST_SELECT_MANUL)
            SWRB_RTC_TIME_Disp(hWin_SWRB_MANUL, ID_MANUL_EDIT_DATE, &rtcDate, &rtcTime);
        else
            ;

        OSTimeDlyHMSM(0,0,1,0);
    }
}

void SWRB_TestDataFileWriteString(char *str)
{
    if(gSwrbTestDataSaveState){
        SWRB_TestDataFileOpen(FA_WRITE|FA_OPEN_ALWAYS);
        f_puts(str,file);
        f_close(file);
    }
}

void SWRB_TestDataFileWriteData(char *headstr, int data, u8 CRflag)
{
    char *dataStr;

    if(gSwrbTestDataSaveState){
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
}

void SWRB_TestDataFileWriteDate(char *headStr, RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    char *dateStr;

    if(gSwrbTestDataSaveState){
        dateStr = mymalloc(SRAMIN, sizeof(char)*40);
        *dateStr = 0;

        SWRB_TestDataFileOpen(FA_WRITE|FA_OPEN_ALWAYS);

        sprintf(dateStr, "\r\n%s:20%d/%d/%d %d:%d:%d\r\n", headStr, date->RTC_Year, date->RTC_Month, date->RTC_Date, time->RTC_Hours, time->RTC_Minutes, time->RTC_Seconds);
        f_puts(dateStr, file);
        f_close(file);

        myfree(SRAMIN, dateStr);
    }
}

FRESULT SWRB_TestDataFileCrypt(enum CryptoMode mode)
{
    FRESULT flErr;
    int i;
    int fileLength, leftFileLength;
    char *str;

    if(gSwrbTestDataSaveState){
        SWRB_TestDataFileOpen(FA_READ|FA_OPEN_ALWAYS);

        fileLength = f_size(file);
        leftFileLength = fileLength%8;

        str = mymalloc(SRAMIN, sizeof(char)*(8-leftFileLength));
        mymemset(str, '*', sizeof(char)*(8-leftFileLength));
        SWRB_TestDataFileWriteString(str);
        myfree(SRAMIN, str);

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
    }

    return flErr;
}

static void SWRB_TestDataFileEncryptoProc(FunctionalState encryptoState)
{
    int i;
    FRESULT flErr;
    WM_HWIN hItem;

    if(ENABLE == encryptoState){
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n>>>Start Encrypting TestData<<<\r\n");

        for(i=ID_PCBTEST_BUTTON_START;i<=ID_PCBTEST_BUTTON_EXIT;i++){
            hItem = WM_GetDialogItem(hWin_SWRB_PCBTEST, i);
            WM_DisableWindow(hItem);
        }

        flErr = SWRB_TestDataFileCrypt(EncryptMode);
//            SWRB_TestDataFileCrypt(DecryptMode);

        for(i=ID_PCBTEST_BUTTON_START;i<=ID_PCBTEST_BUTTON_EXIT;i++){
            hItem = WM_GetDialogItem(hWin_SWRB_PCBTEST, i);
            WM_EnableWindow(hItem);
        }

        if(flErr == FR_OK){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n***TestData Encrypting Finished*****\r\n");
        }else{
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n***TestData Encrypting Fault*****\r\n");
        }
    }
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
        case SWRB_TEST_SELECT_SLAM:
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Set(void)
{
    switch(gSwrbTestSelectFlag){
        case SWRB_TEST_SELECT_NONE:
            break;
        case SWRB_TEST_SELECT_PCB:
            SweepRobot_PCBTestLoginOKProc();
            break;
        case SWRB_TEST_SELECT_POWER_STATION:
            ;
            break;
        case SWRB_TEST_SELECT_SLAM:
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
        case SWRB_TEST_SELECT_SLAM:
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
        case SWRB_TEST_SELECT_SLAM:
            break;
    }
}

void SWRB_TestCtrlTask(void *pdata)
{
    u8 i;
    OS_CPU_SR cpu_sr;

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SweepRobotTest_PCBTestInitProc();
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PLEASE PRESS SET TO SET SERIAL NUMBER BEFORE TEST\r\n");

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

    OSTaskCreate(SweepRobot_ManulTestTask,(void*)0,(OS_STK*)&SWRB_MANUL_TASK_STK[SWRB_MANUL_TEST_STK_SIZE-1],SWRB_MANUL_TEST_TASK_PRIO);
    OSTaskSuspend(SWRB_MANUL_TEST_TASK_PRIO);
    
    OSTaskCreate(SweepRobot_StepMotorTask,(void*)0,(OS_STK*)&SWRB_STEPMOTOR_TASK_STK[SWRB_STEPMOTOR_TASK_STK_SIZE-1],SWRB_STEPMOTOR_TASK_PRIO);
    OSTaskSuspend(SWRB_STEPMOTOR_TASK_PRIO);

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

static void SWRB_PCBTestIndicateButtonToggle()
{
    SWRB_IndicateButtonToggle(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE);
}

void SweepRobot_PCBTestStartProc(void)
{
    char *str;
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE || gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "TEST RESUMED\r\n");
        }else{
            MultiEdit_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n");

            OS_ENTER_CRITICAL();
            SWRB_TestDataFileWriteSN();
            SWRB_TestDataFileWriteDate("Test Start Time", &rtcDate, &rtcTime);
            OS_EXIT_CRITICAL();

            str = mymalloc(SRAMIN, sizeof(char)*50);
            *str = 0;
            sprintf(str, "\r\nTest Start Time:20%d/%d/%d %d:%d:%d\r\n",\
                          rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
            myfree(SRAMIN, str);
            Progbar_Set_Value(0);
        }

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        SWRB_PCBTestCheckboxDisable();

        SWRB_ValidTestTaskCntGet();

        TEST_LED_TASK_CB_REG(SWRB_PCBTestIndicateButtonToggle);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTRED);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "PAUSE");
        BUTTON_DispPauseCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);
        printf("T->ON\r\n");

        OS_ENTER_CRITICAL();
        OSTaskResume(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        /* FIXME:  Task count would be wrong if click checkbox when uncomment this code */
//        SWRB_PCBTestCheckboxEnable();

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_GREEN);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_USER_204153051);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "RESUME");
        BUTTON_DispResumeCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        printf("LW->SPD=0\r\n");
        printf("RW->SPD=0\r\n");
        printf("LB->SPD=0\r\n");
        printf("RB->SPD=0\r\n");
        printf("MB->SPD=0\r\n");
        printf("FAN->SPD=0\r\n");
        printf("SNSR->IFRD=0\r\n");
        printf("SNSR->BSWC=0\r\n");
        printf("CRG->OFF\r\n");
        printf("IRDA->OFF\r\n");
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PRESS RESUME TO RESUME TEST\r\n");
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "TEST PAUSED\r\n");
    }
}

void SweepRobot_PCBTestLoginProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestMode = SWRB_TEST_MODE_SET;

        Text_Set_Text(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, "Please Input Password");
        Text_Set_Color(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, GUI_BLACK);

        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
        SWRB_PCBTestCheckboxDisable();

        WM_ShowWin(hWin_SWRB_LOGIN);
        WM_BringToTop(hWin_SWRB_LOGIN);
    }
}

void SweepRobot_PCBTestLoginOKProc(void)
{
    WM_HWIN hItem;
    char *str;

    if(gSwrbTestMode == SWRB_TEST_MODE_SET){

        hItem = WM_GetDialogItem(hWin_SWRB_LOGIN, ID_LOGIN_EDIT_PASSWORD);

        str = mymalloc(SRAMIN, sizeof(char)*11);
        *str = 0;
        Edit_Get_Text(hWin_SWRB_LOGIN, ID_LOGIN_EDIT_PASSWORD, str);

        if(!(strcmp(str,gLoginPassWord))){

            gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;

            Text_Set_Text(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, "Please Input Password");
            Text_Set_Color(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, GUI_BLACK);

            hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SNSET_BUTTON_SNSET);
            BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
            BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
            BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);

            SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START);
            SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
            SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
            SWRB_PCBTestCheckboxEnable();

            SWRB_ListWheelLastItemPosGet(hWin_SWRB_SNSETTING);

            WM_HideWin(hWin_SWRB_LOGIN);
            WM_HideWin(hWin_SWRB_PCBTEST);
            WM_ShowWin(hWin_SWRB_SNSETTING);
        }else{
            Text_Set_Text(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, "Password Error");
            Text_Set_Color(hWin_SWRB_LOGIN, ID_LOGIN_TEXT_PASSWORD, GUI_RED);
        }
        myfree(SRAMIN, str);
    }
}

void SweepRobot_PCBTestLoginCancelProc(void)
{
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
    SWRB_PCBTestCheckboxEnable();

    WM_HideWin(hWin_SWRB_LOGIN);
    WM_ShowWin(hWin_SWRB_PCBTEST);
}

void SweepRobot_PCBTestLoginEditProc(WM_MESSAGE *pMsg)
{
    SWRB_LastCallNumpadEditSave(pMsg);
    WM_ShowWin(hWin_SWRB_NUMPAD);
    WM_BringToTop(hWin_SWRB_NUMPAD);
}

void SweepRobot_PCBTestNumPadOKProc(void)
{
    char *str;

    str = mymalloc(SRAMIN, sizeof(char)*10);
    SWRB_NumpadInputNumGet(str);
    Edit_Set_Text(SWRB_LastCallNumpadEditWinGet(), SWRB_LastCallNumpadEditIdGet(), str);
    myfree(SRAMIN, str);

    WM_HideWin(hWin_SWRB_NUMPAD);
    WM_BringToBottom(hWin_SWRB_NUMPAD);
    WM_BringToTop(hWin_SWRB_PCBTEST);
    WM_BringToTop(hWin_SWRB_LOGIN);
}

void SweepRobotTest_SetSNPressedProc(void)
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

        WM_HideWin(hWin_SWRB_PCBTEST);
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

        if(gSwrbTestRunState == SWRB_TEST_RUN_STATE_ERROR){
            gSwrbTestRunState = SWRB_TEST_RUN_STATE_NORMAL;

            SWRB_PCBTestWarningDlgHide();
        }

        SWRB_PCBTestCheckboxEnable();

        TEST_LED_TASK_CB_DEREG();

        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "START");
        BUTTON_DispStartCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);
        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);

        str = mymalloc(SRAMIN, sizeof(char)*50);
        sprintf(str, "\r\nTest Stop Time:20%02d/%02d/%02d %02d:%02d:%02d\r\n",\
        rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
        myfree(SRAMIN, str);

        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PRESS START TO START TEST\r\n");

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        for(i=ID_PCBTEST_CHECKBOX_WHEEL;i<ID_PCBTEST_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }

        SweepRobotTest_PCBTestInitProc();
    }
}

void SweepRobot_PCBTestExitProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;

        SWRB_PCBTestCheckboxEnable();

        mf_close();

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        SweepRobotTest_PCBTestInitProc();
        printf("T->OFF\r\n");

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "START");
        BUTTON_DispStartCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);

        MultiEdit_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PRESS START TO ENTER TEST MODE AND START TEST\r\n");

        WM_HideWin(hWin_SWRB_PCBTEST);
        WM_ShowWin(hWin_SWRB_START);
    }
}

static void SweepRobot_PCBTestCtrlReset(void)
{
    SweepRobot_Charge24VOff();
    SweepRobot_KeyTestCtrlIdlePos();
    SweepRobot_CollisionCtrlOff(COLLISION_CHAN_ALL);
    SweepRobot_WheelFloatCtrlMoveToIdlePos();
    SweepRobot_AshTrayTestInsCtrlMoveToIdlePos();
}

static void SweepRobot_PCBTestGUIReset(void)
{
    int i;

    printf("T->ON\r\n");
    printf("CRG->OFF\r\n");
    printf("IRDA->OFF\r\n");
    printf("LW->SPD=0\r\n");
    printf("RW->SPD=0\r\n");
    printf("LB->SPD=0\r\n");
    printf("RB->SPD=0\r\n");
    printf("MB->SPD=0\r\n");
    printf("FAN->SPD=0\r\n");
    printf("SNSR->IFRD=0\r\n");
    printf("SNSR->BSWC=0\r\n");

    for(i=ID_PCBTEST_EDIT_U1;i<=ID_PCBTEST_EDIT_D8;i++){
        Edit_Set_Value(hWin_SWRB_PCBTEST, i, 0);
    }

    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL, "WHEEL");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, "BRUSH");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN, "FAN");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, "IFRD");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION, "COLLISION");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_ASH_TRAY, "ASH TRAY");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_UNIWHEEL, "UNIWHEEL");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY, "KEY");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IRDA, "IRDA");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BUZZER, "BUZZER");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_RGB_LED, "RGB LED");
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_CHARGE, "CHARGE");

    Progbar_Set_Value(0);
}

void SweepRobotTest_PCBTestInitProc(void)
{
    SweepRobot_PCBTestCtrlReset();

    SweepRobot_PCBTestGUIReset();

    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);
}

void SWRB_ValidTestTaskCntGet(void)
{
    int i;
    WM_HWIN hItem;

    gSwrbTestValidTaskCnt = 0;
    for(i=ID_PCBTEST_CHECKBOX_WHEEL;i<ID_PCBTEST_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_PCBTEST, i);
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

void SWRB_NextTestTaskResumePostAct(u8 taskPrio)
{
    OS_CPU_SR cpu_sr;

    gSwrbTestValidTaskCnt--;

    Progbar_Set_Percent();

    if(gSwrbTestValidTaskCnt){
        OS_ENTER_CRITICAL();
        OSTaskResume(taskPrio+1);
        OS_EXIT_CRITICAL();
    }else{
        SWRB_TestFinishProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR

#else
    OS_ENTER_CRITICAL();
    OSTaskSuspend(taskPrio);
    OS_EXIT_CRITICAL();
#endif
}

static void SWRB_PCBTestWarningDlgHide(void)
{
    WM_Set_Y_Size(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, 330);

    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START);

//    WM_HideWin(hWin_SWRB_WARNING);
    WM_DeleteWindow(hWin_SWRB_WARNING);
}

void SWRB_PCBTestWarningDLGReTestProc(void)
{
    gSwrbTestRunState = SWRB_TEST_RUN_STATE_RETEST;

    SWRB_PCBTestWarningDlgHide();

    OSTaskResume(gSwrbTestRuningTaskPrio);
}

void SWRB_PCBTestWarningDLGSkipProc(void)
{
    gSwrbTestRunState = SWRB_TEST_RUN_STATE_NORMAL;

    SWRB_PCBTestWarningDlgHide();

    SWRB_NextTestTaskResumePostAct(gSwrbTestRuningTaskPrio);
}

void SWRB_TestTaskErrorAct(void)
{
    OS_CPU_SR cpu_sr;

    gSwrbTestRunState = SWRB_TEST_RUN_STATE_ERROR;

    WM_Set_Y_Size(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, 270);

    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START);

    hWin_SWRB_WARNING = CreateWarningDLG();
//    WM_ShowWin(hWin_SWRB_WARNING);
//    WM_BringToTop(hWin_SWRB_WARNING);

    OS_ENTER_CRITICAL();
    OSTaskSuspend(gSwrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
}

static void SWRB_TestFinishProc(void)
{
    int i;
    char *str;

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SweepRobot_PCBTestCtrlReset();

    RTC_GetDate(RTC_Format_BIN, &rtcDate);
    RTC_GetTime(RTC_Format_BIN, &rtcTime);
    SWRB_TestDataFileWriteDate("Test finish time", &rtcDate, &rtcTime);

    SWRB_TestDUTWriteSN();

    str = mymalloc(SRAMIN, sizeof(char)*50);
    sprintf(str, "\r\nTest Finish Time:20%02d/%02d/%02d %02d:%02d:%02d\r\n",\
                    rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date,\
                    rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
    myfree(SRAMIN, str);

    str = "\r\n***TEST FINISHED***\r\n";
    SWRB_TestDataFileWriteString(str);

    /* Encrypt Test Data File when set enable */
    SWRB_TestDataFileEncryptoProc(DISABLE);

    SWRB_PCBTestCheckboxEnable();

    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
    Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_LIGHTGRAY);
    Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTBLUE);
//    Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "START");
    BUTTON_DispStartCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);
    for(i=ID_PCBTEST_CHECKBOX_WHEEL;i<ID_PCBTEST_CHECKBOX_BOUND;i++){
        Checkbox_Set_Text_Color(i, GUI_BLACK);
    }

    SweepRobot_PCBTestGUIReset();

    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);

    SWRB_ListWheelSNInc(hWin_SWRB_SNSETTING);

    /* FIXME: enter hardfault when dereg led task cb function */
    TEST_LED_TASK_CB_DEREG();

    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);
    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
}

void SweepRobot_StartDlgPCBBtnClickProc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_PCB;

    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);

    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START);
    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_PCBTEST);
}

void SweepRobot_StartDlgPowerStationBtnClickPorc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_POWER_STATION;

    gSwrbTestRuningTaskPrio = SWRB_POWER_STATION_TEST_TASK_PRIO;
    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_POWER_STATION);
}

void SweepRobot_StartDlgManulBtnClickProc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_MANUL;

    gSwrbTestRuningTaskPrio = SWRB_MANUL_TEST_TASK_PRIO;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_MANUL);
}

#ifdef _SHOW_SLAM_DLG
void SweepRobot_StartDlgSLAMBtnClickProc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_SLAM;

    gSwrbTestRuningTaskPrio = SWRB_SLAM_MONITOR_TASK_PRIO;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_SLAM);
}
#endif

void SweepRobot_StartDlgStepMotorBtnClickProc(void)
{
    gSwrbTestSelectFlag = SWRB_TEST_SELECT_STEP_MOTOR;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_STEPMOTOR);
}

void SweepRobot_StartDlgDecryptoBtnClickProc(void)
{

}

static void SweepRobot_PSTestIndicateBtnToggle(void)
{
    SWRB_IndicateButtonToggle(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_INDICATE);
}

void SweepRobot_PowerStationTestStartProc(void)
{
    char *str;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE || gSwrbTestMode == SWRB_TEST_MODE_PAUSE){

        if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
            SweepRobot_PowerStationTestGraphClear();
        }

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        printf("T->ON\r\n");
        printf("PS->ON\r\n");

        TEST_LED_TASK_CB_REG(SweepRobot_PSTestIndicateBtnToggle);

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_LIGHTRED);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "PAUSE");
        BUTTON_DispPauseCHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);
        SWRB_WM_EnableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_STOP);

        SWRB_WM_DisableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_EXIT);
        SWRB_WM_DisableWindow(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_SET);

        str = ">>>PowerStation<<<\r\nTest Start\r\n";
        MultiEdit_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_MULTIEDIT_MAIN, str);

        OSTaskResume(SWRB_POWER_STATION_TEST_TASK_PRIO);

    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        printf("PS->OFF\r\n");

        TEST_LED_TASK_CB_DEREG();

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_USER_204153051);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "RESUME");
        BUTTON_DispResumeCHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);

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

        printf("PS->OFF\r\n");
        printf("T->OFF\r\n");

        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, "START");
        BUTTON_DispStartCHNStr(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_START, 18, 43);
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
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_PowerStationTestExitProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;

        printf("T->OFF\r\n");
        WM_HideWin(hWin_SWRB_POWER_STATION);
        WM_ShowWin(hWin_SWRB_START);
    }
}

static void SWRB_ManulTestIndicateButtonToggle()
{
    SWRB_IndicateButtonToggle(hWin_SWRB_MANUL, ID_MANUL_BUTTON_INDICATE);
}

void SweepRobot_ManulStartProc(void)
{
    int i;
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        for(i=0;i<USART_REC_LEN;i++)
            USART_RX_BUF[i] = 0;

//        Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, "Stop");
        BUTTON_DispStopCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, 18, 43);
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, GUI_LIGHTRED);

        TEST_LED_TASK_CB_REG(SWRB_ManulTestIndicateButtonToggle);

        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET);
        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);

        printf("T->ON\r\n");
//        printf("LW->SPD=25\r\n");
//        printf("RW->SPD=25\r\n");
//        printf("LB->SPD=25\r\n");
//        printf("RB->SPD=25\r\n");
//        printf("MB->SPD=5\r\n");
//        printf("FAN->SPD=25\r\n");
        printf("SNSR->IFRD=1\r\n");
        printf("IRDA->ON\r\n");

        OS_ENTER_CRITICAL();
        OSTaskResume(SWRB_MANUL_TEST_TASK_PRIO);
        OS_EXIT_CRITICAL();
    }else{

        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

//        Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, "Start");
        BUTTON_DispStartCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, 18, 43);
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, GUI_LIGHTBLUE);

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_INDICATE, GUI_LIGHTGRAY);

        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET);
        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);

//        printf("LW->SPD=0\r\n");
//        printf("RW->SPD=0\r\n");
//        printf("LB->SPD=0\r\n");
//        printf("RB->SPD=0\r\n");
//        printf("MB->SPD=0\r\n");
//        printf("FAN->SPD=0\r\n");
        printf("SNSR->IFRD=0\r\n");
        printf("IRDA->OFF\r\n");
        printf("IRDA->ERS\r\n");

        gSwrbTestTaskRunCnt = 0;

        OS_ENTER_CRITICAL();
        OSTaskSuspend(SWRB_MANUL_TEST_TASK_PRIO);
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulSetProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestMode = SWRB_TEST_MODE_SET;

        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, GUI_BLUE);

        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START);
        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);
    }else{
        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, GUI_LIGHTGRAY);

        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START);
        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);
    }
}

void SweepRobot_ManulResetProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        printf("IRDA->ERS\r\n");

        SweepRobot_ManulTestDataReset();
    }
}

void SweepRobot_ManulExitProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbTestSelectFlag = SWRB_TEST_SELECT_NONE;

        printf("T->OFF\r\n");
        WM_HideWin(hWin_SWRB_MANUL);
        WM_ShowWin(hWin_SWRB_START);
    }
}

void SweepRobot_ManulWheelProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    if(aSwrbManulTestState[SWRB_TEST_STATE_WHEEL]){
        printf("WHL->DIR=1\r\n");
        printf("LW->SPD=25\r\n");
        printf("RW->SPD=25\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 0;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_WHEEL, GUI_LIGHTBLUE);
    }else{
        printf("LW->SPD=0\r\n");
        printf("RW->SPD=0\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 1;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_WHEEL, GUI_LIGHTGRAY);
    }

    OS_EXIT_CRITICAL();
}

void SweepRobot_ManulBrushProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    if(aSwrbManulTestState[SWRB_TEST_STATE_WHEEL]){
        printf("LB->SPD=30\r\n");
        printf("RB->SPD=30\r\n");
        printf("MB->SPD=40\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 0;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BRUSH, GUI_LIGHTBLUE);
    }else{
        printf("LB->SPD=0\r\n");
        printf("RB->SPD=0\r\n");
        printf("MB->SPD=0\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 1;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BRUSH, GUI_LIGHTGRAY);
    }

    OS_EXIT_CRITICAL();
}

void SweepRobot_ManulFanProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    if(aSwrbManulTestState[SWRB_TEST_STATE_FAN]){
        printf("FAN->SPD=25\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_FAN] = 0;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_FAN, GUI_LIGHTBLUE);
    }else{
        printf("FAN->SPD=0\r\n");
        aSwrbManulTestState[SWRB_TEST_STATE_FAN] = 1;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_FAN, GUI_LIGHTGRAY);
    }

    OS_EXIT_CRITICAL();
}

void SweepRobot_ManulBuzzerProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    if(aSwrbManulTestState[SWRB_TEST_STATE_BUZZER]){
        
        aSwrbManulTestState[SWRB_TEST_STATE_BUZZER] = 0;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_LIGHTBLUE);
    }else{
        
        aSwrbManulTestState[SWRB_TEST_STATE_BUZZER] = 1;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_LIGHTGRAY);
    }

    OS_EXIT_CRITICAL();
}

void SweepRobot_ManulRGBLEDProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    if(aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED]){
        
        aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED] = 0;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_LIGHTBLUE);
    }else{
        
        aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED] = 1;
        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_LIGHTGRAY);
    }

    OS_EXIT_CRITICAL();
}

#ifdef _SHOW_SLAM_DLG
void SweepRobot_SLAMStartProc(void)
{
    
}

void SweepRobot_SLAMResetProc(void)
{

}

void SweepRobot_SLAMStopProc(void)
{

}

void SweepRobot_SLAMExitProc(void)
{
    
}
#endif
