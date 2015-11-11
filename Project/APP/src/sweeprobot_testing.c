#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"

#include "usart.h"
#include "stm32f4xx_it.h"

#define SWRB_TEST_ACQUIRED_DATA_LEN_MAX  200

u8 usartRxFlag = 0;
int usartRxNum = 0;

u32 swrbTestStateMap = 0;
u32 lastSwrbTestStateMap = 0;

u16 swrbTestTaskCnt = 0;

u8 swrbTestRuningTaskPrio = 0;

u16 swrbTestAcquiredData[SWRB_TEST_ACQUIRED_DATA_LEN_MAX] = {0};

static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;

static void Start_Task(void *pdata);
static void emWin_Maintask(void *pdata);
static void Touch_Task(void *pdata);
static void Led_Task(void *pdata);
static void Key_Task(void *pdata);
static void Usart_Task(void *pdata);
static void SWRB_Test_Ctrl_Task(void *pdata);
//static void SweepRobot_Test_Start(void *pdata);
//static void TIM3_ISR(void);

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK USART_TASK_STK[USART_STK_SIZE];
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
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

OS_EVENT TEST_EVT;

OS_Q KEY_Msg;
OS_Q USART_Msg;

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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);

    OSStatInit();

    OS_ENTER_CRITICAL();

    OSTaskCreate(emWin_Maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWINDEMO_TASK_PRIO);
    OSTaskCreate(Touch_Task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
    OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(Key_Task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
    OSTaskCreate(Usart_Task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);
    OSTaskCreate(SWRB_Test_Ctrl_Task,(void*)0,(OS_STK*)&SWRB_TEST_CTRL_TASK_STK[SWRB_TEST_CTRL_STK_SIZE-1],SWRB_TEST_CTRL_TASK_PRIO);
    OSTaskCreate(SweepRobot_Wheel_Test_Task,(void*)0,(OS_STK*)&SWRB_WHEEL_TEST_TASK_STK[SWRB_WHEEL_TEST_STK_SIZE-1],SWRB_WHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Brush_Test_Task,(void*)0,(OS_STK*)&SWRB_BRUSH_TEST_TASK_STK[SWRB_BRUSH_TEST_STK_SIZE-1],SWRB_BRUSH_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Fan_Test_Task,(void*)0,(OS_STK*)&SWRB_FAN_TEST_TASK_STK[SWRB_FAN_TEST_STK_SIZE-1],SWRB_FAN_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IFRD_Test_Task,(void*)0,(OS_STK*)&SWRB_IFRD_TEST_TASK_STK[SWRB_IFRD_TEST_STK_SIZE-1],SWRB_IFRD_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Collision_Test_Task,(void*)0,(OS_STK*)&SWRB_COLLISION_TEST_TASK_STK[SWRB_COLLISION_TEST_STK_SIZE-1],SWRB_COLLISION_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Wheel_Float_Test_Task,(void*)0,(OS_STK*)&SWRB_WHEEL_FLOAT_TEST_TASK_STK[SWRB_WHEEL_FLOAT_TEST_STK_SIZE-1],SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Ash_Tray_Test_Task,(void*)0,(OS_STK*)&SWRB_ASH_TRAY_TEST_TASK_STK[SWRB_ASH_TRAY_TEST_STK_SIZE-1],SWRB_ASH_TRAY_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_UniWheel_Test_Task,(void*)0,(OS_STK*)&SWRB_UNIWHEEL_TEST_TASK_STK[SWRB_UNIWHEEL_TEST_STK_SIZE-1],SWRB_UNIWHEEL_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Key_Test_Task,(void*)0,(OS_STK*)&SWRB_KEY_TEST_TASK_STK[SWRB_KEY_TEST_STK_SIZE-1],SWRB_KEY_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_IrDA_Test_Task,(void*)0,(OS_STK*)&SWRB_IRDA_TEST_TASK_STK[SWRB_IRDA_TEST_STK_SIZE-1],SWRB_IRDA_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_Buzzer_Test_Task,(void*)0,(OS_STK*)&SWRB_BUZZER_TEST_TASK_STK[SWRB_BUZZER_TEST_STK_SIZE-1],SWRB_BUZZER_TEST_TASK_PRIO);
    OSTaskCreate(SweepRobot_RGB_LED_Test_Task,(void*)0,(OS_STK*)&SWRB_RGB_LED_TEST_TASK_STK[SWRB_RGB_LED_TEST_STK_SIZE-1],SWRB_RGB_LED_TEST_TASK_PRIO);

    for(i=SWRB_WHEEL_TEST_TASK_PRIO;i<SWRB_TEST_TASK_PRIO_BOUND;i++){
        OSTaskSuspend(i);
    }

    OS_EXIT_CRITICAL();
    OSTaskDel(OS_PRIO_SELF);
}

void emWin_Maintask(void *pdata)
{
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);
    //  MainTask();

    hWinEJE_SweepRobot_test_System = CreateEJE_SweepRobot_test_System();

    WM_EnableMemdev(hWinEJE_SweepRobot_test_System);
    WM_MULTIBUF_Enable(1);

    MultiEdit_Set_Text("PUSH KEY0 TO START\r\n");
    MultiEdit_Add_Text("READY FOR TESTING\r\n");

    while(1)
    {
        GUI_Exec();
        OSTimeDly(40);
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
        }
        OSTimeDlyHMSM(0,0,0,5);
    }
}

void SWRB_Test_Ctrl_Task(void *pdata)
{
    SweepRobot_TestInitProc();

	while(1){
    if(gkeyCodeGetFinishFlag == 1){
      switch(gkeyCode){
        /* TEST START/RESUME PRESSED*/
        case 1:
          SweepRobot_TestStartProc();
          break;
        /* TEST PAUSE PRESSED */
        case 2:
          SweepRobot_TestPauseProc();
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

void SweepRobot_TestStartProc(void)
{
    OS_CPU_SR cpu_sr;

    printf("TEST->ON\r\n");
    MultiEdit_Set_Text("\r\n");
    OS_ENTER_CRITICAL();
    OSTaskResume(swrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;
}

void SweepRobot_TestPauseProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    OSTaskSuspend(swrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
    printf("LWHEEL->SPEED=0\r\n");
    printf("RWHEEL->SPEED=0\r\n");
    printf("LBRUSH->SPEED=0\r\n");
    printf("RBRUSH->SPEED=0\r\n");
    printf("MBRUSH->SPEED=0\r\n");
    printf("FAN->SPEED=0\r\n");
    printf("SENSOR->IFRD_LED=0\r\n");
    MultiEdit_Add_Text("TEST PAUSED\r\n");
    MultiEdit_Add_Text("PRESS KEY0 TO RESUME TEST\r\n");

    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;  
}

void SweepRobot_TestStopProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    OSTaskSuspend(swrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
    SweepRobot_TestInitProc();
    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;
}

void SweepRobot_TestExitProc(void)
{
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    OSTaskSuspend(swrbTestRuningTaskPrio);
    OS_EXIT_CRITICAL();
    SweepRobot_TestInitProc();
    printf("TEST->OFF\r\n");
    MultiEdit_Set_Text("ROBOT EXIT TEST MODE\r\n");
    MultiEdit_Add_Text("PRESS KEY0 TO ENTER TEST MODE AND START TEST\r\n");
    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;
}

void SweepRobot_TestInitProc(void)
{
    u32 i;

    printf("TEST->ON\r\n");
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
    for(i=ID_CHECKBOX_WHEEL;i<ID_CHECKBOX_BOUND;i++){
        Checkbox_Set_State(i, 0);
        Checkbox_Set_Text_Color(i, GUI_BLACK);
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
    Progbar_Set_Value(0);
    MultiEdit_Set_Text("TEST STOPED\r\n");
    MultiEdit_Add_Text("PRESS KEY0 TO RESTART TEST\r\n");

    swrbTestTaskCnt = 0;
    swrbTestStateMap = 0;
    swrbTestRuningTaskPrio = SWRB_TEST_START_TASK_BOUND+1;
    gkeyCode = 0;
    gkeyCodeGetFinishFlag = 0;
}

/*
void SweepRobot_Test_Test_Task(void *pdata)
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
