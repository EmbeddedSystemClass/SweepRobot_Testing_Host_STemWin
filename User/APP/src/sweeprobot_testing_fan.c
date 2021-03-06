#include "sweeprobot_testing_fan.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_FAN_TEST_TIME_OUT_CNT  200

const static int SWRB_TEST_FAN_CUR_LOW_THRESHOLD = 20;
const static int SWRB_TEST_FAN_CUR_HIGH_THRESHOLD = 500;

static FAN_TestTypeDef fan;

static void SweepRobot_FanTestInit(void)
{
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_FAN_TEST_TASK_PRIO;

    str = "\r\n>>>FAN TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    mymemset(&fan, 0, sizeof(fan));

    printf("FAN->SPD=50\r\n");
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_FanTestProc(void)
{
    u8 i;
    char *str;

    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
        printf("FAN->RD\r\n");
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            fan.current = usartRxNum;
            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
            }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                str = mymalloc(SRAMIN, sizeof(char)*10);
                *str = 0;
                sprintf(str, "%d", usartRxNum);
                Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][0],\
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][1],\
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
    if(SWRB_TEST_FAN_CUR_LOW_THRESHOLD < fan.current &&  SWRB_TEST_FAN_CUR_HIGH_THRESHOLD > fan.current){
        gSwrbTestStateMap &= ~(1<<SWRB_TEST_FAN_POS);
        fan.validCnt++;
    }else{
        gSwrbTestStateMap |= (1<<SWRB_TEST_FAN_POS);
    }

    if(fan.validCnt > SWRB_TEST_VALID_COMP_TIMES){
        fan.validFlag = 1;
        printf("FAN->SPD=0\r\n");

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                       gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][0],\
                                                       gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][1],\
                                                       SWRB_MANUAL_TEST_OK_BK_COLOR);
        }
    }

    if(fan.validFlag){
        gSwrbTestTaskRunCnt = 0;

        SWRB_TestDataSaveToFile(Fan_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "FAN OK\r\n";
            SWRB_TestDataFileWriteString(str);

    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_FAN, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN, "FAN OK");
            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_FAN_TEST_TASK_PRIO);
    }
}

static void SweepRobot_FanPCBTestTimeOutProc(void)
{
    char *str;

#ifdef  USE_ERROR_CODE_DISP
    str = "C1\r\n";
#else
    str = "ERROR->FAN\r\n";
#endif
    SWRB_TestDataFileWriteString("ERROR->FAN\r\n");

    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_FAN, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN, "FAN ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();
}

static void SweepRobot_FanManualTestTimeOutProc(void)
{
    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                           gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][0],\
                                           gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_FAN_CUR_POS][1],\
                                           SWRB_MANUAL_TEST_FAULT_BK_COLOR);
}

static void SweepRobot_FanTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("FAN->SPD=0\r\n");

    SWRB_TestDataSaveToFile(Fan_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_FanPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_FanManualTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_FAN_TEST_TASK_PRIO);
#endif
}

static void SWRB_FanTestProgDisp(void)
{
    Progbar_ManualTest_Set_Percent(gSwrbTestTaskRunCnt, SWRB_FAN_TEST_TIME_OUT_CNT);
}

void SweepRobot_FanTestTask(void *pdata)
{
    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_FAN)){
            SWRB_NextTestTaskResumePreAct(SWRB_FAN_TEST_TASK_PRIO);
        }else{
            SWRB_FanTestProgDisp();
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_FanTestInit();
            }

            if(gSwrbTestTaskRunCnt > 10){
                SweepRobot_FanTestProc();
            }

            if(gSwrbTestTaskRunCnt > SWRB_FAN_TEST_TIME_OUT_CNT){
                SweepRobot_FanTestTimeOutProc();
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
