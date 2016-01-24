#include "includes.h"
#include "sweeprobot_testing.h"

#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSDErrCHN;

u8 USB_HOST_User_App(void)
{
    while(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
        LED1=!LED1;
        gSwrbTestUDiskInsertState = ENABLE;
        OSTimeDlyHMSM(0,0,0,200);
    }
    gSwrbTestUDiskInsertState = DISABLE;
    LED1 = 1;

    return 0;
}

static void SWRBTest_CtrlPanelInit(void)
{
    LED_Init();
    KEY_Init();
    TFTLCD_Init();
}

static void SWRBTest_DeviceInit(void)
{
    SweepRobotTest_SteerMotor5VCtrlGPIOInit();
    SweepRobotTest_StepMotorDriverGPIOInit();
    SweepRobot_CollisionTestGPIOInit();
    SweepRobot_WheelFloatTestGPIOInit();
    SweepRobot_IrDATestGPIOInit();
    SweepRobot_ChargeTestGPIOInit();
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
    f_mount(fs[2],"2:",1);
    
    USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);

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

    SWRBTest_DeviceInit();

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

//    /* XXX: should never get here */
//    return -1;
}
