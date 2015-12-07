#include "includes.h"

#include "sweeprobot_testing.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSDErrCHN;

static const char *pSDErrStr ="\xe8\xaf\xb7\xe6\x8f\x92\xe5\x85\xa5SD\xe5\x8d\xa1";

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

#ifdef DATA_IN_ExtSRAM

#else
    W25QXX_Init();
    FSMC_SRAM_Init();
#endif

    if(exfans_init()){
        goto MEM_INIT_FAULT;
    }

	mem_init(SRAMIN);
	mem_init(SRAMEX);
	mem_init(SRAMCCM);

	f_mount(fs[0],"0:",1);
	f_mount(fs[1],"1:",1);

	OSInit();
	OS_Task_Create();
	OSStart();

MEM_INIT_FAULT:
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    GUI_SetFont(GUI_FONT_32_ASCII);
    GUI_SetColor(GUI_RED);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DispStringAt("MEM_INIT_FAULT!", 270, 200);
    GUI_SetFont(GUI_DEFAULT_FONT);
    GUI_SetColor(GUI_DEFAULT_COLOR);
    GUI_SetBkColor(GUI_DEFAULT_BKCOLOR);
    goto INFINITY_LOOP;

NO_SD_FAULT:
    FSMC_SRAM_Init();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();

    GUI_SetColor(GUI_RED);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    GUI_SetFont(&GUI_FontSDErrCHN);
    GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt(pSDErrStr, 320, 200);
    
    GUI_SetFont(GUI_FONT_32_ASCII);
    GUI_DispStringAt("Please insert SD card!", 270, 250);
    GUI_SetFont(GUI_DEFAULT_FONT);
    GUI_SetColor(GUI_DEFAULT_COLOR);
    GUI_SetBkColor(GUI_DEFAULT_BKCOLOR);

INFINITY_LOOP:
	while(1){
        
	}

    /* XXX: should never get here */
	return -1;
}
