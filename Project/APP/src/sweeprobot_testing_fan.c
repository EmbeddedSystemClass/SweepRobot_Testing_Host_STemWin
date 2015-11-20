#include "sweepRobot_testing_fan.h"

#include "usart.h"
#include "includes.h"

#define SWRB_TEST_FAN_OC_THRESHOLD      1000
#define SWRB_TEST_FAN_CUR_LOW_BOUND     20
#define SWRB_TEST_FAN_CUR_HIGH_BOUND    500

static FAN_TestTypeDef fan;

static void SweepRobot_FanTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_FAN_TEST_TASK_PRIO;
    
    str = "\r\n>>>FAN TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    printf("FAN->SPEED=30\r\n");
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
            Edit_Set_Value(ID_EDIT_U1, usartRxNum);
            /* TODO: add fan over current protection here */
            usartRxNum = 0;
            usartRxFlag = 0;
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
        fan.validCnt = 0;
    }

    if(fan.validCnt > SWRB_TEST_VALID_COMP_TIMES){
        fan.validFlag = 1;
        printf("FAN->SPEED=0\r\n");
    }

    if(fan.validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        gSwrbTestAcquiredData[SWRB_TEST_DATA_FAN_CUR_POS] = fan.current;
        SWRB_TestDataSaveToFile(Fan_TestDataSave);
        
        str = "FAN OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(str);
        Checkbox_Set_Text_Color(ID_CHECKBOX_FAN, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_FAN);

        SWRB_NextTestTaskResume(SWRB_FAN_TEST_TASK_PRIO);
    }
}

static void SweepRobot_FanTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("FAN->SPEED=0\r\n");
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_FAN_CUR_POS] = fan.current;
    SWRB_TestDataSaveToFile(Fan_TestDataSave);
    
    str = "ERROR->FAN\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(str);
    Checkbox_Set_Text_Color(ID_CHECKBOX_FAN, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_FAN);

    SWRB_NextTestTaskResume(SWRB_FAN_TEST_TASK_PRIO);
}

void SweepRobot_FanTestTask(void *pdata)
{
    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_FAN)){
            SWRB_NextTestTaskResume(SWRB_FAN_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_FanTestInit();
            }

            if(gSwrbTestTaskRunCnt > 5){
                SweepRobot_FanTestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_FanTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void Fan_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("FAN->CUR=", fan.current);
}
