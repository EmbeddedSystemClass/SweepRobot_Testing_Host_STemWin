#include "sweeprobot_testing_collision.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_COLLISION_TEST_TIME_OUT_CNT   20

#define COLLISION_TEST_RELAY_SWITCH_WAIT_TIME_MS   5
#define COLLISION_TEST_STEER_MOTOR_MOVE_WAIT_TIME_MS    500

static COLLISION_TestTypeDef collision[SWRB_COLLISION_CHAN_NUM];

static u8 gSwrbFrontCollisionTestFinishFlag = 0;

static void SweepRobot_CollisionPCBTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;

    str = "\r\n>>>COLLISION TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        mymemset(&collision[i], 0, sizeof(collision[i]));
    }

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_CollisionPCBTestProc(void)
{
    u8 i,j;
    char *str;

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        if(!collision[i].validFlag){
            if(!collision[i].onValidFlag){
                SweepRobot_CollisionRelayCtrlOn((enum COLLISION_CHAN)i);
                OSTimeDlyHMSM(0,0,0,COLLISION_TEST_RELAY_SWITCH_WAIT_TIME_MS);
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("CLSN->RD=%d\r\n",i);
                    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                    if(usartRxFlag){
                        collision[i].onValue = usartRxNum;
                        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                            Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
                        }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                            str = mymalloc(SRAMIN, sizeof(char)*10);
                            *str = 0;
                            sprintf(str, "%d", usartRxNum);
                            Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][0],\
                                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][1],\
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

                if(!collision[i].onValue){
                    collision[i].onValidCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
                    collision[i].onValidCnt = 0;
                }

                if(collision[i].onValidCnt > 1){
                    collision[i].onValidFlag = 1;
                }
            }else{
                SweepRobot_CollisionRelayCtrlOff((enum COLLISION_CHAN)i);
                OSTimeDlyHMSM(0,0,0,COLLISION_TEST_RELAY_SWITCH_WAIT_TIME_MS);
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("CLSN->RD=%d\r\n",i);
                    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                    if(usartRxFlag){
                        collision[i].offValue = usartRxNum;
                        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                            Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1+i, usartRxNum);
                        }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                            str = mymalloc(SRAMIN, sizeof(char)*10);
                            *str = 0;
                            sprintf(str, "%d", usartRxNum);
                            Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][0],\
                                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][1],\
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

                if(collision[i].offValue){
                    collision[i].offValidCnt++;
                    gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+i));
                }else{
                    gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
                    collision[i].offValidCnt = 0;
                }

                if(collision[i].offValidCnt > 1){
                    collision[i].validFlag = 1;

                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                   gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][0],\
                                                                   gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+i][1],\
                                                                   SWRB_MANUAL_TEST_OK_BK_COLOR);
                    }
                }
            }
        }
    }

    if(collision[COLLISION_CHAN_L].validFlag && collision[COLLISION_CHAN_FL].validFlag && collision[COLLISION_CHAN_R].validFlag && collision[COLLISION_CHAN_FR].validFlag){
        gSwrbTestTaskRunCnt = 0;

        SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);

        SWRB_TestDataSaveToFile(Collision_TestDataSave);

        str = "COLLISION OK\r\n";
        SWRB_TestDataFileWriteString(str);

//        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_COLLISION, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION, "COLLISION OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
    }
}

static void SweepRobot_CollisionPCBTestTimeOutProc(void)
{
    char *str;

    SweepRobot_CollisionRelayCtrlOff(COLLISION_CHAN_ALL);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "E1\r\n";
#else
        str = "ERROR->COLLISION_L\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FL_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "E2\r\n";
#else
        str = "ERROR->COLLISION_FL\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "E3\r\n";
#else
        str = "ERROR->COLLISION_R\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FR_MASK){
#ifdef  USE_ERROR_CODE_DISP
        str = "E4\r\n";
#else
        str = "ERROR->COLLISION_FR\r\n";
#endif
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }

    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_COLLISION, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION, "COLLISION ERROR");
    Edit_Clear();
}

static void SweepRobot_CollisionManualTestTimeOutProc(void)
{
    SweepRobot_CollisionCtrlLeftSteerMotorPosMove(STEER_MOTOR_IDLE_POS);
    SweepRobot_CollisionCtrlRightSteerMotorPosMove(STEER_MOTOR_IDLE_POS);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS][0],\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS][1],\
                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FL_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_FL_POS][0],\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_FL_POS][1],\
                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_R_POS][0],\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_R_POS][1],\
                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FR_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_FR_POS][0],\
                                               gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_FR_POS][1],\
                                               SWRB_MANUAL_TEST_FAULT_BK_COLOR);
    }
}

static void SweepRobot_CollisionTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    gSwrbFrontCollisionTestFinishFlag = 0;

    SWRB_TestDataSaveToFile(Collision_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_CollisionPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_CollisionManualTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
#endif
}

static void SweepRobot_CollisionManualTestInit(void)
{
    u8 i;
    char *str;

    if(!gSwrbFrontCollisionTestFinishFlag){
        gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;

        str = "\r\n>>>COLLISION TEST<<<\r\n";
        SWRB_TestDataFileWriteString(str);

        SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    }

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        if(gSwrbFrontCollisionTestFinishFlag){
            if(i==COLLISION_CHAN_L || i==COLLISION_CHAN_FR){
                mymemset(&collision[i], 0, sizeof(collision[i]));
            }
        }else{
            mymemset(&collision[i], 0, sizeof(collision[i]));
        }
    }

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_CollisionTestDataQuery(enum COLLISION_CHAN chan, int *dest_value)
{
    char *str;
    int i;

    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
        printf("CLSN->RD=%d\r\n",chan);
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        if(usartRxFlag){
            *dest_value = usartRxNum;
            str = mymalloc(SRAMIN, sizeof(char)*10);
            *str = 0;
            sprintf(str, "%d", usartRxNum);
            Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][0],\
                                    gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][1],\
                                    str);
            myfree(SRAMIN, str);
            usartRxNum = 0;
            usartRxFlag = 0;
            USART_RX_STA = 0;
            break;
        }else{
            continue;
        }
    }
}

static void SweepRobot_CollisionTestSteerMotorOffDataValidCmp(enum COLLISION_CHAN chan, int *value, u8 *cnt, u8 *flag)
{
    SweepRobot_CollisionTestDataQuery(chan, value);

    if(*value){
        (*cnt)++;
        gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+chan));
    }else{
        gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+chan));
    }

    if((*cnt) > 1){
        *flag = 1;
    }
}

static void SweepRobot_CollisionTestSteerMotorOnDataValidCmp(enum COLLISION_CHAN chan, int *value, u8 *cnt, u8 *flag)
{
    SweepRobot_CollisionTestDataQuery(chan, value);

    if(!(*value)){
        (*cnt)++;
        gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+chan));
    }else{
        gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+chan));
    }

    if((*cnt) > 1){
        *flag = 1;
    }
}

static void SweepRobot_CollisionManualTestFrontDataProc(enum COLLISION_CHAN chan)
{
    if(chan == COLLISION_CHAN_FL || chan == COLLISION_CHAN_R){
        if(!collision[chan].onValidFlag){
            if(!collision[chan].offValidFlag){
                SweepRobot_CollisionTestSteerMotorOffDataValidCmp((enum COLLISION_CHAN)chan, &collision[chan].offValue, &collision[chan].offValidCnt, &collision[chan].offValidFlag);

                if(collision[chan].offValidFlag){
                    if(chan == COLLISION_CHAN_FL){
                        SweepRobot_CollisionCtrlLeftSteerMotorPosMove(STEER_MOTOR_FRONT_POS);
                        OSTimeDlyHMSM(0,0,0,COLLISION_TEST_STEER_MOTOR_MOVE_WAIT_TIME_MS);
                    }else if(chan == COLLISION_CHAN_R){
                        SweepRobot_CollisionCtrlRightSteerMotorPosMove(STEER_MOTOR_SIDE_POS);
                        OSTimeDlyHMSM(0,0,0,COLLISION_TEST_STEER_MOTOR_MOVE_WAIT_TIME_MS);
                    }
                }
            }else{
                SweepRobot_CollisionTestSteerMotorOnDataValidCmp((enum COLLISION_CHAN)chan, &collision[chan].onValue, &collision[chan].onValidCnt, &collision[chan].onValidFlag);

                if(collision[chan].onValidFlag){
                    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                            gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][0],\
                                                            gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][1],\
                                                            SWRB_MANUAL_TEST_OK_BK_COLOR);
                    if(chan == COLLISION_CHAN_FL){
                        SweepRobot_CollisionCtrlLeftSteerMotorPosMove(STEER_MOTOR_IDLE_POS);
                    }else if(chan == COLLISION_CHAN_R){
                        SweepRobot_CollisionCtrlRightSteerMotorPosMove(STEER_MOTOR_IDLE_POS);
                    }
                }

                if( collision[COLLISION_CHAN_FL].onValidFlag && collision[COLLISION_CHAN_R].onValidFlag ){
                    gSwrbTestTaskRunCnt = 0;
                    gSwrbFrontCollisionTestFinishFlag = 1;
                }
            }
        }
    }
}

static void SweepRobot_CollisionManualTestSideDataProc(enum COLLISION_CHAN chan)
{
    if(chan == COLLISION_CHAN_L || chan == COLLISION_CHAN_FR){
        if(!collision[chan].onValidFlag){
            if(!collision[chan].offValidFlag){
                SweepRobot_CollisionTestSteerMotorOffDataValidCmp((enum COLLISION_CHAN)chan, &collision[chan].offValue, &collision[chan].offValidCnt, &collision[chan].offValidFlag);

                if(collision[chan].offValidFlag){
                    if(chan == COLLISION_CHAN_L){
                        SweepRobot_CollisionCtrlLeftSteerMotorPosMove(STEER_MOTOR_SIDE_POS);
                        OSTimeDlyHMSM(0,0,0,COLLISION_TEST_STEER_MOTOR_MOVE_WAIT_TIME_MS);
                        /* TODO: Add Task Resume in UART Rx ISR to decrease waiting time */
                    }else if(chan == COLLISION_CHAN_FR){
                        SweepRobot_CollisionCtrlRightSteerMotorPosMove(STEER_MOTOR_FRONT_POS);
                        OSTimeDlyHMSM(0,0,0,COLLISION_TEST_STEER_MOTOR_MOVE_WAIT_TIME_MS);
                    }
                }
            }else{
                SweepRobot_CollisionTestSteerMotorOnDataValidCmp((enum COLLISION_CHAN)chan, &collision[chan].onValue, &collision[chan].onValidCnt, &collision[chan].onValidFlag);

                if(collision[chan].onValidFlag){
                    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                            gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][0],\
                                                            gSwrbManualTestListviewDispDataCoord[SWRB_MANUAL_TEST_DATA_COLLISION_L_POS+chan][1],\
                                                            SWRB_MANUAL_TEST_OK_BK_COLOR);
                    if(chan == COLLISION_CHAN_L){
                        SweepRobot_CollisionCtrlLeftSteerMotorPosMove(STEER_MOTOR_IDLE_POS);
                    }else if(chan == COLLISION_CHAN_FR){
                        SweepRobot_CollisionCtrlRightSteerMotorPosMove(STEER_MOTOR_IDLE_POS);
                    }
                }
            }
        }
    }
}

static void SweepRobot_CollisionManualTestProc(void)
{
    u8 i;
    char *str;

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        if(!gSwrbFrontCollisionTestFinishFlag){
            SweepRobot_CollisionManualTestFrontDataProc((enum COLLISION_CHAN)i);
        }else{
            SweepRobot_CollisionManualTestSideDataProc((enum COLLISION_CHAN)i);
        }
    }

    if( collision[COLLISION_CHAN_FL].validFlag && collision[COLLISION_CHAN_FR].validFlag && \
        collision[COLLISION_CHAN_L].validFlag && collision[COLLISION_CHAN_R].validFlag \
    ){
        gSwrbTestTaskRunCnt = 0;
        gSwrbFrontCollisionTestFinishFlag = 0;

        SWRB_TestDataSaveToFile(Collision_TestDataSave);

        str = "COLLISION OK\r\n";
        SWRB_TestDataFileWriteString(str);

        SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
    }
}

static void SWRB_CollisionTestProgDisp(void)
{
    Progbar_ManualTest_Set_Percent(gSwrbTestTaskRunCnt, SWRB_COLLISION_TEST_TIME_OUT_CNT);
}

void SweepRobot_CollisionTestTask(void *pdata)
{
    gSwrbFrontCollisionTestFinishFlag = 0;

    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION)){
            SWRB_NextTestTaskResumePreAct(SWRB_COLLISION_TEST_TASK_PRIO);
        }else{
            SWRB_CollisionTestProgDisp();
            gSwrbTestTaskRunCnt++;

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                /* PCB TEST Collision Test Proc */
                if(gSwrbTestTaskRunCnt == 1){
                    SweepRobot_CollisionPCBTestInit();
                }

                if(gSwrbTestTaskRunCnt > 1){
                    SweepRobot_CollisionPCBTestProc();
                }

                if(gSwrbTestTaskRunCnt > SWRB_COLLISION_TEST_TIME_OUT_CNT){
                    SweepRobot_CollisionTestTimeOutProc();
                }
            }else if (gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                /* Manual Test Collision Test Proc */
                if(gSwrbTestTaskRunCnt == 1){
                    SweepRobot_CollisionManualTestInit();
                }

                if(gSwrbTestTaskRunCnt > 1){
                    SweepRobot_CollisionManualTestProc();
                }

                if(gSwrbTestTaskRunCnt > SWRB_COLLISION_TEST_TIME_OUT_CNT){
                    if(gSwrbFrontCollisionTestFinishFlag){
                        SweepRobot_CollisionTestTimeOutProc();
                    }else{
                        gSwrbTestTaskRunCnt = 0;
                        gSwrbFrontCollisionTestFinishFlag = 1;
                    }
                }
            }

            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void Collision_TestDataSave(void)
{
    u8 i;

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_ON_VALUE_POS+i] = collision[i].onValue;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_OFF_VALUE_POS+i] = collision[i].offValue;
    }

    SWRB_TestDataFileWriteData("COLLISION->L_onValue=", collision[COLLISION_CHAN_L].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FL_onValue=", collision[COLLISION_CHAN_FL].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->R_onValue=", collision[COLLISION_CHAN_R].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FR_onValue=", collision[COLLISION_CHAN_FR].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->L_offValue=", collision[COLLISION_CHAN_L].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FL_offValue=", collision[COLLISION_CHAN_FL].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->R_offValue=", collision[COLLISION_CHAN_R].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FR_offValue=", collision[COLLISION_CHAN_FR].offValue, 1);
}
