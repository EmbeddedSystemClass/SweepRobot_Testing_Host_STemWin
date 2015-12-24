#include "sweeprobot_testing_key.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static KEY_TestTypeDef key;

static void SweepRobot_KeyTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_KEY_TEST_TASK_PRIO;
    
    str = "\r\n>>>KEY TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    
    SweepRobot_KeyTestCtrlTestPos();
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);

    key.value = 0;
    key.validCnt = 0;
    key.validFlag = 0;
}

static void SweepRobot_KeyTestProc(void)
{
    u8 i;
    char *str;
    
    if(!key.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("KEY->READ\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                key.value = usartRxNum;
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
                usartRxNum = 0;
                usartRxFlag = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        
        if(key.value){
            key.validFlag = 1;
        }
        
        if(key.validFlag){
            SweepRobot_KeyTestCtrlIdlePos();
        }
    }
    
    if(key.validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_KeyTestCtrlIdlePos();

        SWRB_TestDataSaveToFile(KEY_TestDataSave);
        
        str = "ERROR->KEY\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "KEY OK\r\n");
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_KEY, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY, "KEY OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_KEY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_KeyTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_KeyTestCtrlIdlePos();

    SWRB_TestDataSaveToFile(KEY_TestDataSave);

    str = "ERROR->KEY\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_KEY, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY, "KEY ERROR");
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_KEY_TEST_TASK_PRIO);
#endif
}

void SweepRobot_KeyTestTask(void *pdata)
{
    
    SweepRobot_KeyTestGPIOInit();
    
    while(1){
        
        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_KEY)){
            SWRB_NextTestTaskResumePreAct(SWRB_KEY_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
        
            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_KeyTestInit();
            }
            
            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_KeyTestProc();
            }
        
            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_KeyTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void KEY_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_KEY_VALUE_POS] = key.value;
    SWRB_TestDataFileWriteData("KEY->Value=", key.value, 1);
}
