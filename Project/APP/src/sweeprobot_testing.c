#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"

#include "usart.h"
#include "stm32f4xx_it.h"
#include <stdio.h>
#include <string.h>

#define SWRB_TEST_ACQUIRED_DATA_LEN_MAX  50

u8 usartRxFlag = 0;
int usartRxNum = 0;

enum SWRB_TEST_MODE gSwrbTestMode = SWRB_TEST_MODE_IDLE;
enum SWRB_TEST_SET_STATE gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;
enum SWRB_TEST_TASK_PRIO gSwrbTestRuningTaskPrio;
u32 gSwrbTestStateMap = 0;
u32 lastSwrbTestStateMap = 0;
u16 gSwrbTestTaskRunCnt = 0;
int gSwrbTestTaskCnt;
char *gSwrbTestDataFilePath;
int gSwrbTestAcquiredData[SWRB_TEST_ACQUIRED_DATA_LEN_MAX] = {0};

static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;

static RTC_TimeTypeDef rtcTime;
static RTC_DateTypeDef rtcDate;

static WM_MESSAGE *pWmMsg;

static void Start_Task(void *pdata);
static void emWin_Maintask(void *pdata);
static void Touch_Task(void *pdata);
static void Led_Task(void *pdata);
static void Key_Task(void *pdata);
static void Rtc_Task(void *pdata);
static void Usart_Task(void *pdata);
static void SweepRobot_TestSaveDataTask(void *pdata);
static void SWRB_TestCtrlTask(void *pdata);
static void SWRB_ExceptionCheckTask(void *pdata);

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK USART_TASK_STK[USART_STK_SIZE];
OS_STK RTC_TASK_STK[RTC_STK_SIZE];
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
OS_STK LED_TASK_STK[LED_STK_SIZE];
OS_STK SAVE_DATA_TASK_STK[SAVE_DATA_STK_SIZE];
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

static void SweepRobot_TestInitProc(void);

void OS_Task_Create(void)
{
    OSTaskCreate(Start_Task,
                (void*)0,
                (OS_STK*)&START_TASK_STK[START_STK_SIZE-1],
                START_TASK_PRIO);
}

void Start_Task(void *pdata)
{
    u8 i;

    OS_CPU_SR cpu_sr;

    OSStatInit();

    OS_ENTER_CRITICAL();

    OSTaskCreate(emWin_Maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWIN_TASK_PRIO);
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
    OSTaskCreate(SweepRobot_Buzzer_Test_Task,(void*)0,(OS_STK*)&SWRB_BUZZER_TEST_TASK_STK[SWRB_BUZZER_TEST_STK_SIZE-1],SWRB_BUZZER_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_RGB_LED_Test_Task,(void*)0,(OS_STK*)&SWRB_RGB_LED_TEST_TASK_STK[SWRB_RGB_LED_TEST_STK_SIZE-1],SWRB_RGB_LED_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_ChargeTestTask,(void*)0,(OS_STK*)&SWRB_CHARGE_TEST_TASK_STK[SWRB_CHARGE_TEST_STK_SIZE-1],SWRB_CHARGE_TEST_TASK_PRIO);

    for(i=SWRB_WHEEL_TEST_TASK_PRIO;i<SWRB_TEST_TASK_PRIO_BOUND;i++){
        OSTaskSuspend(i);
    }
    OSTaskSuspend(SAVE_DATA_TASK_PRIO);

    OS_EXIT_CRITICAL();
    OSTaskDel(OS_PRIO_SELF);
}

static void SweepRobot_TestCkbStateSet(void)
{
    u16 i;

    for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++)
        Checkbox_Set_State(i, 1);
}

static void SweepRobot_TestCkbStateDisable(void)
{
    u16 i;

    for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++){
        Checkbox_Set_State(i, 2);
    }
}

void emWin_Maintask(void *pdata)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);

    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_MULTIBUF_Enable(1);
    GUI_Init();

    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DrawBitmap( &bmeje_logo, 320, 214);
    OSTimeDlyHMSM(0,0,1,500);

    hWin_SWRB_MAIN = CreateEJE_SweepRobot_test_System();
    hWin_SWRB_SNSETTING = CreateSNSettingDLG();
    hWin_SWRB_TIMESETTING = CreateTimeSettingDLG();

    WIDGET_SetDefaultEffect(&WIDGET_Effect_None);

    OSTaskCreate(Touch_Task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
    OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(Key_Task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
    OSTaskCreate(Rtc_Task,(void*)0,(OS_STK*)&RTC_TASK_STK[RTC_STK_SIZE-1],RTC_TASK_PRIO);
    OSTaskCreate(Usart_Task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);
    OSTaskCreate(SWRB_TestCtrlTask,(void*)0,(OS_STK*)&SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE-1],SWRB_TEST_CTRL_TASK_PRIO);
    OSTaskCreate(SWRB_ExceptionCheckTask, (void*)0,(OS_STK*)&SWRB_TEST_EXCEPTION_CHECK_TASK_STK[SWRB_TEST_EXCEPTION_CHECK_STK_SIZE-1],SWRB_TEST_EXCEPTION_CHECK_TASK_PRIO);

    MultiEdit_Set_Text("PRESS KEY0 TO START\r\n");
    MultiEdit_Add_Text("READY FOR TESTING\r\n");

    gSwrbTestTaskCnt = 0;
    SweepRobot_TestCkbStateSet();

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
	while(1)
	{
		LED0 = !LED0;

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
//    RTC_TimeTypeDef rtcTmpTime;
//    RTC_DateTypeDef rtcTmpDate;
//
//    rtcTmpTime.RTC_Hours = 9;
//    rtcTmpTime.RTC_Minutes = 28;
//    rtcTmpTime.RTC_Seconds = 40;
//
//    rtcTmpDate.RTC_Year = 15;
//    rtcTmpDate.RTC_Month = 11;
//    rtcTmpDate.RTC_Date = 25;
//
//    RTC_SetTime(RTC_Format_BIN, &rtcTmpTime);
//    RTC_SetDate(RTC_Format_BIN, &rtcTmpDate);

    while(1){

        RTC_GetDate(RTC_Format_BIN, &rtcDate);
        RTC_GetTime(RTC_Format_BIN, &rtcTime);

        RTC_TIME_Disp(&rtcDate, &rtcTime);

        OSTimeDlyHMSM(0,0,1,0);
    }
}

void Usart_Task(void *pdata)
{
//    static char *usartRxStr;

    while(1){
        if(USART_RX_STA & (0x8000) ){
    //      printf("%s\r\n",USART_RX_BUF);
    //      USART_RxArrayToString(USART_RX_BUF, &usartRxStr);
            USART_RxArrayToNumber(USART_RX_BUF, &usartRxNum);
            usartRxFlag = 1;
    //      MultiEdit_Add_Text(usartRxStr);
    //      printf("%d\r\n",usartRxNum);
            USART_RX_STA = 0;
            /* Resume usart data query task immediately */
            OSTimeDlyResume(gSwrbTestRuningTaskPrio);
        }
        OSTimeDlyHMSM(0,0,0,5);
    }
}

void SWRB_TestDataFileWriteString(char *str)
{
    SWRB_TestDataFileOpen();
    f_puts(str,file);
    f_close(file);
}

void SWRB_TestDataFileWriteData(char *headstr, int data, u8 CRflag)
{
    char *datastr;

    SWRB_TestDataFileOpen();

    f_puts(headstr, file);
    datastr = mymalloc(SRAMIN, sizeof(char)*10);
    mymemset(datastr, 0, sizeof(char)*10);
    if(CRflag){
        sprintf(datastr, "%d\r\n", data);
    }else{
        sprintf(datastr, "%d", data);
    }
    f_puts(datastr,file);
    myfree(SRAMIN, datastr);

    f_close(file);
}

void SWRB_TestDataFileWriteDate(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    SWRB_TestDataFileWriteData("Test Time:20",date->RTC_Year, 0);
    SWRB_TestDataFileWriteData("/",date->RTC_Month, 0);
    SWRB_TestDataFileWriteData("/",date->RTC_Date, 0);

    SWRB_TestDataFileWriteData(" ",time->RTC_Hours, 0);
    SWRB_TestDataFileWriteData(":",time->RTC_Minutes, 0);
    SWRB_TestDataFileWriteData(":",time->RTC_Seconds, 1);
}

void SweepRobot_TestSaveDataTask(void *pdata)
{

    while(1){

        SWRB_TestDataFileWriteDate(&rtcDate, &rtcTime);

        if(Checkbox_Get_State(ID_CHECKBOX_WHEEL)){
            Wheel_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_BRUSH)){
            Brush_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_FAN)){
            Fan_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_IFRD)){
            IFRD_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_COLLISION)){
            Collision_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_WHEEL_FLOAT)){
            WHEEL_FLOAT_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_ASH_TRAY)){
            ASH_TRAY_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_UNIWHEEL)){
            UNIWHEEL_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_KEY)){
            KEY_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_IRDA)){
            IRDA_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_BUZZER)){
            BUZZER_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_RGB_LED)){
            RGB_LED_TestDataSave();
        }
        if(Checkbox_Get_State(ID_CHECKBOX_CHARGE)){
            CHARGE_TestDataSave();
        }

        OSTaskSuspend(OS_PRIO_SELF);
    }
}

void SWRB_TestCtrlTask(void *pdata)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    OSTaskCreate(SweepRobot_TestSaveDataTask,(void*)0,(OS_STK*)&SAVE_DATA_TASK_STK[SAVE_DATA_STK_SIZE-1],SAVE_DATA_TASK_PRIO);
    OSTaskSuspend(SAVE_DATA_TASK_PRIO);
    OS_EXIT_CRITICAL();

    SweepRobot_TestInitProc();
    MultiEdit_Add_Text("PRESS START TO START TEST\r\n");

    gSwrbTestDataFilePath = mymalloc(SRAMIN, sizeof(char)*40);
    mymemset(gSwrbTestDataFilePath, 0, sizeof(char)*40);

    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    while(1){
        if(gkeyCodeGetFinishFlag == 1){
            switch(gkeyCode){
                /* TEST START/PAUSE/RESUME PRESSED*/
                case 1:
                    SweepRobot_TestStartProc();
                    break;
                /* TEST SET PRESSED */
                case 2:
                    SweepRobot_TestSetProc();
                    break;
                /* TEST STOP PRESSED */
                case 3:
                    SweepRobot_TestStopProc();
                    break;
                /* TEST EXIT PRESSED */
                case 4:
                    SweepRobot_TestExitProc();
                    break;
                default:
                    gkeyCode = 0;
                    gkeyCodeGetFinishFlag = 0;
                break;
            }
        }
        OSTimeDlyHMSM(0,0,0,50);
    }
}

void SWRB_ExceptionCheckTask(void *pdata)
{
    
}

void SweepRobot_TestStartProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
        SWRB_TestDataFileWriteSN(hWin_SWRB_SNSETTING);
        SWRB_TestDataFileWriteDate(&rtcDate, &rtcTime);
    }

    if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE || gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        if(gSwrbTestMode == SWRB_TEST_MODE_PAUSE){
            MultiEdit_Add_Text("TEST RESUMED\r\n");
        }else{
            MultiEdit_Set_Text("\r\n");
        }

        gSwrbTestMode = SWRB_TEST_MODE_RUN;

        Button_Set_unPressedBkColor(hWin_SWRB_MAIN, ID_BUTTON_START, GUI_LIGHTRED);
        Button_Set_Text(ID_BUTTON_START, "PAUSE");
        printf("TEST->ON\r\n");

        OS_ENTER_CRITICAL();
        OSTaskResume(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        gkeyCode = 0;
        gkeyCodeGetFinishFlag = 0;
    }else if(gSwrbTestMode == SWRB_TEST_MODE_RUN){

        gSwrbTestMode = SWRB_TEST_MODE_PAUSE;

        Button_Set_unPressedBkColor(hWin_SWRB_MAIN, ID_BUTTON_START, GUI_LIGHTBLUE);
        Button_Set_Text(ID_BUTTON_START, "RESUME");
        MultiEdit_Add_Text("PRESS RESUME TO RESUME TEST\r\n");
        
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
        MultiEdit_Add_Text("TEST PAUSED\r\n");
        MultiEdit_Add_Text("PRESS RESUME TO RESUME TEST\r\n");

        gkeyCode = 0;
        gkeyCodeGetFinishFlag = 0;
    }
}

void SweepRobot_TestSetProc(void)
{
    WM_HWIN hItem;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
        gSwrbTestMode = SWRB_TEST_MODE_SET;
        gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;

        hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SET_BUTTON_SNSET);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
        BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);

        SWRB_ListWheelLastItemPosGet(hWin_SWRB_SNSETTING);

        WM_HideWin(hWin_SWRB_MAIN);
        WM_ShowWin(hWin_SWRB_SNSETTING);
    }
}

void SweepRobot_TestStopProc(void)
{
    OS_CPU_SR cpu_sr;
    int i;

    if(gSwrbTestMode == SWRB_TEST_MODE_RUN || gSwrbTestMode == SWRB_TEST_MODE_PAUSE){

        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        Button_Set_unPressedBkColor(hWin_SWRB_MAIN, ID_BUTTON_START, GUI_LIGHTBLUE);
        Button_Set_Text(ID_BUTTON_START, "START");

        MultiEdit_Add_Text("PRESS START TO START TEST\r\n");

        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();

        for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }

        SweepRobot_TestInitProc();
        gkeyCode = 0;
        gkeyCodeGetFinishFlag = 0;
    }
}

void SweepRobot_TestExitProc(void)
{
    OS_CPU_SR cpu_sr;

    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){

        mf_close();
        OS_ENTER_CRITICAL();
        OSTaskSuspend(gSwrbTestRuningTaskPrio);
        OS_EXIT_CRITICAL();
        SweepRobot_TestInitProc();
        printf("TEST->OFF\r\n");

        Button_Set_unPressedBkColor(hWin_SWRB_MAIN, ID_BUTTON_START, GUI_LIGHTBLUE);
        Button_Set_Text(ID_BUTTON_START, "START");

        MultiEdit_Set_Text("ROBOT EXIT TEST MODE\r\n");
        MultiEdit_Add_Text("PRESS START TO ENTER TEST MODE AND START TEST\r\n");

        gkeyCode = 0;
        gkeyCodeGetFinishFlag = 0;
    }
}

void SweepRobot_TestInitProc(void)
{
    int i;

    SweepRobot_Charge24VOff();
    SweepRobot_KeyTestCtrlOff();
    SweepRobot_CollisionCtrlOff();
    SweepRobot_WheelFloatCtrlOff();
    SweepRobot_AshTrayTestInsCtrlOff();

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

    for(i=ID_EDIT_U1;i<=ID_EDIT_D8;i++){
        Edit_Set_Value(i, 0);
    }

    Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL");
    Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH");
    Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN");
    Checkbox_Set_Text(ID_CHECKBOX_IFRD, "IFRD");
    Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION");
    Checkbox_Set_Text(ID_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT");
    Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY");
    Checkbox_Set_Text(ID_CHECKBOX_UNIWHEEL, "UNIWHEEL");
    Checkbox_Set_Text(ID_CHECKBOX_KEY, "KEY");
    Checkbox_Set_Text(ID_CHECKBOX_IRDA, "IRDA");
    Checkbox_Set_Text(ID_CHECKBOX_BUZZER, "BUZZER");
    Checkbox_Set_Text(ID_CHECKBOX_RGB_LED, "RGB LED");
    Checkbox_Set_Text(ID_CHECKBOX_CHARGE, "CHARGE");

    Progbar_Set_Value(0);

    gSwrbTestTaskRunCnt = 0;
    gSwrbTestStateMap = 0;
    gSwrbTestRuningTaskPrio = (enum SWRB_TEST_TASK_PRIO)(SWRB_TEST_START_TASK_BOUND+1);

    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;
}

void SWRB_NextTestTaskResumePreAct(u8 taskPrio)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();

    OSTaskResume(taskPrio+1);
    OSTaskSuspend(OS_PRIO_SELF);

    OS_EXIT_CRITICAL();
}

static void SWRB_ValidTestTaskCntGet(void)
{
    int i;
    WM_HWIN hItem;

    for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
        if(CHECKBOX_GetState(hItem))
            gSwrbTestTaskCnt++;
    }
}

void SWRB_NextTestTaskResumePostAct(u8 taskPrio)
{
    int i;
    char *str;
    OS_CPU_SR cpu_sr;
    WM_HWIN hItem;

    gSwrbTestTaskCnt--;

    OS_ENTER_CRITICAL();

    if(gSwrbTestTaskCnt){
        OSTaskResume(taskPrio+1);
    }else{
        gSwrbTestMode = SWRB_TEST_MODE_IDLE;

        SWRB_ValidTestTaskCntGet();
        SweepRobot_TestInitProc();

        str = "\r\nTEST FINISHED\r\n";
        SWRB_TestDataFileWriteString(str);

        MultiEdit_Add_Text(str);
        Button_Set_unPressedBkColor(hWin_SWRB_MAIN, ID_BUTTON_START, GUI_LIGHTBLUE);
        Button_Set_Text(ID_BUTTON_START, "START");
        for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++){
            Checkbox_Set_Text_Color(i, GUI_BLACK);
        }

        SWRB_ListWheelSNInc(&hWin_SWRB_SNSETTING);
    }

    OSTaskSuspend(OS_PRIO_SELF);

    OS_EXIT_CRITICAL();
}

/*
void SweepRobot_TestTestTask(void *pdata)
{
  OS_CPU_SR cpu_sr;

  plat_int_reg_cb(STM32F4xx_INT_TIM3, TIM3_ISR);

  while(1){
    if(gkeyCodeGetFinishFlag == 1){
      switch(gkeyCode){
        case 1:
          TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
          TIM_Cmd(TIM3,ENABLE);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 2:
          OS_ENTER_CRITICAL();
          OSTaskDel(LED_TASK_PRIO);
          OS_EXIT_CRITICAL();
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 3:
          OS_ENTER_CRITICAL();
          OSTaskResume(TEST_CTRL_TASK_PRIO);
          OSTaskDel(OS_PRIO_SELF);
          OS_EXIT_CRITICAL();
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 4:
          OS_ENTER_CRITICAL();
          OSTaskCreate(Led_Task,(void*)0,(OS_STK*)LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
          OS_EXIT_CRITICAL();
          TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
          TIM_Cmd(TIM3,DISABLE);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        default:
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
      }
    }
    OSTimeDlyHMSM(0,0,0,20);
  }
}
*/
