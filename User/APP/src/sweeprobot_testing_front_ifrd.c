#include "sweeprobot_testing_front_ifrd.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_FRONT_IFRD_CHAN_NUM   8
#define SWRB_FRONT_IFRD_TEST_TIMES_PER_CHAN  10

#define SWRB_FRONT_IFRD_TEST_STEP_MOTOR_MOVE_SPEED  10

enum SWRB_FRONT_IFRD_TEST_POS{
    SWRB_FRONT_IFRD_TEST_L1_POS,
    SWRB_FRONT_IFRD_TEST_L2_POS,
    SWRB_FRONT_IFRD_TEST_L3_POS,
    SWRB_FRONT_IFRD_TEST_L4_POS,
    SWRB_FRONT_IFRD_TEST_R4_POS,
    SWRB_FRONT_IFRD_TEST_R3_POS,
    SWRB_FRONT_IFRD_TEST_R2_POS,
    SWRB_FRONT_IFRD_TEST_R1_POS,
    SWRB_FRONT_IFRD_TEST_POS_BOUND,
};

#define IS_SWRB_FRONT_IFRD_TEST_LEFT_SIDE(cnt)      ( (cnt == SWRB_FRONT_IFRD_TEST_L1_POS) || \
                                                      (cnt == SWRB_FRONT_IFRD_TEST_L2_POS) || \
                                                      (cnt == SWRB_FRONT_IFRD_TEST_L3_POS) || \
                                                      (cnt == SWRB_FRONT_IFRD_TEST_L4_POS)\
                                                    )

static FRONT_IFRD_TestTypeDef frontIFRD[SWRB_FRONT_IFRD_CHAN_NUM];

static const u16 SWRB_FRONT_IFRD_VALID_THRESHOLD[SWRB_FRONT_IFRD_CHAN_NUM] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };

/* Absolute Position */
//static const u16 gSwrbFrontIFRDTestMoveStepSeq[SWRB_FRONT_IFRD_CHAN_NUM] = { 900, 5380, 10350, 14230, 20160, 24100, 28900, 33200 };
/* Relative Position */
static const u16 gSwrbFrontIFRDTestMoveStepSeq[SWRB_FRONT_IFRD_CHAN_NUM] = { 900, 4480, 4970, 3880, 5930, 3940, 4800, 4300 };

static u8 gSwrbFrontIFRDTestStepMotorMoveCnt = 0;
static u8 gSwrbFrontIFRDTestChanCnt = 0;
static u16 gSwrbFrontIFRDTestStateMap = 0;

static void SweepRobot_FrontIFRDTestFinishProc(void);

static void SweepRobot_FrontIFRDTestListviewSetStr(char *str)
{
    Listview_Set_Item_Text(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                            gSwrbManualTestListviewDispDataFrontIFRDCoord[gSwrbFrontIFRDTestChanCnt][0],\
                                                            gSwrbManualTestListviewDispDataFrontIFRDCoord[gSwrbFrontIFRDTestChanCnt][1],\
                                                            str);
}

static void SweepRobot_FrontIFRDTestListviewSetColor(GUI_COLOR color)
{
    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN,\
                                                                   gSwrbManualTestListviewDispDataFrontIFRDCoord[gSwrbFrontIFRDTestChanCnt][0],\
                                                                   gSwrbManualTestListviewDispDataFrontIFRDCoord[gSwrbFrontIFRDTestChanCnt][1],\
                                                                   color);
}

static void SweepRobot_FrontIFRDTestStepMotorISRCB(void)
{
    if( SweepRobotTest_StepMotorModeGet() == STEP_MOTOR_MODE_STOP ){
        OSTaskResume(SWRB_FRONT_IFRD_TEST_TASK_PRIO);
    }
}

static void SweepRobot_FrontIFRDTestFinishStepMotorISRCB(void)
{
    if( SweepRobotTest_StepMotorModeGet() == STEP_MOTOR_MODE_STOP ){
        SweepRobotTest_StepMotorPwrOff();
        STEP_MOTOR_ISR_CB_DEREG();
    }
}

void SweepRobot_FrontIFRDTestStateReset(void)
{
    int i;

    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        mymemset(&frontIFRD[i], 0, sizeof(frontIFRD[i]));
    }

    gSwrbFrontIFRDTestStepMotorMoveCnt = 0;
    gSwrbFrontIFRDTestChanCnt = 0;
    gSwrbFrontIFRDTestStateMap = 0;
}

static void SweepRobot_FrontIFRDTestInit(void)
{
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_FRONT_IFRD_TEST_TASK_PRIO;

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                     gSwrbManualTestListviewDispItemCoord[SWRB_TEST_STATE_IFRD][0], \
                                                     gSwrbManualTestListviewDispItemCoord[SWRB_TEST_STATE_IFRD][1], \
                                                     GUI_LIGHTBLUE);

        Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                     gSwrbManualTestListviewDispItemFrontIFRDCoord[0][0], \
                                                     gSwrbManualTestListviewDispItemFrontIFRDCoord[0][1], \
                                                     GUI_LIGHTRED);
    }

    str = "\r\n>>>FRONT IFRD TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
#endif

    SweepRobotTest_StepMotorPwrOn();
    OSTimeDlyHMSM(0,0,0,500);

    printf("SNSR->IFRD=0\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);

    SweepRobot_FrontIFRDTestStateReset();

    /* XXX: REG StepMotorISRCB, Do not forget to dereg when not use it */
    STEP_MOTOR_ISR_CB_REG(SweepRobot_FrontIFRDTestStepMotorISRCB);

    SweepRobotTest_StepMotorGoHome();
    OSTaskSuspend(OS_PRIO_SELF);
}

static void SweepRobot_FrontIFRDTestStepMotorMoveProc(void)
{
    if(gSwrbFrontIFRDTestStepMotorMoveCnt < SWRB_FRONT_IFRD_CHAN_NUM){
        gSwrbFrontIFRDTestStepMotorMoveCnt++;

        gSwrbFrontIFRDTestChanCnt = gSwrbFrontIFRDTestStepMotorMoveCnt-1;

        SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_BACKWARD);
        SweepRobotTest_StepMotorMoveSteps(SWRB_FRONT_IFRD_TEST_STEP_MOTOR_MOVE_SPEED, gSwrbFrontIFRDTestMoveStepSeq[gSwrbFrontIFRDTestChanCnt]);

        OSTaskSuspend(OS_PRIO_SELF);
    }else{
        gSwrbFrontIFRDTestStepMotorMoveCnt++;
    }
}

static void SweepRobot_FrontIFRDTestTxOffProc(void)
{
    u8 i;
    char *str;

    if(!frontIFRD[gSwrbFrontIFRDTestChanCnt].validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            if(IS_SWRB_FRONT_IFRD_TEST_LEFT_SIDE(gSwrbFrontIFRDTestChanCnt)){
                printf("SNSR->RD=1\r\n");
            }else{
                printf("SNSR->RD=2\r\n");
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                frontIFRD[gSwrbFrontIFRDTestChanCnt].offValue = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    SweepRobot_FrontIFRDTestListviewSetStr(str);
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
    printf("SNSR->IFRD=1\r\n");
}

static void SweepRobot_FrontIFRDTestTxOnProc(void)
{
    u8 i;
    char *str;

    if(!frontIFRD[gSwrbFrontIFRDTestChanCnt].validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            if(IS_SWRB_FRONT_IFRD_TEST_LEFT_SIDE(gSwrbFrontIFRDTestChanCnt)){
                printf("SNSR->RD=1\r\n");
            }else{
                printf("SNSR->RD=2\r\n");
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                frontIFRD[gSwrbFrontIFRDTestChanCnt].onValue = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    SweepRobot_FrontIFRDTestListviewSetStr(str);
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

        if(frontIFRD[gSwrbFrontIFRDTestChanCnt].onValue){
            if( (frontIFRD[gSwrbFrontIFRDTestChanCnt].offValue - frontIFRD[gSwrbFrontIFRDTestChanCnt].onValue) > SWRB_FRONT_IFRD_VALID_THRESHOLD[gSwrbFrontIFRDTestChanCnt] ){
                gSwrbFrontIFRDTestStateMap &= ~(0<<gSwrbFrontIFRDTestChanCnt);
                frontIFRD[gSwrbFrontIFRDTestChanCnt].validCnt++;
            }else{
                gSwrbFrontIFRDTestStateMap |= (1<<gSwrbFrontIFRDTestChanCnt);
                SweepRobot_FrontIFRDTestListviewSetColor(SWRB_MANUAL_TEST_FAULT_BK_COLOR);
            }

            if(frontIFRD[gSwrbFrontIFRDTestChanCnt].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                frontIFRD[gSwrbFrontIFRDTestChanCnt].validFlag = 1;
                SweepRobot_FrontIFRDTestListviewSetColor(SWRB_MANUAL_TEST_OK_BK_COLOR);
            }
        }else{
            gSwrbFrontIFRDTestStateMap |= 1<<(gSwrbFrontIFRDTestChanCnt);
            SweepRobot_FrontIFRDTestListviewSetColor(SWRB_MANUAL_TEST_FAULT_BK_COLOR);
        }
    }
    printf("SNSR->IFRD=0\r\n");
}

static void SweepRobot_FrontIFRDTestFinishProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    gSwrbFrontIFRDTestStepMotorMoveCnt = 0;
    gSwrbFrontIFRDTestChanCnt = 0;
    printf("SNSR->IFRD=0\r\n");

    STEP_MOTOR_ISR_CB_REG(SweepRobot_FrontIFRDTestFinishStepMotorISRCB);

    SweepRobotTest_StepMotorGoHome();

    Listview_Set_Item_BkColor(hWin_SWRB_MANUAL, ID_MANUAL_LISTVIEW_MAIN, \
                                                     gSwrbManualTestListviewDispItemFrontIFRDCoord[0][0], \
                                                     gSwrbManualTestListviewDispItemFrontIFRDCoord[0][1], \
                                                     GUI_LIGHTBLUE);

    SWRB_TestDataSaveToFile(FrontIFRD_TestDataSave);

//    SWRB_WM_EnableWindow(hWin_SWRB_MANUAL, ID_MANUAL_BUTTON_RESET);

    OSTaskResume(SWRB_IFRD_TEST_TASK_PRIO);
    OSTaskSuspend(OS_PRIO_SELF);
}

static void SWRB_FrontIFRDTestProgDisp(void)
{
    Progbar_ManualTest_Set_Percent(gSwrbFrontIFRDTestStepMotorMoveCnt, SWRB_FRONT_IFRD_CHAN_NUM);
}

void SweepRobot_FrontIFRDTestTask(void *pdata)
{
    while(1){
//        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD)){
//            SWRB_NextTestTaskResumePreAct(SWRB_IFRD_TEST_TASK_PRIO);
//        }else{
        
            SWRB_FrontIFRDTestProgDisp();
            if(gSwrbTestTaskRunCnt == 0){
                SweepRobot_FrontIFRDTestInit();
            }

            if(( gSwrbTestTaskRunCnt % (SWRB_FRONT_IFRD_TEST_TIMES_PER_CHAN<<1) ) == 0){
                SweepRobot_FrontIFRDTestStepMotorMoveProc();
            }

            if(gSwrbFrontIFRDTestStepMotorMoveCnt <= SWRB_FRONT_IFRD_CHAN_NUM){
                if((gSwrbTestTaskRunCnt+1)%2){
                    SweepRobot_FrontIFRDTestTxOffProc();
                }else{
                    SweepRobot_FrontIFRDTestTxOnProc();
                }

                if(gSwrbFrontIFRDTestStepMotorMoveCnt != 0)
                    gSwrbTestTaskRunCnt++;
            }else{
               SweepRobot_FrontIFRDTestFinishProc();
            }

            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
//        }
    }
}

void FrontIFRD_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("FrontIFRD->L1_onValue=", frontIFRD[0].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L1_offValue=", frontIFRD[0].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L2_onValue=", frontIFRD[1].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L2_offValue=", frontIFRD[1].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L3_onValue=", frontIFRD[2].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L3_offValue=", frontIFRD[2].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L4_onValue=", frontIFRD[3].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->L4_offValue=", frontIFRD[3].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R4_onValue=", frontIFRD[4].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R4_offValue=", frontIFRD[4].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R3_onValue=", frontIFRD[5].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R3_offValue=", frontIFRD[5].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R2_onValue=", frontIFRD[6].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R2_offValue=", frontIFRD[6].offValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R1_onValue=", frontIFRD[7].onValue, 1);
    SWRB_TestDataFileWriteData("FrontIFRD->R1_offValue=", frontIFRD[7].offValue, 1);
}

void SweepRobot_FrontIFRDTestStartProc(void)
{
    OSTaskResume(SWRB_FRONT_IFRD_TEST_TASK_PRIO);
    OSTaskSuspend(OS_PRIO_SELF);
}
