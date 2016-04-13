#include "sweeprobot_testing_wheel_float.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_WHEEL_FLOAT_TEST_TIME_OUT_CNT      50

#define WHEEL_FLOAT_TEST_INIT_WAIT_TIME_SEC     1
#define WHEEL_FLOAT_TEST_MOVE_WAIT_TIME_MS      500

static WHEEL_FLOAT_TestTypeDef wheelFloat[SWRB_WHEEL_FLOAT_CHAN_NUM];

void SweepRobot_WheelFloatTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_WHEEL_FLOAT_TEST_TASK_PRIO;

    str = "\r\n>>>WHEEL FLOAT TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);

    for(i=0;i<SWRB_WHEEL_FLOAT_CHAN_NUM;i++){
        mymemset(&wheelFloat[i], 0, sizeof(wheelFloat[i]));
    }

    SweepRobot_WheelFloatCtrlSteerMotorPosMove(STEER_MOTOR_DOWN_POS);

    OSTimeDlyHMSM(0,0,0,300);
}

static void SweepRobot_WheelFloatTestDownPosRxDataProc(enum WheelFloatChan chan)
{
    int i;
    char *str;

    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
        printf("WF->RD=%d\r\n",chan);
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            wheelFloat[chan].downValue = usartRxNum;
            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1+i, usartRxNum);
            }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                str = mymalloc(SRAMIN, sizeof(char)*10);
                *str = 0;
                sprintf(str, "%d", usartRxNum);
                Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][0],\
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][1],\
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
}

/* TODO: Add Wheel Float Up Pos and Down Pos Test */
static void SweepRobot_WheelFloatTestDownPosProc(void)
{
    u8 i;

    for(i=0;i<SWRB_WHEEL_FLOAT_CHAN_NUM;i++){
        if(!wheelFloat[i].downValidFlag){
            SweepRobot_WheelFloatTestDownPosRxDataProc((enum WheelFloatChan)i);

            if(wheelFloat[i].downValue){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].downValidCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].downValidCnt = 0;
            }

            if(wheelFloat[i].downValidCnt){
                wheelFloat[i].downValidFlag = 1;
            }

            if(wheelFloat[WHEEL_FLOAT_CHAN_L].downValidFlag && wheelFloat[WHEEL_FLOAT_CHAN_R].downValidFlag){
                SweepRobot_WheelFloatCtrlSteerMotorPosMove(STEER_MOTOR_UP_POS);
                OSTimeDlyHMSM(0,0,0,WHEEL_FLOAT_TEST_MOVE_WAIT_TIME_MS);
            }
        }
    }
}

static void SweepRobot_WheelFloatTestUpPosRxDataProc(enum WheelFloatChan chan)
{
    int i;
    char *str;

    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
        printf("WF->RD=%d\r\n",chan);
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            wheelFloat[chan].upValue = usartRxNum;
            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
            }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                str = mymalloc(SRAMIN, sizeof(char)*10);
                *str = 0;
                sprintf(str, "%d", usartRxNum);
                Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][0],\
                                        gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][1],\
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
}

static void SweepRobot_WheelFloatTestUpPosProc(void)
{
    u8 i;

    for(i=0;i<SWRB_WHEEL_FLOAT_CHAN_NUM;i++){
        if(!wheelFloat[i].validFlag){
            SweepRobot_WheelFloatTestUpPosRxDataProc((enum WheelFloatChan)i);

            if(!wheelFloat[i].upValue){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].upValidCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
            }

            if(wheelFloat[i].upValidCnt){
                wheelFloat[i].validFlag = 1;

                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][0],\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS+i][1],\
                                                               SWRB_MANUAL_TEST_OK_BK_COLOR);
                }
            }
        }
    }
}

static void SweepRobot_WheelFloatTestProc(void)
{
    char *str;

    if(wheelFloat[WHEEL_FLOAT_CHAN_L].downValidFlag && wheelFloat[WHEEL_FLOAT_CHAN_R].downValidFlag){
        SweepRobot_WheelFloatTestUpPosProc();
    }else{
        SweepRobot_WheelFloatTestDownPosProc();
    }

    if(wheelFloat[WHEEL_FLOAT_CHAN_L].validFlag && wheelFloat[WHEEL_FLOAT_CHAN_R].validFlag){
        gSwrbTestTaskRunCnt = 0;

        SweepRobot_WheelFloatCtrlSteerMotorPosMove(STEER_MOTOR_DOWN_POS);

        SWRB_TestDataSaveToFile(SweepRobot_WheelFloatTestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "WHEEL FLOAT OK\r\n";
            SWRB_TestDataFileWriteString(str);
    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);

            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT OK");
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    }
}

static void SweepRobot_WheelFloatPCBTestTimeOutProc(void)
{
    char *str;

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "F1\r\n";
#else
        str = "ERROR->WHEEL_FLOAT_L\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "F2\r\n";
#else
        str = "ERROR->WHEEL_FLOAT_R\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT ERR");
    Edit_Clear();
}

static void SweepRobot_WheelFloatManualTestTimeOutProc(void)
{
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS][0],\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_L_POS][1],\
                                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_R_POS][0],\
                                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_WHEEL_FLOAT_R_POS][1],\
                                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
}

static void SweepRobot_WheelFloatTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_WheelFloatCtrlSteerMotorPosMove(STEER_MOTOR_DOWN_POS);

    SWRB_TestDataSaveToFile(SweepRobot_WheelFloatTestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_WheelFloatPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_WheelFloatManualTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
#endif
}

static void SWRB_WheelFloatTestProgDisp(void)
{
    Progbar_ManualTest_Set_Percent(gSwrbTestTaskRunCnt, SWRB_WHEEL_FLOAT_TEST_TIME_OUT_CNT);
}

void SweepRobot_WheelFloatTestTask(void *pdata)
{
    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL_FLOAT)){
            SWRB_NextTestTaskResumePreAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
        }else{
            SWRB_WheelFloatTestProgDisp();
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_WheelFloatTestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_WheelFloatTestProc();
            }

            if(gSwrbTestTaskRunCnt > SWRB_WHEEL_FLOAT_TEST_TIME_OUT_CNT){
                SweepRobot_WheelFloatTestTimeOutProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void SweepRobot_WheelFloatTestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_L_DOWN_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_L].downValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_R_DOWN_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_R].downValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_L_UP_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_L].upValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_R_UP_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_R].upValue;
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->L_downValue=", wheelFloat[WHEEL_FLOAT_CHAN_L].downValue, ENABLE);
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->R_downValue=", wheelFloat[WHEEL_FLOAT_CHAN_R].downValue, ENABLE);
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->L_upValue=", wheelFloat[WHEEL_FLOAT_CHAN_L].upValue, ENABLE);
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->R_upValue=", wheelFloat[WHEEL_FLOAT_CHAN_R].upValue, ENABLE);
}
