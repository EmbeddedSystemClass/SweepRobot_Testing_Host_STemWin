#include "includes.h"
#include "sweeprobot_testing.h"

#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSDErrCHN;

static void SWRBTest_CtrlPanelInit(void)
{
    LED_Init();
    KEY_Init();
    TFTLCD_Init();
    SweepRobotTest_StepMotorDriverGPIOInit();
    SweepRobot_CollisionTestGPIOInit();
    SweepRobot_WheelFloatTestGPIOInit();
}

static int SWRBTest_StorageInit(void)
{
    if(SD_Init()){
        gSwrbTestSDCardInsertState = DISABLE;
    }else{
        gSwrbTestSDCardInsertState = ENABLE;
    }

    W25QXX_Init();
    FSMC_SRAM_Init();

    if(exfans_init()){
        return -1;
    }

    mem_init(SRAMIN);
    mem_init(SRAMEX);
    mem_init(SRAMCCM);

    f_mount(fs[0],"0:",1);
    f_mount(fs[1],"1:",1);
    
    return 0;
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    uart_init(115200);
    USER_RTC_Init();
    SWRBTest_CtrlPanelInit();

    if(SWRBTest_StorageInit()){
        goto MEM_INIT_FAULT;
    }

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
    
INFINITY_LOOP:
    while(1){
        
    }

    /* XXX: should never get here */
    return -1;
}
