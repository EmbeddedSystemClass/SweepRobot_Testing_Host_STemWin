#include "sweeprobot_testing_buzzer.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
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
    
#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
#endif
    
    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    mymemset(&buzzer, 0, sizeof(buzzer));
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
    
    hWin_SWRB_BUZZER = CreateBUZZER_TestDLG();
    WM_BringToTop(hWin_SWRB_BUZZER);

    Text_Set_Color(hWin_SWRB_BUZZER, ID_PCBTEST_TEXT_BUZZER, GUI_BLACK);
    Text_Set_Text(hWin_SWRB_BUZZER, ID_PCBTEST_TEXT_BUZZER, "IS BUZZER OK?");
}

static void SweepRobot_BuzzerTestProc(void)
{
    if(!buzzer.validFlag){
        buzzer.validCnt++;
        
        if(buzzer.validCnt >3)
            buzzer.validCnt = 1;
        
        switch(buzzer.validCnt){
            case 1:
                printf("BZR->ON=1\r\n");
                break;
            case 2:
                printf("BZR->ON=2\r\n");
                break;
            case 3:
                printf("BZR->ON=3\r\n");
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
    
    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        str = "BUZZER OK\r\n";
        SWRB_TestDataFileWriteString(str);

    //    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_BUZZER, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BUZZER, "BUZZER OK");
        Edit_Clear();
    }else if (gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][0],\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][1],\
                                                                "OK");
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][0],\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][1],\
                                                                SWRB_MANUAL_TEST_OK_BK_COLOR);
    }

    SWRB_NextTestTaskResumePostAct(SWRB_BUZZER_TEST_TASK_PRIO);
}

static void SweepRobot_BuzzerTestErrProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    
    GUI_EndDialog(hWin_SWRB_BUZZER, 0);
    
    SWRB_TestDataSaveToFile(BUZZER_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
#ifdef  USE_ERROR_CODE_DISP
        str = "K1\r\n";
#else
        str = "ERROR->BUZZER\r\n";
#endif
        SWRB_TestDataFileWriteString(str);

        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_BUZZER, GUI_RED);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BUZZER, "BUZZER ERROR");
        Edit_Clear();
    }else if (gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][0],\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][1],\
                                                                "ERR");
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][0],\
                                                                gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_BUZZER_POS][1],\
                                                                SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_BUZZER_TEST_TASK_PRIO);
#endif
}

void SweepRobot_BuzzerTestTask(void *pdata)
{
    while(1){
      
        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BUZZER)){
            SWRB_NextTestTaskResumePreAct(SWRB_BUZZER_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_BuzzerTestInit();
            }

            if(1==gSwrbTestTaskRunCnt%500){
                SweepRobot_BuzzerTestProc();
            }else{
                ;
            }

            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
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
