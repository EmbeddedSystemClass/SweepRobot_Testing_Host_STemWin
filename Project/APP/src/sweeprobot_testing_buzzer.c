#include "sweeprobot_testing_buzzer.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static BUZZER_TestTypeDef buzzer;

static void SweepRobot_BuzzerTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_BUZZER_TEST_TASK_PRIO;
    
    str = "\r\n>>>BUZZER TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);

    OSTimeDlyHMSM(0,0,1,0);
    
    hWin_SWRB_BUZZER = CreateBUZZER_TestDLG();
    
    buzzer.state = 0;
    buzzer.validCnt = 0;
    buzzer.validFlag = 0;
}

static void SweepRobot_BuzzerTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    
    SWRB_TestDataSaveToFile(BUZZER_TestDataSave);

    str = "ERROR->BUZZER\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(str);
    Checkbox_Set_Text_Color(ID_CHECKBOX_BUZZER, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_BUZZER, "BUZZER ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_BUZZER);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_BUZZER_TEST_TASK_PRIO);
}

void SweepRobot_Buzzer_Test_Task(void *pdata)
{
    while(1){
      
        if(!Checkbox_Get_State(ID_CHECKBOX_BUZZER)){
            SWRB_NextTestTaskResumePreAct(SWRB_BUZZER_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_BuzzerTestInit();
            }

            if(gSwrbTestTaskRunCnt == 20){
                printf("BUZZER->ON=1\r\n");
            }else if(gSwrbTestTaskRunCnt == 60){
                printf("BUZZER->ON=2\r\n");
            }else if(gSwrbTestTaskRunCnt == 100){
                printf("BUZZER->ON=3\r\n");
            }

            if(gSwrbTestTaskRunCnt > 140){
                SweepRobot_BuzzerTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void BUZZER_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("BUZZER->Value=", buzzer.state, 1);
}
