#include "sweeprobot_testing_rgb_led.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static RGB_LED_TestTypeDef rgb_led;

static void SweepRobot_RGBLEDTestErrProc(void);

static void SweepRobot_RGBLEDTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_RGB_LED_TEST_TASK_PRIO;
    
    str = "\r\n>>>RGB LED TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

    MultiEdit_Add_Text(str);

    OSTimeDlyHMSM(0,0,1,0);
    
    hWin_SWRB_RGB_LED = CreateRGB_LED_TestDLG();
    
    rgb_led.r_state = 0;
    rgb_led.g_state = 0;
    rgb_led.b_state = 0;
    rgb_led.validCnt = 0;
    rgb_led.validFlag = 0;
}

static void SweepRobot_RGBLEDTestSingleProc(int *ledState, GUI_COLOR color, char *strText, char *strMultiEdit1, char *strMultiEdit2)
{
    Text_Set_Color(hWin_SWRB_RGB_LED, ID_TEXT_RGB_LED, color);
    Text_Set_Text(hWin_SWRB_RGB_LED, ID_TEXT_RGB_LED, strText);
    OSTaskSuspend(gSwrbTestRuningTaskPrio);
    if(*ledState){
        MultiEdit_Add_Text(strMultiEdit1);
    }else{
        MultiEdit_Add_Text(strMultiEdit2);
    }
}

static void SweepRobot_RGBLEDTestProc(void)
{
    char *str;
    
    rgb_led.validCnt = 1;
    printf("RGB_LED->ON=1\r\n");
    SweepRobot_RGBLEDTestSingleProc(&rgb_led.r_state, GUI_RED, "IS RED LED OK?", "RED LED OK\r\n", "ERROR->RED LED\r\n");
    rgb_led.validCnt = 2;
    printf("RGB_LED->ON=2\r\n");
    SweepRobot_RGBLEDTestSingleProc(&rgb_led.g_state, GUI_GREEN, "IS GREEN LED OK?", "GREEN LED OK\r\n", "ERROR->GREEN LED\r\n");
    rgb_led.validCnt = 3;
    printf("RGB_LED->ON=4\r\n");
    SweepRobot_RGBLEDTestSingleProc(&rgb_led.b_state, GUI_BLUE, "IS BLUE LED OK?", "BLUE LED OK\r\n", "ERROR->BLUE LED\r\n");
    
    if(rgb_led.r_state && rgb_led.g_state && rgb_led.b_state){
        gSwrbTestTaskRunCnt = 0;
        printf("RGB_LED->ON=2\r\n");
        GUI_EndDialog(hWin_SWRB_RGB_LED, 0);
        
        SWRB_TestDataSaveToFile(RGB_LED_TestDataSave);
        
        str = "RGB LED OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(str);
        Checkbox_Set_Text_Color(ID_CHECKBOX_RGB_LED, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_RGB_LED, "RGB LED OK");
        Edit_Clear();
        
        SWRB_NextTestTaskResumePostAct(SWRB_RGB_LED_TEST_TASK_PRIO);
    }else{
        SweepRobot_RGBLEDTestErrProc();
    }
}

static void SweepRobot_RGBLEDTestErrProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("RGB_LED->ON=2\r\n");
    GUI_EndDialog(hWin_SWRB_RGB_LED, 0);
    
    SWRB_TestDataSaveToFile(RGB_LED_TestDataSave);

    if(!rgb_led.r_state){
        str = "ERROR->RED LED\r\n";
        SWRB_TestDataFileWriteString(str);
    }
    if(!rgb_led.g_state){
        str = "ERROR->GREEN LED\r\n";
        SWRB_TestDataFileWriteString(str);
    }
    if(!rgb_led.b_state){
        str = "ERROR->BLUE LED\r\n";
        SWRB_TestDataFileWriteString(str);
    }
    
    Checkbox_Set_Text_Color(ID_CHECKBOX_RGB_LED, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_RGB_LED, "RGB LED ERROR");
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_RGB_LED_TEST_TASK_PRIO);
}

void SweepRobot_RGBLEDTestTask(void *pdata)
{
    while(1){
      
        if(!Checkbox_Get_State(ID_CHECKBOX_RGB_LED)){
            SWRB_NextTestTaskResumePreAct(SWRB_RGB_LED_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt ==1){
                SweepRobot_RGBLEDTestInit();
            }

            if(gSwrbTestTaskRunCnt >1){
                SweepRobot_RGBLEDTestProc();
            }

            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void RGB_LED_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("RGB_LED->R_State=", rgb_led.r_state, 1);
    SWRB_TestDataFileWriteData("RGB_LED->G_State=", rgb_led.g_state, 1);
    SWRB_TestDataFileWriteData("RGB_LED->B_State=", rgb_led.b_state, 1);
}

u8 RGB_LED_TestValidCntGet(void)
{
    return rgb_led.validCnt;
}

void RGB_LED_TestRgbStateSet(int state, int value)
{
    switch(state){
        case 1:
            rgb_led.r_state = value;
            break;
        case 2:
            rgb_led.g_state = value;
            break;
        case 3:
            rgb_led.b_state = value;
            break;
        default:break;
    }
}
