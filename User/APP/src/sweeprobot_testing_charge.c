#include "sweeprobot_testing_charge.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static u8 swrbChargeTestStateMap = 0;

#define SWRB_TEST_CHARGE_CUR_POS                0
#define SWRB_TEST_CHARGE_VOL_POS                1
#define SWRB_TEST_CHARGE_24V_POS                2

#define SWRB_TEST_FAULT_CHARGE_CUR_MASK         0x01
#define SWRB_TEST_FAULT_CHARGE_VOL_MASK         0x02
#define SWRB_TEST_FAULT_CHARGE_24V_MASK         0x04

#define SWRB_TEST_CHARGE_CUR_HIGH_THRESHOLD     100
#define SWRB_TEST_CHARGE_CUR_LOW_THRESHOLD      20

#define SWRB_TEST_CHARGE_VOL_HIGH_THRESHOLD     4000
#define SWRB_TEST_CHARGE_VOL_LOW_THRESHOLD      2000

static CHARGE_TestTypeDef charge;

static void SweepRobot_ChargeTestInit(void)
{
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_CHARGE_TEST_TASK_PRIO;

    str = "\r\n>>>CHARGE TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);

    mymemset(&charge, 0, sizeof(charge));

#ifdef _USE_CHARGE_24V
    SweepRobot_IrDATestTxRelayOn();
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
    SweepRobot_Charge24VOn();
#endif
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
    printf("CRG->ON=0\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_ChargeTestProc(void)
{
    u8 i;
    char *str;

    if(!charge.curValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CRG->RD=1\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                charge.current = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][1],\
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

        if( SWRB_TEST_CHARGE_CUR_LOW_THRESHOLD<charge.current && SWRB_TEST_CHARGE_CUR_HIGH_THRESHOLD>charge.current ){
            swrbChargeTestStateMap &= ~(1<<SWRB_TEST_CHARGE_CUR_POS);
            charge.curValidCnt++;
        }else{
            swrbChargeTestStateMap |= (1<<SWRB_TEST_CHARGE_CUR_POS);
        }

        if(charge.curValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.curValidFlag = 1;

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][1],\
                                                           GUI_LIGHTBLUE);
            }
        }
    }

    if(!charge.volValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CRG->RD=0\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                charge.voltage = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U2, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][1],\
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

        if(SWRB_TEST_CHARGE_VOL_LOW_THRESHOLD < charge.voltage && SWRB_TEST_CHARGE_VOL_HIGH_THRESHOLD > charge.voltage){
            swrbChargeTestStateMap &= ~(1<<SWRB_TEST_CHARGE_VOL_POS);
            charge.volValidCnt++;
        }else{
            swrbChargeTestStateMap |= (1<<SWRB_TEST_CHARGE_VOL_POS);
        }

        if(charge.volValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.volValidFlag = 1;

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][1],\
                                                           SWRB_MANUL_TEST_OK_BK_COLOR);
            }
        }
    }

    if(!charge.charge24vValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CRG->RD=2\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                charge.charge24vState = usartRxNum;
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U3, usartRxNum);
                }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    str = mymalloc(SRAMIN, sizeof(char)*10);
                    *str = 0;
                    sprintf(str, "%d", usartRxNum);
                    Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][0],\
                                            gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][1],\
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

        if(charge.charge24vState){
            swrbChargeTestStateMap &= ~(1<<SWRB_TEST_CHARGE_24V_POS);
            charge.charge24vValidCnt++;
        }else{
            swrbChargeTestStateMap |= (1<<SWRB_TEST_CHARGE_24V_POS);
        }

        if(charge.charge24vValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.charge24vValidFlag = 1;

            if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][1],\
                                                           SWRB_MANUL_TEST_OK_BK_COLOR);
            }
        }
    }

    if(charge.curValidFlag && charge.volValidFlag && charge.charge24vValidFlag){
        gSwrbTestTaskRunCnt = 0;
        printf("CRG->OFF\r\n");
        SweepRobot_Charge24VOff();
        SweepRobot_IrDATestTxRelayOff();

        SWRB_TestDataSaveToFile(CHARGE_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "CHARGE OK\r\n";
            SWRB_TestDataFileWriteString(str);

//            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_CHARGE, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_CHARGE, "CHARGE OK");
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_CHARGE_TEST_TASK_PRIO);
    }
}

static void SweepRobot_ChargePCBTestTimeOutProc(void)
{
    char *str;

    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_CUR_MASK){
        str = "ERROR->CHARGE CUR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_VOL_MASK){
        str = "ERROR->CHARGE VOL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_24V_MASK){
        str = "ERROR->CHARGE 24V\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_CHARGE, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_CHARGE, "CHARGE ERROR");
    Edit_Clear();
}

static void SweepRobot_ChargeManulTestTimeOutProc(void)
{
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_CUR_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_CUR_POS][1],\
                                                           SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_VOL_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_VOL_POS][1],\
                                                           SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_24V_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][0],\
                                                           gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_CHARGE_24V_POS][1],\
                                                           SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
}

static void SweepRobot_ChargeTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("CRG->OFF\r\n");
    SweepRobot_Charge24VOff();
    SweepRobot_IrDATestTxRelayOff();

    SWRB_TestDataSaveToFile(CHARGE_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_ChargePCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_ChargeManulTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_CHARGE_TEST_TASK_PRIO);
#endif
}

void SweepRobot_ChargeTestTask(void *pdata)
{
    u16 TimeOutWaitTime;

    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_CHARGE)){
            SWRB_NextTestTaskResumePreAct(SWRB_CHARGE_TEST_TASK_PRIO);
        }else{

            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_ChargeTestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_ChargeTestProc();
            }

            if(charge.charge24vState){
                TimeOutWaitTime = 600;
            }else{
                TimeOutWaitTime = 100;
            }

            if(gSwrbTestTaskRunCnt > TimeOutWaitTime){
                SweepRobot_ChargeTestTimeOutProc();
            }

            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void CHARGE_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_CUR_POS] = charge.current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_VOL_POS] = charge.voltage;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_24V_POS] = charge.charge24vState;

    SWRB_TestDataFileWriteData("CHARGE->Vol=", charge.voltage, 1);
    SWRB_TestDataFileWriteData("CHARGE->Cur=", charge.current, 1);
    SWRB_TestDataFileWriteData("CHARGE->24V=", charge.charge24vState, 1);
}
