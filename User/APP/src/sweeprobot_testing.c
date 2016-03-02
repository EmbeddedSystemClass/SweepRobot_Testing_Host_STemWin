/******************** (C) COPYRIGHT 2007 EJE ***********************************************************
* File Name          : EJE_SWRB_TEST_DLG_Conf.h
* Author             : Meredith Rowe
* Version            : V1.0
* Date               : 12/01/2015
* Description        : SweepRobot Test task creat and task control function
*********************************************************************************************************/

#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"

#include "usart.h"
#include "usbh_usr.h"
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <string.h>

#define SWRB_TEST_ACQUIRED_DATA_LEN_MAX  60

extern USBH_HOST  USB_Host;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

u8 usartRxFlag = 0;
int usartRxNum = 0;

enum SWRB_DIALOG_SELECT gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;
enum SWRB_TEST_MODE gSwrbTestMode = SWRB_TEST_MODE_IDLE;
enum SWRB_TEST_MANUL_SUB_MODE gSwrbTestManulSubMode = SWRB_TEST_MANUL_SUB_MODE_AUTO;
enum SWRB_TEST_RUN_STATE gSwrbTestRunState = SWRB_TEST_RUN_STATE_NORMAL;
enum SWRB_TEST_SET_SELECT gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_SN;
enum SWRB_TEST_TASK_PRIO gSwrbTestRuningTaskPrio;

FunctionalState gSwrbTestSDCardInsertState = DISABLE;
FunctionalState gSwrbTestUDiskInsertState = DISABLE;
FunctionalState gSwrbTestUDiskInsertCmpSkipFlag = DISABLE;
u32 gSwrbTestStateMap = 0;
u16 gSwrbTestTaskRunCnt = 0;
u32 gSwrbTestTotalCnt = 0;
int gSwrbTestValidTaskCnt;
int gSwrbTestValidTaskCntTotal;
int gSwrbTestAcquiredData[SWRB_TEST_ACQUIRED_DATA_LEN_MAX] = {0};

static u8 aSwrbManulTestState[SWRB_TEST_STATE_BOUND] = { 0 };

typedef void (*TestCBFunc_t)(void);
static TestCBFunc_t gLedTaskCB = NULL;

#ifdef USE_KEY_BUTTON
static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;
#endif

static RTC_TimeTypeDef rtcTime;
static RTC_DateTypeDef rtcDate;

static char *gStrCrypt;

static void Start_Task(void *pdata);
#ifdef _USE_SELF_TESTING
static void SelfTest_Task(void *pdata);
#endif
static void emWin_Maintask(void *pdata);
static void USB_Host_Task(void *pdata);
static void Touch_Task(void *pdata);
static void Led_Task(void *pdata);
#ifdef USE_KEY_BUTTON
static void Key_Task(void *pdata);
#endif
static void Rtc_Task(void *pdata);
static void SWRB_TestCtrlTask(void *pdata);
static void SweepRobot_PCBTestCheckboxTextReset(void);
static void SweepRobotTest_PCBTestInitProc(void);
static void SWRB_TestFinishProc(void);
static void SWRB_PCBTestWarningDlgHide(void);
static void SweepRobot_ManulTestCtrlReset(void);
static void SweepRobot_ManulTestAutoModeValidTaskStateDisp(void);
static void SweepRobot_ManulTestManulModeTaskStateReset(void);

#define TEST_LED_TASK_CB_REG(f)             do{gLedTaskCB=f;}while(0)
#define TEST_LED_TASK_CB_DEREG()            do{gLedTaskCB=NULL;}while(0)

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
#ifdef _USE_KEY_BUTTON
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
#endif
OS_STK RTC_TASK_STK[RTC_STK_SIZE];
OS_STK EMWIN_TASK_STK[EMWIN_STK_SIZE];
OS_STK USB_HOST_TASK_STK[USB_HOST_STK_SIZE];
OS_STK LED_TASK_STK[LED_STK_SIZE];
OS_STK SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE];
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
OS_STK SWRB_MANUL_TASK_STK[SWRB_MANUL_TEST_STK_SIZE];
OS_STK SWRB_FRONT_IFRD_TASK_STK[SWRB_FRONT_IFRD_TASK_STK_SIZE];
#ifdef _USE_SELF_TESTING
    OS_STK SWRB_SELF_TEST_TASK_STK[SWRB_SELF_TEST_TASK_STK_SIZE];
#endif

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
    OSTaskCreate(USB_Host_Task,(void*)0,(OS_STK*)&USB_HOST_TASK_STK[USB_HOST_STK_SIZE-1],USB_HOST_TASK_PRIO);
#ifdef _USE_SELF_TESTING
    /* Self Test Task for System self testing */
    OSTaskCreate(SelfTest_Task,(void*)0,(OS_STK*)&SWRB_SELF_TEST_TASK_STK[SWRB_SELF_TEST_TASK_STK_SIZE-1],SELF_TEST_TASK_PRIO);
    OSTaskSuspend(SELF_TEST_TASK_PRIO);
#endif
    OS_EXIT_CRITICAL();

    OSTaskDel(OS_PRIO_SELF);
}

#ifdef _USE_SELF_TESTING
static void SelfTest_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;

    while(1){

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            SweepRobot_PCBTestStartBtnProc();
        }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
            SweepRobot_ManulStartBtnProc();
        }

        OS_ENTER_CRITICAL();
        OSTaskSuspend(OS_PRIO_SELF);
        OS_EXIT_CRITICAL();

        OSTimeDlyHMSM(0,0,1,0);
    }
}
#endif

static void emWin_TaskInit(void)
{
    OS_CPU_SR cpu_sr;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
    GUI_Init();

    GUI_EnableAlpha(ENABLE);

    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_MULTIBUF_Enable(ENABLE);

    OS_ENTER_CRITICAL();

#ifdef USE_RMT_CTRL
    hWin_SWRB_CONTROL = CreatewinCtrlDLG();
#endif
    hWin_SWRB_SNSET = CreateSNSettingDLG();
    hWin_SWRB_TIMESET = CreateTimeSettingDLG();
    hWin_SWRB_TESTSEL = CreateTestSelSettingDLG();
    hWin_SWRB_LOGIN = CreateLoginDLG();
#ifdef USE_NUMPAD
    hWin_SWRB_NUMPAD = CreateNumPadDLG();
#endif
#ifdef USE_DECRYPTO
    hWin_SWRB_DECRYPTO = CreatewinDecryptoDLG();
#endif
    hWin_SWRB_MANUL = CreateManulTestDLG();
#ifdef USE_RELAY_CTRL
    hWin_SWRB_RELAY = CreatewinRelayDLG();
#endif
#ifdef USE_STEPMOTOR_CTRL
    hWin_SWRB_STEPMOTOR = CreatewinStepMotorDLG();
#endif
#ifdef USE_STEERMOTOR_CTRL
    hWin_SWRB_STEERMOTOR = CreatewinSteerMotorDLG();
#endif
    hWin_SWRB_PCBTEST = CreateEJE_SWRB_TEST_PCBTestDLG();
#ifdef USE_SLAM
    hWin_SWRB_SLAM = CreateEJE_SWRB_TEST_SLAMDLG();
#endif
    hWin_SWRB_START = CreateEJE_SWRB_TEST_StartDLG();
//    hWin_SWRB_WARNING = CreateWarningDLG();

    OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(Rtc_Task,(void*)0,(OS_STK*)&RTC_TASK_STK[RTC_STK_SIZE-1],RTC_TASK_PRIO);
    OSTaskCreate(SWRB_TestCtrlTask,(void*)0,(OS_STK*)&SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE-1],SWRB_TEST_CTRL_TASK_PRIO);

    OS_EXIT_CRITICAL();

    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_SNSET, ID_SNSET_LISTWHEEL_YEAR, ID_SNSET_LISTWHEEL_MONTH, ID_SNSET_LISTWHEEL_DATE);
    SWRB_ListWheelRTCDateUpdate(hWin_SWRB_TIMESET, ID_TIMESET_LISTWHEEL_YEAR, ID_TIMESET_LISTWHEEL_MONTH, ID_TIMESET_LISTWHEEL_DAY);
}

void emWin_Maintask(void *pdata)
{
    emWin_TaskInit();

    while(1)
    {
        switch(gSwrbDialogSelectFlag){
            case SWRB_DIALOG_SELECT_SET:
                SWRB_SET_ListwheelSnapPosUpdate();
                SWRB_SET_EditTextUpdate();
                break;
#ifdef USE_SLAM
            case SWRB_DIALOG_SELECT_SLAM:
                WM_InvalidateWindow(hWin_SWRB_SLAM);
                break;
#endif
            default:break;
        }

        GUI_Exec();
        OSTimeDlyHMSM(0,0,0,10);
    }
}

void USB_Host_Task(void *pdata)
{
    while(1)
    {
        USBH_Process(&USB_OTG_Core, &USB_Host);
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

#ifdef USE_KEY_BUTTON
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
#endif

void Rtc_Task(void *pdata)
{
    while(1){

        RTC_GetDate(RTC_Format_BIN, &rtcDate);
        RTC_GetTime(RTC_Format_BIN, &rtcTime);

        switch(gSwrbDialogSelectFlag){
            case SWRB_DIALOG_SELECT_PCB:
                SWRB_RTC_TIME_Disp(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_DATE, &rtcDate, &rtcTime);
                break;
            case SWRB_DIALOG_SELECT_MANUL:
                SWRB_RTC_TIME_Disp(hWin_SWRB_MANUL, ID_MANUL_EDIT_DATE, &rtcDate, &rtcTime);
                break;
            case SWRB_DIALOG_SELECT_NONE:
                SWRB_RTC_TIME_Disp(hWin_SWRB_START, ID_START_EDIT_DATE, &rtcDate, &rtcTime);
                break;
            case SWRB_DIALOG_SELECT_SET:
                SWRB_RTC_TIME_Disp(hWin_SWRB_TIMESET, ID_TIMESET_EDIT_ACTVALUE, &rtcDate, &rtcTime);
                break;
            default:break;
        }

        OSTimeDlyHMSM(0,0,1,0);
    }
}

void SWRB_TestDataFileWriteString(char *str)
{
    FRESULT flErr;
    u8 cnt;

    flErr = flErr;

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        f_puts(str,file);
        do{
            flErr = f_sync(file);
        }while(flErr!=FR_OK && ++cnt<10);
    }
}

void SWRB_TestDataFileWriteData(char *headstr, int data, u8 CRflag)
{
    FRESULT flErr;
    u8 cnt;
    char *dataStr;


    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        dataStr = mymalloc(SRAMIN, sizeof(char)*10);
        mymemset(dataStr, 0, sizeof(char)*10);
        if(CRflag){
            sprintf(dataStr, "%s%d\r\n", headstr, data);
        }else{
            sprintf(dataStr, "%s%d", headstr, data);
        }
        f_puts(dataStr,file);
        do{
            flErr = f_sync(file);
        }while(flErr!=FR_OK && ++cnt<10);
        myfree(SRAMIN, dataStr);
    }
}

void SWRB_TestDataFileWriteDate(char *headStr, RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    FRESULT flErr;
    int cnt;
    char *dateStr;

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        dateStr = mymalloc(SRAMIN, sizeof(char)*40);
        mymemset(dateStr, 0, sizeof(char)*40);

        sprintf(dateStr, "\r\n%s:20%d/%02d/%02d %02d:%02d:%02d\r\n", headStr, date->RTC_Year, date->RTC_Month, date->RTC_Date, time->RTC_Hours, time->RTC_Minutes, time->RTC_Seconds);
        f_puts(dateStr, file);
        do{
            flErr = f_sync(file);
        }while(flErr!=FR_OK && ++cnt<10);
        myfree(SRAMIN, dateStr);
    }
}

int SWRB_TestDataFileCrypt(enum CryptoMode mode)
{
    FRESULT flErr;
    int i,cnt;
    int fileLength;

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
#ifdef USE_DECRYPTO
        if(mode == DecryptMode && gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_DECRYPTO){
            MultiEdit_Set_Text(hWin_SWRB_DECRYPTO, ID_DECRYPTO_MULTIEDIT_MAIN, "Test Data Decrypting...");
        }
#endif

        fileLength = f_size(file);
        if(fileLength>>3){
            gStrCrypt = mymalloc(SRAMIN, sizeof(char)*10);

            for(i=0;i<(fileLength>>3);i++){
                cnt = 0;
                do{
                    flErr = f_lseek(file, 8*i);
                    mymemset(gStrCrypt, 0, sizeof(char)*10);
                    flErr = f_read(file, gStrCrypt, 8, &br);
                }while(flErr!=FR_OK && ++cnt<10);

                if(mode == EncryptMode){
                    SWRB_ByteEncrypt(gStrCrypt);

                    cnt = 0;
                    do{
                        f_lseek(file, 8*i);
                        flErr = f_write(file, gStrCrypt, 8, &bw);
                        f_sync(file);
                    }while(flErr!=FR_OK && ++cnt<10);
                }else{
                    SWRB_ByteDecrypt(gStrCrypt);
#ifdef USE_DECRYPTO
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_DECRYPTO){
                        MultiEdit_Add_Text(hWin_SWRB_DECRYPTO, ID_DECRYPTO_MULTIEDIT_MAIN, gStrCrypt);
                    }
#endif
                }
            }
            myfree(SRAMIN, gStrCrypt);
        }else{
            return -1;
        }
        return 0;
    }else{
        return -1;
    }
}

static void SWRB_TestDataFileCryptoProc(FunctionalState encryptoState)
{
    int i;

    if(ENABLE == encryptoState){
        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n>Start Encrypting TestData...\r\n");

            for(i=ID_PCBTEST_BUTTON_START;i<=ID_PCBTEST_BUTTON_EXIT;i++){
                SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, i);
            }

            /* Test Data Encrypt or Decrypt Process */
            if(SWRB_TestDataFileCrypt(EncryptMode)){
                MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n>TestData Encrypting Fault\r\n");
            }else{
                MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "\r\n>TestData Encrypting Finished\r\n");
            }

            for(i=ID_PCBTEST_BUTTON_START;i<=ID_PCBTEST_BUTTON_EXIT;i++){
                if(i!=ID_PCBTEST_BUTTON_SET){
                    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, i);
                }
            }
        }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
            SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START);

            /* Test Data Encrypt or Decrypt Process */
            SWRB_TestDataFileCrypt(EncryptMode);

            SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START);
        }
    }
}

#ifdef USE_KEY_BUTTON
static void SWRB_TEST_BUTTON_CTRL_Start(void)
{
    switch(gSwrbDialogSelectFlag){
        case SWRB_DIALOG_SELECT_NONE:
            break;
        case SWRB_DIALOG_SELECT_PCB:
            SweepRobot_PCBTestStartBtnProc();
            break;
        case SWRB_DIALOG_SELECT_MANUL:

            break;
        case SWRB_DIALOG_SELECT_SLAM:
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Set(void)
{
    switch(gSwrbDialogSelectFlag){
        case SWRB_DIALOG_SELECT_NONE:
            break;
        case SWRB_DIALOG_SELECT_PCB:
            break;
        case SWRB_DIALOG_SELECT_MANUL:

            break;
        case SWRB_DIALOG_SELECT_SLAM:
            ;
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Stop(void)
{
    switch(gSwrbDialogSelectFlag){
        case SWRB_DIALOG_SELECT_NONE:
            SweepRobot_StartDlgPCBBtnClickProc();
            break;
        case SWRB_DIALOG_SELECT_PCB:
            SweepRobot_PCBTestStopBtnProc();
            break;
        case SWRB_DIALOG_SELECT_MANUL:

            break;
        case SWRB_DIALOG_SELECT_SLAM:
            break;
    }
}

static void SWRB_TEST_BUTTON_CTRL_Exit(void)
{
    switch(gSwrbDialogSelectFlag){
        case SWRB_DIALOG_SELECT_NONE:
            break;
        case SWRB_DIALOG_SELECT_PCB:
            SweepRobot_PCBTestExitBtnProc();
            break;
        case SWRB_DIALOG_SELECT_MANUL:

            break;
        case SWRB_DIALOG_SELECT_SLAM:
            break;
    }
}
#endif

void SWRB_TestCtrlTask(void *pdata)
{
    u8 i;
    OS_CPU_SR cpu_sr;

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SweepRobotTest_PCBTestInitProc();

    OS_ENTER_CRITICAL();

    OSTaskCreate(SweepRobot_WheelTestTask,(void*)0,(OS_STK*)&SWRB_WHEEL_TEST_TASK_STK[SWRB_WHEEL_TEST_STK_SIZE-1],SWRB_WHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_BrushTestTask,(void*)0,(OS_STK*)&SWRB_BRUSH_TEST_TASK_STK[SWRB_BRUSH_TEST_STK_SIZE-1],SWRB_BRUSH_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_FanTestTask,(void*)0,(OS_STK*)&SWRB_FAN_TEST_TASK_STK[SWRB_FAN_TEST_STK_SIZE-1],SWRB_FAN_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IFRDTestTask,(void*)0,(OS_STK*)&SWRB_IFRD_TEST_TASK_STK[SWRB_IFRD_TEST_STK_SIZE-1],SWRB_IFRD_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_CollisionTestTask,(void*)0,(OS_STK*)&SWRB_COLLISION_TEST_TASK_STK[SWRB_COLLISION_TEST_STK_SIZE-1],SWRB_COLLISION_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_WheelFloatTestTask,(void*)0,(OS_STK*)&SWRB_WHEEL_FLOAT_TEST_TASK_STK[SWRB_WHEEL_FLOAT_TEST_STK_SIZE-1],SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_AshTrayTestTask,(void*)0,(OS_STK*)&SWRB_ASH_TRAY_TEST_TASK_STK[SWRB_ASH_TRAY_TEST_STK_SIZE-1],SWRB_ASH_TRAY_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_UniWheel_Test_Task,(void*)0,(OS_STK*)&SWRB_UNIWHEEL_TEST_TASK_STK[SWRB_UNIWHEEL_TEST_STK_SIZE-1],SWRB_UNIWHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IrDATestTask,(void*)0,(OS_STK*)&SWRB_IRDA_TEST_TASK_STK[SWRB_IRDA_TEST_STK_SIZE-1],SWRB_IRDA_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_ChargeTestTask,(void*)0,(OS_STK*)&SWRB_CHARGE_TEST_TASK_STK[SWRB_CHARGE_TEST_STK_SIZE-1],SWRB_CHARGE_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_BuzzerTestTask,(void*)0,(OS_STK*)&SWRB_BUZZER_TEST_TASK_STK[SWRB_BUZZER_TEST_STK_SIZE-1],SWRB_BUZZER_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_RGBLEDTestTask,(void*)0,(OS_STK*)&SWRB_RGB_LED_TEST_TASK_STK[SWRB_RGB_LED_TEST_STK_SIZE-1],SWRB_RGB_LED_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_KeyTestTask,(void*)0,(OS_STK*)&SWRB_KEY_TEST_TASK_STK[SWRB_KEY_TEST_STK_SIZE-1],SWRB_KEY_TEST_TASK_PRIO);

    for(i=SWRB_WHEEL_TEST_TASK_PRIO;i<SWRB_TEST_TASK_PRIO_END_BOUND;i++){
        OSTaskSuspend(i);
    }

    OSTaskCreate(SweepRobot_ManulTestTask,(void*)0,(OS_STK*)&SWRB_MANUL_TASK_STK[SWRB_MANUL_TEST_STK_SIZE-1],SWRB_MANUL_TEST_TASK_PRIO);
    OSTaskSuspend(SWRB_MANUL_TEST_TASK_PRIO);

    OSTaskCreate(SweepRobot_FrontIFRDTestTask,(void*)0,(OS_STK*)&SWRB_FRONT_IFRD_TASK_STK[SWRB_FRONT_IFRD_TASK_STK_SIZE-1],SWRB_FRONT_IFRD_TEST_TASK_PRIO);
    OSTaskSuspend(SWRB_FRONT_IFRD_TEST_TASK_PRIO);

    OSTaskCreate(Touch_Task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
#ifdef USE_KEY_BUTTON
    OSTaskCreate(Key_Task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
#endif

    OS_EXIT_CRITICAL();

#ifndef USE_KEY_BUTTON
    OSTaskDel(SWRB_TEST_CTRL_TASK_PRIO);
#else
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
#endif
}

static void SWRB_PCBTestIndicateButtonToggle()
{
    SWRB_IndicateButtonToggle(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE);
}

void SweepRobot_PCBTestStartBtnProc(void)
{
    char *str;
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE || gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        STD_UART_ENABLE();

        printf("T->ON\r\n");

        if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  ">TEST RESUMED\r\n");
        }else{
            MultiEdit_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "");

            SweepRobot_PCBTestCheckboxTextReset();

            SWRB_TestCurSNDisp(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_SN);

            SWRB_ValidTestTaskCntGet();

            if( (SWRB_TEST_TASK_PRIO_END_BOUND - (SWRB_TEST_TASK_PRIO_START_BOUND+1)) == gSwrbTestValidTaskCntTotal){
                /* Write Serial Number into the flash of DUT When all test tasks were selected */
                SWRB_TestDUTWriteSN();
            }

            if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
                SWRB_TestDataFileOpen(FA_WRITE|FA_READ);
                SWRB_TestDataFileWriteSN();
                SWRB_TestDataFileWriteDate(">PCB Test Start Time", &rtcDate, &rtcTime);
            }

            str = mymalloc(SRAMIN, sizeof(char)*50);
            *str = 0;
            sprintf(str, "\r\nPCB Test Start Time:20%d/%02d/%02d %02d:%02d:%02d\r\n\r\n",\
                          rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
            myfree(SRAMIN, str);
            Progbar_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_PROGBAR_MAIN, 0);
        }

        SWRB_PCBTestCheckboxDisable();

        TEST_LED_TASK_CB_REG(SWRB_PCBTestIndicateButtonToggle);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTRED);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "PAUSE");
        BUTTON_DispPauseCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
//        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);

        OS_ENTER_CRITICAL();
        if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
            gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);
        }
        gSwrbTestMode = SWRB_TEST_MODE_RUN;
        OSTaskResume(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        /* FIXME:  Valid Task count would be wrong if click checkbox when pause */
//        SWRB_PCBTestCheckboxEnable();

        TEST_LED_TASK_CB_DEREG();
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_GREEN);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_USER_204153051);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "RESUME");
        BUTTON_DispResumeCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        printf("CRG->OFF\r\n");
        GUI_Delay(1);
        printf("LW->SPD=0\r\n");
        GUI_Delay(1);
        printf("RW->SPD=0\r\n");
        GUI_Delay(1);
        printf("LB->SPD=0\r\n");
        GUI_Delay(1);
        printf("RB->SPD=0\r\n");
        GUI_Delay(1);
        printf("MB->SPD=0\r\n");
        GUI_Delay(1);
        printf("FAN->SPD=0\r\n");
        GUI_Delay(1);
        printf("SNSR->IFRD=0\r\n");
        GUI_Delay(1);
        printf("IRDA->OFF\r\n");
        GUI_Delay(1);
        printf("SNSR->BSWC=0\r\n");
        GUI_Delay(1);

        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PRESS RESUME TO RESUME TEST\r\n");
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "TEST PAUSED\r\n");
    }
}

/* TODO: Add PCB Test Dialog Set button implemention here */
//void SweepRobot_PCBTestLoginProc(void)
//{

//}

/* TODO: Add NumPad implemention here*/
#ifdef USE_NUMPAD
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
#endif

void SweepRobot_PCBTestStopBtnProc(void)
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

        switch(gSwrbTestRuningTaskPrio){
            case SWRB_KEY_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_KEY, 0);
                break;
            case SWRB_BUZZER_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_BUZZER, 0);
                break;
            case SWRB_RGB_LED_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_RGB_LED, 0);
                break;
            default:break;
        }

        SWRB_PCBTestCheckboxEnable();

        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_LIGHTGRAY);
        Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTBLUE);
//        Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "START");
        BUTTON_DispStartCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);
        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
//        SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
        SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);

        str = mymalloc(SRAMIN, sizeof(char)*50);
        sprintf(str, "\r\nTest Stop Time:20%02d/%02d/%02d %02d:%02d:%02d\r\n",\
        rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date, rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
        myfree(SRAMIN, str);

        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, "PRESS START TO START TEST\r\n");

        for(i=ID_PCBTEST_CHECKBOX_WHEEL;i<ID_PCBTEST_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }

        if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
            f_close(file);
        }

        OS_ENTER_CRITICAL();
        TEST_LED_TASK_CB_DEREG();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)NULL;
        OS_EXIT_CRITICAL();

        SweepRobotTest_PCBTestInitProc();

        STD_UART_DISABLE();
    }
}

void SweepRobot_PCBTestExitBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;

        SWRB_PCBTestCheckboxEnable();

        if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
            f_close(file);
        }

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)NULL;
        OS_EXIT_CRITICAL();

        STD_UART_ENABLE();

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

        STD_UART_DISABLE();
    }
}

static void SweepRobot_TestHostCtrlStateReset(void)
{
    SweepRobot_Charge24VOff();
    SweepRobot_KeyTestElectroMagnetCtrlReleasePos();
    SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
    SweepRobot_WheelFloatCtrlMoveToDownPos();
    SweepRobotTest_StepMotorSetIdle();
}

static void SweepRobot_TestDUTStateReset(void)
{
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
}

static void SweepRobot_PCBTestCheckboxTextReset(void)
{
    int i;

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
    for(i=ID_PCBTEST_CHECKBOX_WHEEL;i<ID_PCBTEST_CHECKBOX_BOUND;i++){
        Checkbox_Set_Text_Color(i, GUI_BLACK);
    }
}

static void SweepRobot_PCBTestGUIReset(void)
{
    int i;

    for(i=ID_PCBTEST_EDIT_U1;i<=ID_PCBTEST_EDIT_D8;i++){
        Edit_Set_Value(hWin_SWRB_PCBTEST, i, 0);
    }

    Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_INDICATE, GUI_LIGHTGRAY);
    Button_Set_BkColor(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, GUI_LIGHTBLUE);
//    Button_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, "START");
    BUTTON_DispStartCHNStr(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START, 18, 43);

    Progbar_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_PROGBAR_MAIN, 0);

    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);
    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_EXIT);
//    SWRB_WM_EnableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_SET);
}

void SweepRobotTest_PCBTestInitProc(void)
{
    SweepRobot_TestHostCtrlStateReset();
    SweepRobot_TestDUTStateReset();
    SweepRobot_PCBTestGUIReset();
    SweepRobot_PCBTestCheckboxTextReset();

    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)NULL;
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

void SWRB_TestInitCommonAct(u8 taskPrio)
{
    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                     gSwrbManulTestListviewDispItemCoord[taskPrio-(SWRB_TEST_TASK_PRIO_START_BOUND+1)][0], \
                                                     gSwrbManulTestListviewDispItemCoord[taskPrio-(SWRB_TEST_TASK_PRIO_START_BOUND+1)][1], \
                                                     GUI_LIGHTRED);
    }
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

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                   gSwrbManulTestListviewDispItemCoord[taskPrio-(SWRB_TEST_TASK_PRIO_START_BOUND+1)][0], \
                                                   gSwrbManulTestListviewDispItemCoord[taskPrio-(SWRB_TEST_TASK_PRIO_START_BOUND+1)][1], \
                                                   GUI_LIGHTBLUE);
    }

    if(gSwrbTestValidTaskCnt){
        OS_ENTER_CRITICAL();
        OSTaskResume(taskPrio+1);
        OSTaskSuspend(taskPrio);
        OS_EXIT_CRITICAL();
    }else{
        SWRB_TestFinishProc();

        OS_ENTER_CRITICAL();
        OSTaskSuspend(taskPrio);
#ifdef _USE_SELF_TESTING
        OSTaskResume(SELF_TEST_TASK_PRIO);
#endif
        OS_EXIT_CRITICAL();
    }
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
    OSTaskSuspend(OS_PRIO_SELF);
    OS_EXIT_CRITICAL();
}

static void SWRB_PCBTestFinishProc(void)
{
    char *str;

    SweepRobot_TestHostCtrlStateReset();
    SweepRobot_TestDUTStateReset();

    RTC_GetDate(RTC_Format_BIN, &rtcDate);
    RTC_GetTime(RTC_Format_BIN, &rtcTime);

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        SWRB_TestDataFileWriteDate(">PCB Test finish time", &rtcDate, &rtcTime);

        str = mymalloc(SRAMIN, sizeof(char)*50);
        mymemset(str, 0, sizeof(char)*50);
        str = "\r\n****************************************\r\n";
        SWRB_TestDataFileWriteString(str);

        /* Encrypt Test Data File when set enable */
        SWRB_TestDataFileCryptoProc(DISABLE);
    }

    *str = 0;
    sprintf(str, "\r\nTest Finish Time:20%02d/%02d/%02d %02d:%02d:%02d\r\n",\
                    rtcDate.RTC_Year, rtcDate.RTC_Month, rtcDate.RTC_Date,\
                    rtcTime.RTC_Hours, rtcTime.RTC_Minutes, rtcTime.RTC_Seconds);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);

    myfree(SRAMIN, str);

    SWRB_PCBTestCheckboxEnable();

    SweepRobot_PCBTestGUIReset();

    SWRB_ValidTestTaskCntGet();

#ifndef _USE_SN_INC
    SWRB_TestCurSNInc();
#else
    if(gSwrbTestValidTaskCnt == SWRB_TEST_TASK_PRIO_END_BOUND - (SWRB_TEST_TASK_PRIO_START_BOUND+1)){
        SWRB_TestCurSNInc();
    }
#endif

    gSwrbTestValidTaskCnt = 0;
}

static void SWRB_ManulTestFinishProc(void)
{
    char *str;

    SweepRobot_ManulTestCtrlReset();
    
    RTC_GetDate(RTC_Format_BIN, &rtcDate);
    RTC_GetTime(RTC_Format_BIN, &rtcTime);

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        SWRB_TestDataFileWriteDate("Manul Test finish time", &rtcDate, &rtcTime);

        str = mymalloc(SRAMIN, sizeof(char)*50);
        mymemset(str, 0, sizeof(char)*50);
        str = "\r\n****************************************\r\n";
        SWRB_TestDataFileWriteString(str);
        myfree(SRAMIN, str);

        /* Encrypt Test Data File when set enable */
        SWRB_TestDataFileCryptoProc(DISABLE);
    }

    BUTTON_DispStartCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, 18, 43);
    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, GUI_LIGHTBLUE);

    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_INDICATE, GUI_LIGHTGRAY);

    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START);
    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET);
    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);
}

static void SWRB_TestFinishProc(void)
{
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SWRB_PCBTestFinishProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SWRB_ManulTestFinishProc();
    }

    TEST_LED_TASK_CB_DEREG();

    /* Exit Test Mode When Test Finished */
    printf("T->OFF\r\n");

    STD_UART_DISABLE();

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        f_close(file);
    }

    gSwrbTestValidTaskCnt = 0;
    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)NULL;
}

void SweepRobot_StartDlgPCBBtnClickProc(void)
{
#ifdef  _USE_USB_EN_CMP
    if(gSwrbTestUDiskInsertState || gSwrbTestUDiskInsertCmpSkipFlag){
#endif

    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_PCB;

    SWRB_TestCurSNDisp(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_SN);

    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);

//    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_START);
    SWRB_WM_DisableWindow(hWin_SWRB_PCBTEST, ID_PCBTEST_BUTTON_STOP);

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_PCBTEST);

#ifdef  _USE_USB_EN_CMP
    }else{
        hWin_SWRB_START_WARNING = CreateEJE_SWRB_TEST_StartWarningDLG();
    }
#endif
}

void SweepRobot_StartDlgManulBtnClickProc(void)
{
#ifdef  _USE_USB_EN_CMP
    if(gSwrbTestUDiskInsertState || gSwrbTestUDiskInsertCmpSkipFlag){
#endif

    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_MANUL;

    gSwrbTestRuningTaskPrio = SWRB_MANUL_TEST_TASK_PRIO;

    if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_AUTO){
        SweepRobot_ManulTestAutoModeValidTaskStateDisp();
    }else{
        SweepRobot_ManulTestManulModeTaskStateReset();
    }

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_MANUL);

#ifdef  _USE_USB_EN_CMP
    }else{
        hWin_SWRB_START_WARNING = CreateEJE_SWRB_TEST_StartWarningDLG();
    }
#endif
}

void SweepRobot_StartDlgSetBtnClickProc(void)
{
    WM_ShowWin(hWin_SWRB_LOGIN);
    WM_BringToTop(hWin_SWRB_LOGIN);
}

#ifdef USE_SLAM
void SweepRobot_StartDlgSLAMBtnClickProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_SLAM;

    gSwrbTestRuningTaskPrio = SWRB_SLAM_MONITOR_TASK_PRIO;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_SLAM);
}
#endif

#ifdef USE_STEPMOTOR_CTRL
void SweepRobot_StartDlgStepMotorBtnClickProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_STEP_MOTOR;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_STEPMOTOR);
}
#endif

#ifdef USE_DECRYPTO
void SweepRobot_StartDlgDecryptoBtnClickProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_DECRYPTO;

    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_DECRYPTO);
}
#endif

static void SweepRobot_ManulTestCtrlReset(void)
{
    SweepRobot_Charge24VOff();
    SweepRobot_KeyTestElectroMagnetCtrlReleasePos();
    SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
    SweepRobot_WheelFloatCtrlMoveToDownPos();
}

static void SWRB_ManulTestIndicateButtonToggle()
{
    SWRB_IndicateButtonToggle(hWin_SWRB_MANUL, ID_MANUL_BUTTON_INDICATE);
}

static void SweepRobot_ManulStartBtnManulModeStartProc(void)
{
    OS_CPU_SR cpu_sr;

    printf("T->ON\r\n");
    GUI_Delay(1);
    printf("SNSR->IFRD=1\r\n");
    GUI_Delay(1);
    printf("IRDA->ON\r\n");
    GUI_Delay(1);

    SweepRobot_ManulTestSNDisp();

    OS_ENTER_CRITICAL();
    gSwrbTestRuningTaskPrio = SWRB_MANUL_TEST_TASK_PRIO;
    OSTaskResume(gSwrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
}

static void SweepRobot_ManulStartBtnAutoModeStartProc(void)
{
    OS_CPU_SR cpu_sr;

    printf("T->ON\r\n");

    SWRB_ValidTestTaskCntGet();

    SweepRobot_ManulTestDataReset();
    SweepRobot_ManulTestGuiReset();
    SweepRobot_ManulTestSNDisp();
    SweepRobot_ManulTestDataQuery();
    SweepRobot_ManulTestBatteryVoltDisp();

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        SWRB_TestDataFileOpen(FA_WRITE|FA_READ);
        SWRB_TestDataFileWriteDate("Manul Test Start Time", &rtcDate, &rtcTime);
    }

    OS_ENTER_CRITICAL();
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_TASK_PRIO_START_BOUND+1);
    OSTaskResume(gSwrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
}

static void SweepRobot_ManulStartBtnStartProc(void)
{
    int i;
    OS_CPU_SR cpu_sr;

    gSwrbTestMode = SWRB_TEST_MODE_RUN;

    for(i=0;i<USART_RX_LEN;i++)
        USART_RX_BUF[i] = 0;

//    Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, "Stop");
    BUTTON_DispStopCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, 18, 43);
    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, GUI_LIGHTRED);

    OS_ENTER_CRITICAL();
    TEST_LED_TASK_CB_REG(SWRB_ManulTestIndicateButtonToggle);
    OS_EXIT_CRITICAL();

    SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET);
    SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
    SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);

    STD_UART_ENABLE();

    if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_MANUL){
        SweepRobot_ManulStartBtnManulModeStartProc();
    }else if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_AUTO){
        SweepRobot_ManulStartBtnAutoModeStartProc();
    }
}

static void SweepRobot_ManulStartBtnStopProc(void)
{
    OS_CPU_SR cpu_sr;

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;
    gSwrbTestTaskRunCnt = 0;

    if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_MANUL){
        printf("SNSR->IFRD=0\r\n");
        GUI_Delay(1);
        printf("IRDA->OFF\r\n");
        GUI_Delay(1);
        printf("IRDA->ERS\r\n");
    }else if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_AUTO){
        SweepRobot_FrontIFRDTestStateReset();
        SweepRobot_TestHostCtrlStateReset();
        SweepRobot_TestDUTStateReset();

        switch(gSwrbTestRuningTaskPrio){
            case SWRB_KEY_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_KEY, 0);
                break;
            case SWRB_BUZZER_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_BUZZER, 0);
                break;
            case SWRB_RGB_LED_TEST_TASK_PRIO:
                GUI_EndDialog(hWin_SWRB_RGB_LED, 0);
                break;
            default:break;
        }
    }

    BUTTON_DispStartCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, 18, 43);
    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_START, GUI_LIGHTBLUE);

    TEST_LED_TASK_CB_DEREG();
    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_INDICATE, GUI_LIGHTGRAY);

    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET);
    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
    SWRB_WM_EnableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_EXIT);

    STD_UART_DISABLE();

    if(gSwrbTestSDCardInsertState || gSwrbTestUDiskInsertState){
        f_close(file);
    }

    OS_ENTER_CRITICAL();
    if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OSTaskSuspend(SWRB_MANUL_TEST_TASK_PRIO);
    }else if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_AUTO){
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
    }
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)NULL;
    OS_EXIT_CRITICAL();
}

void SweepRobot_ManulStartBtnProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE || gSwrbTestMode == SWRB_TEST_MODE_PAUSE){
        SweepRobot_ManulStartBtnStartProc();
    }else{
        SweepRobot_ManulStartBtnStopProc();
    }
}

static void SweepRobot_ManulTestAutoModeValidTaskStateDisp(void)
{
    int i;

    for(i=0;i<SWRB_TEST_STATE_BOUND;i++){
        if(Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL+i)){
            Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                         gSwrbManulTestListviewDispItemCoord[i][0],\
                                                         gSwrbManulTestListviewDispItemCoord[i][1],\
                                                         GUI_BLUE);
        }else{
            Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                         gSwrbManulTestListviewDispItemCoord[i][0],\
                                                         gSwrbManulTestListviewDispItemCoord[i][1],\
                                                         GUI_LIGHTGRAY);
        }
    }

    Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 SWRB_MANUL_TEST_LISTVIEW_COLUMN_ITEM,\
                                                 SWRB_MANUL_TEST_LISTVIEW_ROW_SNUM,\
                                                 GUI_BLUE);

    if(Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD)){
        Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][0],\
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][1],\
                                                 GUI_BLUE);
    }else{
        Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][0],\
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][1],\
                                                 GUI_LIGHTGRAY);
    }

    Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 gSwrbManulTestListviewDispItemPowerStationCoord[0][0],\
                                                 gSwrbManulTestListviewDispItemPowerStationCoord[0][1],\
                                                 GUI_LIGHTGRAY);

    Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 SWRB_MANUL_TEST_LISTVIEW_COLUMN_ITEM,\
                                                 SWRB_MANUL_TEST_LISTVIEW_ROW_INT_VREF,\
                                                 GUI_LIGHTGRAY);
}

static void SweepRobot_ManulTestManulModeTaskStateReset(void)
{
    int i;

    for(i=0;i<SWRB_TEST_STATE_BOUND;i++){
        Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                     gSwrbManulTestListviewDispItemCoord[i][0],\
                                                     gSwrbManulTestListviewDispItemCoord[i][1],\
                                                     GUI_BLACK);
    }

    Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][0],\
                                                 gSwrbManulTestListviewDispItemFrontIFRDCoord[0][1],\
                                                 GUI_BLACK);

    Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                 gSwrbManulTestListviewDispItemPowerStationCoord[0][0],\
                                                 gSwrbManulTestListviewDispItemPowerStationCoord[0][1],\
                                                 GUI_BLACK);

    for(i=0;i<2;i++){
        Listview_Set_Item_TextColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                     gSwrbManulTestListviewDispItemSystemCoord[i][0],\
                                                     gSwrbManulTestListviewDispItemSystemCoord[i][1],\
                                                     GUI_BLACK);
    }
}

void SweepRobot_ManulSetBtnEnterManulModeProc(void)
{
    int Id;

    gSwrbTestManulSubMode = SWRB_TEST_MANUL_SUB_MODE_MANUL;

    SweepRobot_ManulTestManulModeTaskStateReset();

    for(Id=ID_MANUL_BUTTON_WHEEL;Id<ID_MANUL_BUTTON_BOUND;Id++){
        SWRB_WM_EnableWindow(hWin_SWRB_MANUL, Id);
    }

    Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, GUI_LIGHTGREEN);
    BUTTON_DispManulModeCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, 18, 43);
}

void SweepRobot_ManulSetBtnProc(void)
{
    int Id;

    if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_MANUL){

        gSwrbTestManulSubMode = SWRB_TEST_MANUL_SUB_MODE_AUTO;

        SweepRobot_ManulTestAutoModeValidTaskStateDisp();

        for(Id=ID_MANUL_BUTTON_WHEEL;Id<ID_MANUL_BUTTON_BOUND;Id++){
            SWRB_WM_DisableWindow(hWin_SWRB_MANUL, Id);
        }

        Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, GUI_LIGHTCYAN);
        BUTTON_DispAutoModeCHNStr(hWin_SWRB_MANUL, ID_MANUL_BUTTON_SET, 18, 43);
    }else if(gSwrbTestManulSubMode == SWRB_TEST_MANUL_SUB_MODE_AUTO){

        WM_HideWin(hWin_SWRB_MANUL);
        WM_ShowWin(hWin_SWRB_LOGIN);
        WM_BringToTop(hWin_SWRB_LOGIN);
    }
}

void SweepRobot_ManulResetBtnProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        SweepRobot_ManulTestDataReset();
        SweepRobot_ManulTestGuiReset();

        SWRB_WM_DisableWindow(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RESET);
    }
}

void SweepRobot_ManulExitBtnProc(void)
{
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;

        STD_UART_ENABLE();

        printf("T->OFF\r\n");
        WM_HideWin(hWin_SWRB_MANUL);
        WM_ShowWin(hWin_SWRB_START);

        STD_UART_DISABLE();

        SweepRobotTest_LoginEditClear();
    }
}

void SweepRobot_ManulWheelBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        if(aSwrbManulTestState[SWRB_TEST_STATE_WHEEL]){
            printf("T->ON\r\n");
            printf("WHL->DIR=1\r\n");
            printf("LW->SPD=40\r\n");
            printf("RW->SPD=40\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 0;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_WHEEL, GUI_LIGHTBLUE);
        }else{
            printf("LW->SPD=0\r\n");
            printf("RW->SPD=0\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_WHEEL] = 1;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_WHEEL, GUI_GRAY);
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulBrushBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        if(aSwrbManulTestState[SWRB_TEST_STATE_WHEEL]){
            printf("T->ON\r\n");
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
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BRUSH, GUI_GRAY);
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulFanBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        if(aSwrbManulTestState[SWRB_TEST_STATE_FAN]){
            printf("T->ON\r\n");
            printf("FAN->SPD=25\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_FAN] = 0;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_FAN, GUI_LIGHTBLUE);
        }else{
            printf("FAN->SPD=0\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_FAN] = 1;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_FAN, GUI_GRAY);
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulIFRDBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        if(aSwrbManulTestState[SWRB_TEST_STATE_IFRD]){
            printf("SNSR->IFRD=1\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_IFRD] = 0;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_IFRD, GUI_LIGHTBLUE);
        }else{
            printf("SNSR->IFRD=0\r\n");
            aSwrbManulTestState[SWRB_TEST_STATE_IFRD] = 1;
            Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_IFRD, GUI_GRAY);
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulBuzzerBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        aSwrbManulTestState[SWRB_TEST_STATE_BUZZER]++;
        switch(aSwrbManulTestState[SWRB_TEST_STATE_BUZZER]){
            case 1:
                printf("T->ON\r\n");
                printf("BZR->ON=1\r\n");
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_LIGHTRED);
                break;
            case 2:
                printf("BZR->ON=2\r\n");
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_LIGHTGREEN);
                break;
            case 3:
                printf("BZR->ON=3\r\n");
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_LIGHTBLUE);
                break;
            case 4:
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_BUZZER, GUI_GRAY);
                aSwrbManulTestState[SWRB_TEST_STATE_BUZZER] = 0;
                break;
            default:break;
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulRGBLEDBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED]++;
        switch(aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED]){
            case 1:
                printf("T->ON\r\n");
                printf("RGB->ON=%d\r\n", RGB_LED_RED);
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_LIGHTRED);
                break;
            case 2:
                printf("RGB->ON=%d\r\n", RGB_LED_GREEN);
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_LIGHTGREEN);
                break;
            case 3:
                printf("RGB->ON=%d\r\n", RGB_LED_BLUE);
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_LIGHTBLUE);
                break;
            case 4:
                printf("RGB->ON=%d\r\n", RGB_LED_RGB);
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_WHITE);
                break;
            case 5:
                printf("RGB->ON=%d\r\n", RGB_LED_BLUE);
                Button_Set_BkColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RGB_LED, GUI_GRAY);
                aSwrbManulTestState[SWRB_TEST_STATE_RGB_LED] = 0;
                break;
            default:break;
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulRELAYBtnProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode==SWRB_TEST_MODE_RUN && gSwrbTestManulSubMode==SWRB_TEST_MANUL_SUB_MODE_MANUL){
        OS_ENTER_CRITICAL();
        aSwrbManulTestState[SWRB_TEST_STATE_COLLISION]++;
        switch(aSwrbManulTestState[SWRB_TEST_STATE_COLLISION]){
            case 1:
                SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
                SweepRobot_CollisionRelayCtrlOn(COLLISION_CHAN_L);
                Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, "L ON");
                Button_Set_TextColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, GUI_LIGHTRED);
                break;
            case 2:
                SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
                SweepRobot_CollisionRelayCtrlOn(COLLISION_CHAN_FL);
                Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, "fL ON");
                Button_Set_TextColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, GUI_LIGHTYELLOW);
                break;
            case 3:
                SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
                SweepRobot_CollisionRelayCtrlOn(COLLISION_CHAN_R);
                Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, "R ON");
                Button_Set_TextColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, GUI_LIGHTBLUE);
                break;
            case 4:
                SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
                SweepRobot_CollisionRelayCtrlOn(COLLISION_CHAN_FR);
                Button_Set_Text(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, "FR ON");
                Button_Set_TextColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, GUI_LIGHTMAGENTA);
                break;
            case 5:
                SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);
                aSwrbManulTestState[SWRB_TEST_STATE_COLLISION] = 0;
                Button_Set_TextColor(hWin_SWRB_MANUL, ID_MANUL_BUTTON_RELAY, GUI_GRAY);
                break;
            default:break;
        }
        OS_EXIT_CRITICAL();
    }
}

void SweepRobot_ManulTest_CtrlBtnStateArrayReset(void)
{
    u8 i;

    for(i=SWRB_TEST_STATE_WHEEL;i<SWRB_TEST_STATE_BOUND;i++){
        aSwrbManulTestState[i] = 0;
    }
}
