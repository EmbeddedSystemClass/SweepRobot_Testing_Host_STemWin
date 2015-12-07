#include "sweeprobot_testing_fan.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

//const static int SWRB_TEST_FAN_OC_THRESHOLD = 1000;
const static int SWRB_TEST_FAN_CUR_LOW_BOUND = 20;
const static int SWRB_TEST_FAN_CUR_HIGH_BOUND = 500;

static FAN_TestTypeDef fan;

static void SweepRobot_FanTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_FAN_TEST_TASK_PRIO;
    
    str = "\r\n>>>FAN TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    printf("FAN->SPEED=50\r\n");
    fan.current = 0;
    fan.validCnt = 0;
    fan.validFlag = 0;
}

static void SweepRobot_FanTestProc(void)
{
    u8 i;
    char *str;
    
    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
        printf("FAN->READ\r\n");
        OSTimeDlyHMSM(0,0,0,6);
        if(usartRxFlag){
            fan.current = usartRxNum;
            Edit_Set_Value(ID_MAIN_EDIT_U1, usartRxNum);
            /* TODO: add fan over current protection here */
            usartRxNum = 0;
            usartRxFlag = 0;
            USART_RX_STA = 0;
            break;
        }else{
            continue;
        }
    }
    if(SWRB_TEST_FAN_CUR_LOW_BOUND < fan.current &&  SWRB_TEST_FAN_CUR_HIGH_BOUND > fan.current){
        gSwrbTestStateMap &= ~(1<<SWRB_TEST_FAN_POS);
        fan.validCnt++;
    }else{
        gSwrbTestStateMap |= (1<<SWRB_TEST_FAN_POS);
    }

    if(fan.validCnt > SWRB_TEST_VALID_COMP_TIMES){
        fan.validFlag = 1;
        printf("FAN->SPEED=0\r\n");
    }

    if(fan.validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        SWRB_TestDataSaveToFile(Fan_TestDataSave);
        
        str = "FAN OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_FAN, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, "FAN OK");
        Checkbox_Set_Box_Back_Color(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_FAN_TEST_TASK_PRIO);
    }
}

static void SweepRobot_FanTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("FAN->SPEED=0\r\n");

    SWRB_TestDataSaveToFile(Fan_TestDataSave);
    
    str = "ERROR->FAN\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_FAN, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, "FAN ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_FAN_TEST_TASK_PRIO);
}

void SweepRobot_FanTestTask(void *pdata)
{
    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_FAN)){
            SWRB_NextTestTaskResumePreAct(SWRB_FAN_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
            
            Checkbox_Set_Box_Back_Color(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_FAN, GUI_GREEN, CHECKBOX_CI_ENABLED);

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_FanTestInit();
            }

            if(gSwrbTestTaskRunCnt > 5){
                SweepRobot_FanTestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_FanTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void Fan_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_FAN_CUR_POS] = fan.current;
    SWRB_TestDataFileWriteData("FAN->CUR=", fan.current, 1);
}
