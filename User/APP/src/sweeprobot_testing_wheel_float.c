#include "sweeprobot_testing_wheel_float.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static WHEEL_FLOAT_TestTypeDef wheelFloat[SWRB_WHEEL_FLOAT_CHAN_NUM];

void SweepRobot_WheelFloatTestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_WHEEL_FLOAT_TEST_TASK_PRIO;
    
    str = "\r\n>>>WHEEL FLOAT TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef __SHOW_TEST_TITLE    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    SweepRobot_WheelFloatCtrlMoveToTestPos();
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);

    for(i=0;i<SWRB_WHEEL_FLOAT_CHAN_NUM;i++){
        wheelFloat[i].value = 0;
        wheelFloat[i].validCnt = 0;
        wheelFloat[i].validFlag = 0;
    }
}

static void SweepRobot_WheelFloatTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_WHEEL_FLOAT_CHAN_NUM;i++){
        
        if(!wheelFloat[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("WF->RD=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    wheelFloat[i].value = usartRxNum;
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    USART_RX_STA = 0;
                    break;
                }else{
                    continue;
                }
            }
            if(wheelFloat[i].value){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
            }
            if(wheelFloat[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                wheelFloat[i].validFlag = 1;
            }
        }
    }
    
    if(wheelFloat[WHEEL_FLOAT_CHAN_L].validFlag && wheelFloat[WHEEL_FLOAT_CHAN_R].validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_WheelFloatCtrlMoveToIdlePos();

        SWRB_TestDataSaveToFile(SweepRobot_WheelFloatTestDataSave);
        
        str = "WHEEL FLOAT OK\r\n";
        SWRB_TestDataFileWriteString(str);
//        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);

        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    }
}

static void SweepRobot_WheelFloatTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_WheelFloatCtrlMoveToIdlePos();
    
    SWRB_TestDataSaveToFile(SweepRobot_WheelFloatTestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK){
        str = "ERROR->WHEEL FLOAT L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK){
        str = "ERROR->WHEEL FLOAT R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT ERR");
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
#endif
}

void SweepRobot_WheelFloatTestTask(void *pdata)
{
    
    SweepRobot_WheelFloatTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_WHEEL_FLOAT)){
            SWRB_NextTestTaskResumePreAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
        }else{
        
            gSwrbTestTaskRunCnt++;
            
            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_WheelFloatTestInit();
            }
            
            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_WheelFloatTestProc();
            }
        
            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_WheelFloatTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void SweepRobot_WheelFloatTestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_L_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_L].value;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_R_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_R].value;
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->L_Value=", wheelFloat[WHEEL_FLOAT_CHAN_L].value, ENABLE);
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->R_Value=", wheelFloat[WHEEL_FLOAT_CHAN_R].value, ENABLE);
}