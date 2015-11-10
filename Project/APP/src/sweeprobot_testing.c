#include "sweeprobot_testing.h"
#include "sweeprobot_testing_conf.h"

#include "usart.h"
#include "stm32f4xx_it.h"

u8 usartRxFlag = 0;
int usartRxNum = 0;

u32 swrbTestStateMap = 0;
u32 lastSwrbTestStateMap = 0;

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
  
  for(i=SWRB_WHEEL_TEST_TASK_PRIO;i<SWRB_TEST_TASK_BOUND;i++){
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

  MultiEdit_Add_Text("PUSH KEY0 TO START\r\n");
  MultiEdit_Set_Text("READY FOR TESTING\r\n");

  while(1)
  {
    GUI_Exec();
    OSTimeDly(40);
  }
}

void Touch_Task(void *pdata)
{
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
  static char *usartRxStr;

  while(1){
    if(USART_RX_STA & (0x8000) ){
//      printf("%s\r\n",USART_RX_BUF);
      USART_RxArrayToString(USART_RX_BUF, &usartRxStr);
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
	while(1){
    if(gkeyCodeGetFinishFlag == 1){
      switch(gkeyCode){
        case 1:
          printf("TEST->ON\r\n");
          MultiEdit_Set_Text("SWEEPROBOT TEST START!\r\n");
          OSTaskResume(SWRB_WHEEL_TEST_TASK_PRIO);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 2:
          printf("TEST->OFF\r\n");
          MultiEdit_Set_Text("TEST OFF\r\n");
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 3:
          printf("TEST->ON\r\n");
          MultiEdit_Set_Text("TEST ON\r\n");
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 4:
          printf("LWHEEL->SPEED=0\r\n");
          printf("RWHEEL->SPEED=0\r\n");
          MultiEdit_Set_Text("TEST PAUSED\r\n");
          OSTaskSuspend(SWRB_WHEEL_TEST_TASK_PRIO);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
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

/*
void SweepRobot_Test_Start(void *pdata)
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
