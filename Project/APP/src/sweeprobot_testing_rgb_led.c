#include "sweepRobot_testing_rgb_led.h"

#include "usart.h"
#include "includes.h"

static RGB_LED_TestTypeDef rgb_led;

static void SweepRobot_RGBLEDTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_RGB_LED_TEST_TASK_PRIO;
    
    str = "\r\n>>>RGB LED TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);

    OSTimeDlyHMSM(0,0,2,0);
    
    rgb_led.r_state = 0;
    rgb_led.r_state = 0;
    rgb_led.r_state = 0;
    rgb_led.validCnt = 0;
    rgb_led.validFlag = 0;
}

static void SweepRobot_RGBLEDTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    if(gSwrbTestStateMap){
        printf("RGB_LED->ON=1\r\n");
    }else{
        printf("RGB_LED->ON=2\r\n");
    }
    
    SWRB_TestDataSaveToFile(RGB_LED_TestDataSave);

    str = "ERROR->RGB LED\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(str);
    Checkbox_Set_Text_Color(ID_CHECKBOX_RGB_LED, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_RGB_LED, "RGB LED ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_RGB_LED);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_RGB_LED_TEST_TASK_PRIO);
}

void SweepRobot_RGB_LED_Test_Task(void *pdata)
{
    while(1){
      
        if(!Checkbox_Get_State(ID_CHECKBOX_RGB_LED)){
            SWRB_NextTestTaskResumePreAct(SWRB_RGB_LED_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt ==1){
                SweepRobot_RGBLEDTestInit();
            }

            if(gSwrbTestTaskRunCnt == 20){
                printf("RGB_LED->ON=1\r\n");
            }else if(gSwrbTestTaskRunCnt == 60){
                printf("RGB_LED->ON=2\r\n");
            }else if(gSwrbTestTaskRunCnt == 100){
                printf("RGB_LED->ON=4\r\n");
            }

            if(gSwrbTestTaskRunCnt > 140){
                SweepRobot_RGBLEDTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void RGB_LED_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("RGB_LED->R_State=", rgb_led.r_state);
    SWRB_TestDataFileWriteData("RGB_LED->G_State=", rgb_led.g_state);
    SWRB_TestDataFileWriteData("RGB_LED->B_State=", rgb_led.b_state);
}
