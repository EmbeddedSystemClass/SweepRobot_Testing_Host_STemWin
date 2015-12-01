#include "sweeprobot_testing_buzzer.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static BUZZER_TestTypeDef buzzer;

static void SweepRobot_BuzzerTestOKProc(void);
static void SweepRobot_BuzzerTestErrProc(void);

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

    Text_Set_Color(hWin_SWRB_BUZZER, ID_TEXT_BUZZER, GUI_BLACK);
    Text_Set_Text(hWin_SWRB_BUZZER, ID_TEXT_BUZZER, "IS BUZZER OK?");
    
    buzzer.state = 0;
    buzzer.validCnt = 0;
    buzzer.validFlag = 0;
}

static void SweepRobot_BuzzerTestProc(void)
{
    if(!buzzer.validFlag){
        buzzer.validCnt++;
        
        if(buzzer.validCnt >3)
            buzzer.validCnt = 1;
        
        switch(buzzer.validCnt){
            case 1:
                printf("BUZZER->ON=1\r\n");
                break;
            case 2:
                printf("BUZZER->ON=2\r\n");
                break;
            case 3:
                printf("BUZZER->ON=3\r\n");
                break;
        }
    }else{
        if(buzzer.state){
            SweepRobot_BuzzerTestOKProc();
        }else{
            SweepRobot_BuzzerTestErrProc();
        }
    }
}

static void SweepRobot_BuzzerTestOKProc(void)
{
    char *str;
            
    gSwrbTestTaskRunCnt = 0;
    
    SWRB_TestDataSaveToFile(BUZZER_TestDataSave);
    
    GUI_EndDialog(hWin_SWRB_BUZZER, 0);
    
    str = "BUZZER OK\r\n";
    SWRB_TestDataFileWriteString(str);

    MultiEdit_Add_Text(str);
    Checkbox_Set_Text_Color(ID_CHECKBOX_BUZZER, GUI_BLUE);
    Checkbox_Set_Text(ID_CHECKBOX_BUZZER, "BUZZER OK");
    Progbar_Set_Percent(SWRB_TEST_STATE_BUZZER);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_BUZZER_TEST_TASK_PRIO);
}

static void SweepRobot_BuzzerTestErrProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    
    GUI_EndDialog(hWin_SWRB_BUZZER, 0);
    
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

void SweepRobot_BuzzerTestTask(void *pdata)
{
    while(1){
      
        if(!Checkbox_Get_State(ID_CHECKBOX_BUZZER)){
            SWRB_NextTestTaskResumePreAct(SWRB_BUZZER_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_BuzzerTestInit();
            }

            if(gSwrbTestTaskRunCnt%20){
                
            }else{
                SweepRobot_BuzzerTestProc();
            }

            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void BUZZER_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("BUZZER->Value=", buzzer.state, 1);
}

void BUZZER_TestStateSet(u8 stateValue)
{
    buzzer.state = stateValue;
}

void BUZZER_TestValidFlagSet(u8 flagValue)
{
    buzzer.validFlag = flagValue;
}
