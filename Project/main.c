#include "includes.h"

#include "delay.h"
#include "usart.h"

#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"

#include "sram.h"
#include "ILI93xx.h"
#include "touch.h"
#include "led.h"
#include "key.h"

#include "sweeprobot_testing.h"

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	TFTLCD_Init();
  TP_Init();
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

	OSInit();
	OS_Task_Create();
	OSStart();

	while(1){

	}

	return -1;
}

