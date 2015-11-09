#include "includes.h"

#include "delay.h"
#include "usart.h"
#include "malloc.h"

#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"

#include "sram.h"
#include "ILI93xx.h"
#include "timer.h"
#include "touch.h"
#include "led.h"
#include "key.h"

#include "GUI.h"
#include "WM.h"
#include "MULTIEDIT.h"

#include "EJE_SweepRobot_test_SystemDLG.h"

#define START_TASK_PRIO				0
#define START_STK_SIZE			  128
OS_STK	START_TASK_STK[START_STK_SIZE];
static void start_task(void *pdata);

#define TOUCH_TASK_PRIO				2
#define TOUCH_STK_SIZE				128
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
static void touch_task(void *pdata);

#define KEY_TASK_PRIO					3
#define KEY_STK_SIZE					256
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
static void key_task(void *pdata);

#define EMWINDEMO_TASK_PRIO		4
#define EMWINDEMO_STK_SIZE		2048
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
static void emwin_maintask(void *pdata);

#define TEST_TASK_PRIO				5
#define TEST_STK_SIZE					128
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
static void Test_task(void *pdata);

#define LED0_TASK_PRIO 				10
#define LED0_STK_SIZE					64
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
static void led0_task(void *pdata);

#define KEYMSG_Q_NUM	1
#define DATAMSG_Q_NUM	4
OS_Q KEY_Msg;
OS_Q DATA_Msg;

static u8 gkeyCode = 0;
static u8 gkeyCodeGetFinishFlag = 0;

int main(void)
{
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	TFTLCD_Init();
	W25QXX_Init();
	LED_Init();
	KEY_Init();
	FSMC_SRAM_Init();

	mem_init(SRAMIN);
	mem_init(SRAMEX);
	mem_init(SRAMCCM);

	exfuns_init();																						//为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);																		//挂载SD卡
	f_mount(fs[1],"1:",1);																		//挂载FLASH
	TP_Init();
	OSInit();
	OSTaskCreate(start_task,
							(void*)0,
							(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],
							START_TASK_PRIO);
	OSStart();

	while(1){

	}

	return -1;
}

//START任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);				//开启CRC时钟				//在所有窗口上使用存储设备
	GUI_Init();  																							//emwin初始化

	OSStatInit(); 																						//初始化统计任务
	OS_ENTER_CRITICAL();  																		//进入临界区,关闭中断

	OSTaskCreate(emwin_maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWINDEMO_TASK_PRIO);
	OSTaskCreate(touch_task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
	OSTaskCreate(Test_task,(void*)0,(OS_STK*)&TEST_TASK_STK[TEST_STK_SIZE-1],TEST_TASK_PRIO);

	OSTaskSuspend(OS_PRIO_SELF); //挂起start任务
	OS_EXIT_CRITICAL();  //退出临界区,开中断
}

void emwin_maintask(void *pdata)
{
	GUI_Init();
	WM_SetCreateFlags(WM_CF_MEMDEV);
	
	hWinEJE_SweepRobot_test_System = CreateEJE_SweepRobot_test_System();
	while(1)
	{
		if(USART_RX_STA & (0x8000) ){
			printf("%s\r\n",USART_RX_BUF);
			USART_RX_STA = 0;
		}
		GUI_Exec();
		OSTimeDly(50);
	}
}

void touch_task(void *pdata)
{
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,5);
	}
}

void led0_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);
	}
}

void key_task(void *pdata)
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

void Test_task(void *pdata)
{
	while(1){
		if(gkeyCodeGetFinishFlag == 1){
			switch(gkeyCode){
				case 1:
					Progbar_Set_Value(10);
					printf("RWHEEL->SPEED=10\r\n");
					printf("LWHEEL->SPEED=10\r\n");
					MultiEdit_Set_Text("RWHEEL->SPEED=10\r\n");
                    MultiEdit_Set_Text("LWHEEL->SPEED=10\r\n");
                    Checkbox_Set_State(ID_CHECKBOX_0, 1);
					gkeyCode = 0;
					gkeyCodeGetFinishFlag = 0;
					break;
				case 2:
					Progbar_Set_Value(20);
					printf("RWHEEL->SPEED=20\r\n");
					MultiEdit_Set_Text("RWHEEL->SPEED=20");
                    Checkbox_Set_State(ID_CHECKBOX_1, 1);
					gkeyCode = 0;
					gkeyCodeGetFinishFlag = 0;
					break;
				case 3:
					Progbar_Set_Value(30);
					printf("RWHEEL->SPEED=30\r\n");
					MultiEdit_Set_Text("RWHEEL->SPEED=30");
                    Checkbox_Set_State(ID_CHECKBOX_2, 1);
					gkeyCode = 0;
					gkeyCodeGetFinishFlag = 0;
					break;
				case 4:
					Progbar_Set_Value(0);
					printf("RWHEEL->SPEED=0\r\n");
					MultiEdit_Set_Text("RWHEEL->SPEED=0");
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
		OSTimeDlyHMSM(0,0,0, 500);
	}
}
