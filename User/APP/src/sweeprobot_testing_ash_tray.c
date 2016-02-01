#include "sweeprobot_testing_ash_tray.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#ifdef _ASH_TRAY_USE_MINUS_COMPARE
    const static int SWRB_ASH_TRAY_LVL_VALID_MINUS_THRESHOLD = 400;
#else
    const static int SWRB_ASH_TRAY_LVL_VALID_VALUE_THRESHOLD = 3000;
#endif

static ASH_TRAY_TestTypeDef ashTrayIns;
static ASH_TRAY_TestTypeDef ashTrayLvl;

static void SweepRobot_AshTrayTestInit(void)
{
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_ASH_TRAY_TEST_TASK_PRIO;

    str = "\r\n>>>ASH TRAY TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    mymemset(&ashTrayIns, 0, sizeof(ashTrayIns));
    mymemset(&ashTrayLvl, 0, sizeof(ashTrayLvl));

    printf("SNSR->IFRD=0\r\n");

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_AshTrayInsTestProc(void)
{
    u8 i;
    char *str;

    if(!ashTrayIns.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("AT->RD=0\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayIns.value = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][1],\
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
        if(!ashTrayIns.value){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_ASH_TRAY_INS_POS);
            ashTrayIns.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_ASH_TRAY_INS_POS);
        }
        if(ashTrayIns.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            ashTrayIns.validFlag = 1;
        }

        if(ashTrayIns.validFlag){
            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][1],\
                                                           SWRB_MANUL_TEST_OK_BK_COLOR);
            }
        }
    }
}

#ifdef _ASH_TRAY_USE_MINUS_COMPARE
static void SweepRobot_AshTrayLvlTestTxOffProc(void)
{
    u8 i;
    char *str;

    if(!ashTrayLvl.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SNSR->RD=14\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayLvl.offValue = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U2, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][1],\
                                            str);
                    myfree(SRAMIN, str);
                }
                usartRxFlag=0;
                usartRxNum=0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        printf("SNSR->IFRD=1\r\n");
    }
}
#endif

static void SweepRobot_AshTrayLvlTestTxOnProc(void)
{
    u8 i;
    char *str;

    if(!ashTrayLvl.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SNSR->RD=14\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayLvl.onValue = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D2, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][1],\
                                            str);
                    myfree(SRAMIN, str);
                }
                usartRxFlag = 0;
                usartRxNum = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        printf("SNSR->IFRD=0\r\n");

#ifdef _ASH_TRAY_USE_MINUS_COMPARE
        if(ashTrayLvl.offValue - ashTrayLvl.onValue > SWRB_ASH_TRAY_LVL_VALID_MINUS_THRESHOLD){
#else
        if( (0 < ashTrayLvl.onValue) && (ashTrayLvl.onValue < SWRB_ASH_TRAY_LVL_VALID_VALUE_THRESHOLD) ){
#endif
            gSwrbTestStateMap &= ~( (u32)1<<SWRB_TEST_ASH_TRAY_LVL_POS);
            ashTrayLvl.validCnt++;
        }else{
            gSwrbTestStateMap |= ( (u32)1<<SWRB_TEST_ASH_TRAY_LVL_POS);
        }

        if(ashTrayLvl.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            ashTrayLvl.validFlag = 1;

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][1],\
                                                           SWRB_MANUL_TEST_OK_BK_COLOR);
            }
        }
    }
}

static void SweepRobot_AshTrayTestProc(void)
{
    char *str;

    if(gSwrbTestTaskRunCnt > 1){
        SweepRobot_AshTrayInsTestProc();
    }
#ifdef _ASH_TRAY_USE_MINUS_COMPARE
    if(gSwrbTestTaskRunCnt%2){
        SweepRobot_AshTrayLvlTestTxOffProc();
    }else{
        SweepRobot_AshTrayLvlTestTxOnProc();
    }
#else
    if(gSwrbTestTaskRunCnt > 1){
        SweepRobot_AshTrayLvlTestTxOnProc();
    }
#endif

    if(ashTrayIns.validFlag && ashTrayLvl.validFlag){
        gSwrbTestTaskRunCnt = 0;
        printf("SNSR->IFRD=0\r\n");

        SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "ASH TRAY OK\r\n";
            SWRB_TestDataFileWriteString(str);

    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_ASH_TRAY, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_ASH_TRAY, "ASH TRAY OK");
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_AshTrayPCBTestTimeOutProc(void)
{
    char *str;

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK){
        str = "ERROR->ASH TRAY INS\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK){
        str = "ERROR->ASH TRAY LVL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_ASH_TRAY, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_ASH_TRAY, "ASH TRAY ERROR");
    Edit_Clear();
}

static void SweepRobot_AshTrayManulTestTimeOutProc(void)
{
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_INS_POS][1],\
                                                           SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_ASH_TRAY_LVL_POS][1],\
                                                           SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
}

static void SweepRobot_AshTrayTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("SNSR->IFRD=0\r\n");

    SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_AshTrayPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_AshTrayManulTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
#endif
}

void SweepRobot_AshTrayTestTask(void *pdata)
{

    SweepRobot_AshTrayTestGPIOInit();

    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_ASH_TRAY)){
            SWRB_NextTestTaskResumePreAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_AshTrayTestInit();
            }

            SweepRobot_AshTrayTestProc();

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_AshTrayTestTimeOutProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void ASH_TRAY_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
    SWRB_TestDataFileWriteData("ASH_TRAY->INS_Value=", ashTrayIns.value, 1);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_onValue=", ashTrayLvl.onValue, 1);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_offValue=", ashTrayLvl.offValue, 1);
}
