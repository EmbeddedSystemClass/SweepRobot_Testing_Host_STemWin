#include "sweeprobot_testing_irda.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "swrbTestDriver.h"
#include "stm32f4xx_it.h"

#include "usart.h"
#include "includes.h"

#define SWRB_IRDA_TEST_USART_READ_WAIT_TIME     10

#define SWRB_IRDA_TEST_TASK_DLY_TIME    100

static IRDA_TestTypeDef IrDA[SWRB_IRDA_CHAN_BOUND];

static char aIrDATestRxData[SWRB_IRDA_CHAN_BOUND][5] = { 0 };

static void SweepRobot_IrDATestRxCodeProc(int rxDataLen)
{
    int i,j,m;
    OS_CPU_SR cpu_sr;
    
    OS_ENTER_CRITICAL();

    i = 0;
    j = 0;
RX_PROC_LOOP:
    if(i<=SWRB_IRDA_CHAN_BOUND){
        for(m=0;j<=rxDataLen;j++,m++){
            if(USART_RX_BUF[j] != ','){
                aIrDATestRxData[i][m] = USART_RX_BUF[j];
            }else{
                i++;
                j++;
                goto RX_PROC_LOOP;
            }
        }
    }

    OS_EXIT_CRITICAL();
}

static void SweepRobot_IrDATestCodeArrayToNum(void)
{
    int i;
    char *str;

    for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
        if(!IrDA[i].validFlag){
            str = mymalloc(SRAMIN, sizeof(char)*5);
            *str = 0;
            mymemcpy(str, aIrDATestRxData[i], sizeof(aIrDATestRxData[i]));
            IrDA[i].code = atoi(str);
            Edit_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, str);
//            Edit_Set_HexMode(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, 0, 0, 255);
//            Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, IrDA[i].code);
            myfree(SRAMIN, str);
        }
    }
}

static void SweepRobot_IrDACodeQuery(void)
{
//    SweepRobot_IrDACodeTxProc(0x40);
//    OSTimeDlyHMSM(0,0,0,SWRB_IRDA_TEST_USART_READ_WAIT_TIME);
    printf("IRDA->RD\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_IRDA_TEST_USART_READ_WAIT_TIME);
    if(usartRxFlag){
        SweepRobot_IrDATestRxCodeProc(USART_RX_STA&USART_CNT_MASK);
        SweepRobot_IrDATestCodeArrayToNum();
        usartRxFlag = 0;
        USART_RX_STA = 0;
    }
}

static void SweepRobot_IrDATestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_IRDA_TEST_TASK_PRIO;
    
    str = "\r\n>>>IRDA TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef __SHOW_TEST_TITLE    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    printf("IRDA->ON\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
    
    for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
        IrDA[i].code = 0;
        IrDA[i].validCnt = 0;
        IrDA[i].validFlag = 0;
        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, 0);
    }
    mymemset(USART_RX_BUF, 0, sizeof(char)*USART_REC_LEN);
}

static void SweepRobot_IrDATestProc(void)
{
    u8 i,j;
    char *str;
    
    SweepRobot_IrDACodeQuery();
    
    for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
        if(!IrDA[i].validFlag){
            if(IS_IRDA_CODE(IrDA[i].code)){
                IrDA[i].validCnt++;
            }else{
                IrDA[i].validCnt = 0;
            }
            if(IrDA[i].validCnt){
                IrDA[i].validFlag = 1;
            }
            if(IrDA[i].validFlag){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_IRDA_B_POS+i));
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_IRDA_B_POS+i));
            }
            IrDA[i].code = 0;
            for(j=0;j<5;j++)
                aIrDATestRxData[i][j] = 0;
        }
    }
    mymemset(USART_RX_BUF, 0, sizeof(char)*USART_REC_LEN);
    
    if(IrDA[0].validFlag && IrDA[1].validFlag && IrDA[2].validFlag && IrDA[3].validFlag && IrDA[4].validFlag){
        gSwrbTestTaskRunCnt = 0;
        
//        printf("IRDA->OFF\r\n");
//        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        printf("IRDA->ERS\r\n");
        OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
        
        SWRB_TestDataSaveToFile(IRDA_TestDataSave);
        
        str = "IRDA OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
//        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IRDA, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IRDA, "IRDA OK");
//        for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
//            Edit_Set_DecMode(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, 0, 0, 65536, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
//        }
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_IRDA_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IrDATestOverTimeProc(void)
{
    u8 i;
    char *str;
    
    gSwrbTestTaskRunCnt = 0;

//    printf("IRDA->OFF\r\n");
//    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
    printf("IRDA->ERS\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);

    SWRB_TestDataSaveToFile(IRDA_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_B_MSAK){
        str = "ERROR->IRDA_B\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_L_MSAK){
        str = "ERROR->IRDA_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FL_MSAK){
        str = "ERROR->IRDA_FL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FR_MSAK){
        str = "ERROR->IRDA_FR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_R_MSAK){
        str = "ERROR->IRDA_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IRDA, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IRDA, "IRDA ERROR");
    for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
        Edit_Set_DecMode(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, 0, 0, 65536, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
    }
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_IRDA_TEST_TASK_PRIO);
#endif
}

void SweepRobot_IrDATestTask(void *pdata)
{
    
    SweepRobot_IrDATestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_IRDA)){
            SWRB_NextTestTaskResumePreAct(SWRB_IRDA_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_IrDATestInit();
            }

            if(gSwrbTestTaskRunCnt > 2){
                SweepRobot_IrDATestProc();
            }

            if(gSwrbTestTaskRunCnt > 5){
                SweepRobot_IrDATestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_IRDA_TEST_TASK_DLY_TIME);
        }
    }
}

void IRDA_TestDataSave(void)
{
    u8 i;
    
    for(i=0;i<SWRB_IRDA_CHAN_BOUND;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IRDA_B_RxCODE_POS+i] = IrDA[i].code;
    }
    
    SWRB_TestDataFileWriteData("IRDA->B_Code=", IrDA[SWRB_IRDA_CHAN_BACK].code, 1);
    SWRB_TestDataFileWriteData("IRDA->L_Code=", IrDA[SWRB_IRDA_CHAN_LEFT].code, 1);
    SWRB_TestDataFileWriteData("IRDA->FL_Code=", IrDA[SWRB_IRDA_CHAN_FLEFT].code, 1);
    SWRB_TestDataFileWriteData("IRDA->FR_Code=", IrDA[SWRB_IRDA_CHAN_FRIGHT].code, 1);
    SWRB_TestDataFileWriteData("IRDA->R_Code=", IrDA[SWRB_IRDA_CHAN_RIGHT].code, 1);
}