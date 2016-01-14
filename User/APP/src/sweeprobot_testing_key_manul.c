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

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);

    mymemset(&key, 0, sizeof(key));

//    SweepRobot_KeyTestCtrlTestPos();

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_KeyTestProc(void)
{
    u8 i;
    char *str;

    if(!key.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("KEY->RD\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                key.value = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][1],\
                                            str);
                    myfree(SRAMIN, str);
                }
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

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][1],\
                                                           GUI_LIGHTBLUE);
            }
        }
    }

    if(key.validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_KeyTestCtrlIdlePos();

        SWRB_TestDataSaveToFile(KEY_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "KEY OK\r\n";
            SWRB_TestDataFileWriteString(str);

    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "KEY OK\r\n");
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_KEY, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY, "KEY OK");
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_KEY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_KeyPCBTestOverTimeProc(void)
{
    char *str;

    str = "ERROR->KEY\r\n";
    SWRB_TestDataFileWriteString(str);

    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_KEY, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY, "KEY ERROR");
    Edit_Clear();
}

static void SweepRobot_KeyManulTestOverTimeProc(void)
{
    Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_KEY_POS][1],\
                                                           GUI_LIGHTRED);
}

static void SweepRobot_KeyTestOverTimeProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_KeyTestCtrlIdlePos();

    SWRB_TestDataSaveToFile(KEY_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_KeyPCBTestOverTimeProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_KeyManulTestOverTimeProc();
    }

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

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_KEY)){
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
