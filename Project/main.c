#include "includes.h"

#include "sweeprobot_testing.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
    My_RTC_Init();
	LED_Init();
	KEY_Init();
    TFTLCD_Init();
    TIM3_Int_Init(9999,168-1);
    if(SD_Init()){
        goto NO_SD_FAULT;
    }
    W25QXX_Init();
    FSMC_SRAM_Init();
    exfans_init();
    
	mem_init(SRAMIN);
	mem_init(SRAMEX);
	mem_init(SRAMCCM);
    
	f_mount(fs[0],"0:",1);
	f_mount(fs[1],"1:",1);
    
//    MainTask();
    
	OSInit();
	OS_Task_Create();
	OSStart();

  /* XXX: should never get here */
NO_SD_FAULT:
    FSMC_SRAM_Init();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    GUI_SetFont(GUI_FONT_32_ASCII);
    GUI_SetColor(GUI_RED);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DispStringAt("Please insert SD card!", 270, 240);
    GUI_SetFont(GUI_DEFAULT_FONT);
    GUI_SetColor(GUI_DEFAULT_COLOR);
    GUI_SetBkColor(GUI_DEFAULT_BKCOLOR);
	while(1){
        
	}

	return -1;
}
