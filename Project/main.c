#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"
#include "led.h"
#include "timer.h"
#include "touch.h"
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "includes.h"
#include "memdev.h"
#include "key.h"
#include "WM.h"

#include "SweepRobot_Remote_controllerDLG.h"

#define START_TASK_PRIO				0
#define START_STK_SIZE			  128
OS_STK	START_TASK_STK[START_STK_SIZE];
void start_task(void *pdata);

#define TOUCH_TASK_PRIO				2
#define TOUCH_STK_SIZE				128
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
void touch_task(void *pdata);

#define LED0_TASK_PRIO 				3
#define LED0_STK_SIZE					64
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *pdata);

#define EMWINDEMO_TASK_PRIO		4
#define EMWINDEMO_STK_SIZE		2048
OS_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
void emwin_maintask(void *pdata);

#define KEY_TASK_PRIO					5
#define KEY_STK_SIZE					256
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
void key_task(void *pdata);

WM_HWIN hWinEJE_SweepRobot_test_System;

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

	exfuns_init();																						//Ϊfatfs�ļ�ϵͳ�����ڴ�
	f_mount(fs[0],"0:",1);																		//����SD��
	f_mount(fs[1],"1:",1);																		//����FLASH
	TP_Init();																								//��ʼ��������
	OSInit();  																								//��ʼ��UCOS
	OSTaskCreate(start_task,  																//start_task����
							(void*)0,    																	//����
							(OS_STK*)&START_TASK_STK[START_STK_SIZE-1], 	//�����ջջ��
							START_TASK_PRIO);  														//�������ȼ�
	OSStart();  																							//����UCOS
							
	while(1){
		
	}
	
	return -1;
}

//START����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);				//����CRCʱ��				//�����д�����ʹ�ô洢�豸
	GUI_Init();  																							//emwin��ʼ��

	OSStatInit(); 																						//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  																		//�����ٽ���,�ر��ж�

	OSTaskCreate(emwin_maintask,(void*)0,(OS_STK*)&EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE-1],EMWINDEMO_TASK_PRIO);
	OSTaskCreate(touch_task,(void*)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);

	OSTaskSuspend(OS_PRIO_SELF); //����start����
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�
}

void emwin_maintask(void *pdata)
{
	GUI_Init();
	WM_SetCreateFlags(WM_CF_MEMDEV);

//	hWinEJE_SweepRobot_test_System = CreateEJE_SweepRobot_test_System();
	CreateSweepRobot_Remote_controller();
	while(1)
	{
		//_DemoMemDev();
//		MainTask();
		GUI_Exec();
		OSTimeDly(50);
	}
}

void touch_task(void *pdata)
{
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,5);//��ʱ5ms
	}
}

void led0_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);//��ʱ500ms
	}
}

void key_task(void *pdata)
{
	while(1){
		
	}		
}
