#include "sweeprobot_testing_ifrd.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static const u16 SWRB_IFRD_VALID_THRESHOLD[SWRB_IFRD_CHAN_NUM] = { 1000, 1000, 500, 500, 200, 200, 200, 200 };

static IFRD_TestTypeDef ifrd[SWRB_IFRD_CHAN_NUM];

static void SweepRobot_IFRDTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_IFRD_TEST_TASK_PRIO;
    
    str = "\r\n>>>IFRD TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        mymemset(&ifrd[i], 0, sizeof(ifrd[i]));
    }
    
    printf("SNSR->IFRD=0\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("SNSR->BSWC=0\r\n");

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_IFRDTestTxOffProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!ifrd[i].validFlag){
            if(i>=6){
                printf("SNSR->BSWC=1\r\n");
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SNSR->RD=%d\r\n", i+1);
                }else{
                    printf("SNSR->RD=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    ifrd[i].offValue = usartRxNum;
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
                    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        str = mymalloc(SRAMIN, sizeof(char)*10);
                        *str = 0;
                        sprintf(str, "%d", usartRxNum);
                        Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][0],\
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][1],\
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
    }
    printf("SNSR->IFRD=1\r\n");
}

static void SweepRobot_IFRDTestTxOnProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!ifrd[i].validFlag){
            if(i>=6){
                printf("SNSR->BSWC=1\r\n");
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SNSR->RD=%d\r\n", i+1);
                }else{
                    printf("SNSR->RD=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    ifrd[i].onValue = usartRxNum;
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1+i, usartRxNum);
                    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        str = mymalloc(SRAMIN, sizeof(char)*10);
                        *str = 0;
                        sprintf(str, "%d", usartRxNum);
                        Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][0],\
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][1],\
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
            
            /* TODO: Add max ifrd minus value display in Manul Test Auto Mode */
            if(ifrd[i].onValue){
                if( (ifrd[i].offValue - ifrd[i].onValue) > SWRB_IFRD_VALID_THRESHOLD[i] ){
                    gSwrbTestStateMap &= ~(0<<(SWRB_TEST_IFRD_FL_POS+i));
                    ifrd[i].validCnt++;
                }else{
                    gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
                }
                
                if(ifrd[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                    ifrd[i].validFlag = 1;
                    
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][0],\
                                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][1],\
                                                                   GUI_LIGHTBLUE);
                    }
                }
            }else{
                gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
            }
        }
    }
    printf("SNSR->IFRD=0\r\n");

    if( ifrd[0].validFlag && ifrd[1].validFlag && ifrd[2].validFlag && ifrd[3].validFlag && \
        ifrd[4].validFlag && ifrd[5].validFlag && ifrd[6].validFlag && ifrd[7].validFlag
        ){
        gSwrbTestTaskRunCnt = 0;
        printf("SNSR->IFRD=0\r\n");
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
        printf("SNSR->BSWC=0\r\n");
        
        SWRB_TestDataSaveToFile(IFRD_TestDataSave);
        
        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "IFRD OK\r\n";
            SWRB_TestDataFileWriteString(str);
//            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IFRD, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, "IFRD OK");
            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
            Edit_Clear();
        }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
            /* TODO: Add FrontIFRDTest Enable and Disable function here */
            SweepRobot_FrontIFRDTestStartProc();
        }
        
        SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IFRDTestTimeOutErrProc(char *str)
{
    SWRB_TestDataFileWriteString(str);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
}

static void SweepRobot_IFRDPCBTestTimeOutProc(void)
{
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FL_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_F_L\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FR_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_F_R\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_L_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_S_L\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_R_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_S_R\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FL_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_B_FL\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FR_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_B_FR\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SL_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_B_SL\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SR_MASK){
        SweepRobot_IFRDTestTimeOutErrProc("ERROR->IFRD_B_SR\r\n");
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IFRD, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, "IFRD ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();
}

static void SweepRobot_IFRDManulTestTimeOutProc(void)
{
    u8 i;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(gSwrbTestStateMap & (SWRB_TEST_FAULT_IFRD_FL_MASK<<i)){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][0],\
                                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_IFRD_FL_POS+i][1],\
                                                                   GUI_LIGHTRED);
        }
    }
}

static void SweepRobot_IFRDTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("SNSR->IFRD=0\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("SNSR->BSWC=0\r\n");

    SWRB_TestDataSaveToFile(IFRD_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SweepRobot_IFRDPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SweepRobot_IFRDManulTestTimeOutProc();
        
        SweepRobot_FrontIFRDTestStartProc();
    }
    
#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
#endif
}

void SweepRobot_IFRDTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD)){
            SWRB_NextTestTaskResumePreAct(SWRB_IFRD_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_IFRDTestInit();
            }

            if(gSwrbTestTaskRunCnt%2){
                SweepRobot_IFRDTestTxOffProc();
            }else{
                SweepRobot_IFRDTestTxOnProc();
            }
            
            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_IFRDTestTimeOutProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void IFRD_TestDataSave(void)
{
    u8 i;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOn_POS+i] = ifrd[i].onValue;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOff_POS+i] = ifrd[i].offValue;
    }
    
    SWRB_TestDataFileWriteData("IFRD->FL_onValue=", ifrd[0].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FL_offValue=", ifrd[0].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FR_onValue=", ifrd[1].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FR_offValue=", ifrd[1].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SL_onValue=", ifrd[2].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SL_offValue=", ifrd[2].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SR_onValue=", ifrd[3].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SR_offValue=", ifrd[3].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FL_onValue=", ifrd[4].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FL_offValue=", ifrd[4].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FR_onValue=", ifrd[5].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FR_offValue=", ifrd[5].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SL_onValue=", ifrd[6].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SL_offValue=", ifrd[6].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SR_onValue=", ifrd[7].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SR_offValue=", ifrd[7].offValue, 1);
}
