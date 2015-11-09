#include "sweeprobot_testing.h"

#define START_TASK_PRIO				0
#define START_STK_SIZE			  128
OS_STK	START_TASK_STK[START_STK_SIZE];

#define TOUCH_TASK_PRIO				2
#define TOUCH_STK_SIZE				128
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];

#define KEY_TASK_PRIO					3
#define KEY_STK_SIZE					256
OS_STK KEY_TASK_STK[KEY_STK_SIZE];

#define TEST_TASK_PRIO        4
#define TEST_STK_SIZE         2048
OS_STK TEST_TASK_STK[TEST_STK_SIZE];

#define USART_TASK_PRIO       5
#define USART_STK_SIZE        256
OS_STK USART_TASK_STK[USART_STK_SIZE];

#define EMWINDEMO_TASK_PRIO		6
#define EMWINDEMO_STK_SIZE		2048
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];

#define LED0_TASK_PRIO 				10
#define LED0_STK_SIZE					64
OS_STK LED0_TASK_STK[LED0_STK_SIZE];

#define KEYMSG_Q_NUM	1
#define DATAMSG_Q_NUM	4
OS_Q KEY_Msg;
OS_Q DATA_Msg;

static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;

static void Start_Task(void *pdata);
static void emWin_Maintask(void *pdata);
static void Touch_Task(void *pdata);
static void Led_Task(void *pdata);
static void Key_Task(void *pdata);
static void Usart_Task(void *pdata);
static void Test_Task(void *pdata);
static void SweepRobot_Test_Start(void *pdata);

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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);

	OSStatInit();

	OS_ENTER_CRITICAL();

	OSTaskCreate(emWin_Maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWINDEMO_TASK_PRIO);
	OSTaskCreate(Touch_Task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
	OSTaskCreate(Led_Task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);
	OSTaskCreate(Key_Task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
  OSTaskCreate(Usart_Task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);
	OSTaskCreate(SweepRobot_Test_Start,(void*)0,(OS_STK*)&TEST_TASK_STK[TEST_STK_SIZE-1],TEST_TASK_PRIO);

	OSTaskSuspend(OS_PRIO_SELF);
	OS_EXIT_CRITICAL();
}

void emWin_Maintask(void *pdata)
{
  GUI_Init();
  WM_SetCreateFlags(WM_CF_MEMDEV);

  hWinEJE_SweepRobot_test_System = CreateEJE_SweepRobot_test_System();

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
  static int   usartRxNum;
  while(1){
    if(USART_RX_STA & (0x8000) ){
//      printf("%s\r\n",USART_RX_BUF);
      USART_RxArrayToString(USART_RX_BUF, &usartRxStr);
      USART_RxArrayToNumber(USART_RX_BUF, &usartRxNum);
      MultiEdit_Add_Text(usartRxStr);
//      printf("%d\r\n",usartRxNum);
      USART_RX_STA = 0;
		}
    OSTimeDlyHMSM(0,0,0,20);
  }
}

void Test_Task(void *pdata)
{
	while(1){
    if(gkeyCodeGetFinishFlag == 1){
      switch(gkeyCode){
        case 1:
          Progbar_Set_Value(10);
          printf("RWHEEL->SPEED=10\r\n");
          printf("LWHEEL->SPEED=10\r\n");
          MultiEdit_Add_Text("RWHEEL->SPEED=10\r\n");
          MultiEdit_Add_Text("LWHEEL->SPEED=10\r\n");
          Checkbox_Set_State(ID_CHECKBOX_0, 1);
          Checkbox_Set_Color(ID_CHECKBOX_0, GUI_RED);
          Checkbox_Set_Text(ID_CHECKBOX_0, "WHEEL ERROR\r\n");
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 2:
          Progbar_Set_Value(20);
          printf("RWHEEL->SPEED=20\r\n");
          MultiEdit_Add_Text("RWHEEL->SPEED=20\r\n");
          Checkbox_Set_State(ID_CHECKBOX_1, 1);
          Checkbox_Set_Color(ID_CHECKBOX_1, GUI_RED);
          Checkbox_Set_Text(ID_CHECKBOX_1, "SBRUSH ERROR\r\n");
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 3:
          Progbar_Set_Value(30);
          printf("RWHEEL->SPEED=30\r\n");
          MultiEdit_Add_Text("RWHEEL->SPEED=30\r\n");
          Checkbox_Set_State(ID_CHECKBOX_2, 1);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 4:
          Progbar_Set_Value(0);
          printf("RWHEEL->SPEED=0\r\n");
          printf("LWHEEL->SPEED=0\r\n");
          MultiEdit_Add_Text("RWHEEL->SPEED=0\r\n");
          MultiEdit_Add_Text("LWHEEL->SPEED=0\r\n");
          Checkbox_Set_State(ID_CHECKBOX_3, 1);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        default:
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
      }
    }
    OSTimeDlyHMSM(0,0,0, 100);
  }
}

void SweepRobot_Test_Start(void *pdata)
{
  while(1){
    if(gkeyCodeGetFinishFlag == 1){
      switch(gkeyCode){
        case 1:
          TIM_Cmd(TIM3,ENABLE);
          gkeyCode = 0;
          gkeyCodeGetFinishFlag = 0;
          break;
        case 2:

          break;
        case 3:
          break;
        case 4:
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

