#include "sweeprobot_testing_front_ifrd.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static FRONT_IFRD_TestTypeDef frontIFRD[SWRB_FRONT_IFRD_CHAN_NUM];

static const u16 SWRB_FRONT_IFRD_VALID_THRESHOLD[SWRB_FRONT_IFRD_CHAN_NUM] = { 800, 800, 800, 800, 800, 800, 800, 800 };

static void SweepRobot_FrontIFRDTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_IFRD_TEST_TASK_PRIO;

    str = "\r\n>>>IFRD TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef __SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    printf("SNSR->IFRD=0\r\n");
    printf("SNSR->BSWC=0\r\n");

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);

    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        frontIFRD[i].offValue = 0;
        frontIFRD[i].onValue = 0;
        frontIFRD[i].validCnt = 0;
        frontIFRD[i].validFlag = 0;
    }
}

static void SweepRobot_IFRDTestTxOffProc(void)
{
    u8 i,j;

    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!frontIFRD[i].validFlag){
            if(i==6){
                printf("SNSR->BSWC=1\r\n");
                OSTimeDlyHMSM(0,0,0,1);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SNSR->RD=%d\r\n", i+1);
                }else{
                    printf("SNSR->RD=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    frontIFRD[i].offValue = usartRxNum;
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
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
    printf("SNSR->BSWC=0\r\n");
    printf("SNSR->IFRD=1\r\n");
}

static void SweepRobot_IFRDTestTxOnProc(void)
{
    u8 i,j;
    char *str;

    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!frontIFRD[i].validFlag){
            if(i==6){
                printf("SNSR->BSWC=1\r\n");
                OSTimeDlyHMSM(0,0,0,1);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SNSR->RD=%d\r\n", i+1);
                }else{
                    printf("SNSR->RD=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    frontIFRD[i].onValue = usartRxNum;
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    USART_RX_STA = 0;
                    break;
                }else{
                    continue;
                }
            }

            if(frontIFRD[i].onValue){
                if( (frontIFRD[i].offValue - frontIFRD[i].onValue) > SWRB_FRONT_IFRD_VALID_THRESHOLD[i] ){
                    gSwrbTestStateMap &= ~(0<<(SWRB_TEST_IFRD_FL_POS+i));
                    frontIFRD[i].validCnt++;
                }else{
                    gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
                }

                if(frontIFRD[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                    frontIFRD[i].validFlag = 1;
                }
            }else{
                gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
            }
        }
    }
    printf("SNSR->BSWC=0\r\n");
    printf("SNSR->IFRD=0\r\n");

    if( frontIFRD[0].validFlag && frontIFRD[1].validFlag && frontIFRD[2].validFlag && frontIFRD[3].validFlag && \
        frontIFRD[4].validFlag && frontIFRD[5].validFlag && frontIFRD[6].validFlag && frontIFRD[7].validFlag
        ){
        gSwrbTestTaskRunCnt = 0;
        printf("SNSR->IFRD=0\r\n");
        printf("SNSR->BSWC=0\r\n");

        SWRB_TestDataSaveToFile(IFRD_TestDataSave);

        str = "IFRD OK\r\n";
        SWRB_TestDataFileWriteString(str);
//        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IFRD, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, "IFRD OK");
        Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IFRDTestOverTimeErrProc(char *str)
{
    SWRB_TestDataFileWriteString(str);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
}

static void SweepRobot_IFRDTestOverTimeProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("SNSR->IFRD=0\r\n");
    printf("SNSR->BSWC=0\r\n");

    SWRB_TestDataSaveToFile(IFRD_TestDataSave);

    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FL_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_F_L\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FR_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_F_R\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_L_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_S_L\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_R_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_S_R\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FL_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_B_FL\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FR_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_B_FR\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SL_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_B_SL\r\n");
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SR_MASK){
        SweepRobot_IFRDTestOverTimeErrProc("ERROR->IFRD_B_SR\r\n");
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_IFRD, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, "IFRD ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
#endif
}

void SweepRobot_FrontIFRDTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_IFRD)){
            SWRB_NextTestTaskResumePreAct(SWRB_IFRD_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_IFRD, GUI_GREEN, CHECKBOX_CI_ENABLED);

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_FrontIFRDTestInit();
            }

            if(gSwrbTestTaskRunCnt%2){
                SweepRobot_IFRDTestTxOffProc();
            }else{
                SweepRobot_IFRDTestTxOnProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_IFRDTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void FrontIFRD_TestDataSave(void)
{
    u8 i;

    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOn_POS+i] = frontIFRD[i].onValue;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOff_POS+i] = frontIFRD[i].offValue;
    }

    SWRB_TestDataFileWriteData("IFRD->FL_onValue=", frontIFRD[0].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FL_offValue=", frontIFRD[0].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FR_onValue=", frontIFRD[1].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->FR_offValue=", frontIFRD[1].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SL_onValue=", frontIFRD[2].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SL_offValue=", frontIFRD[2].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SR_onValue=", frontIFRD[3].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->SR_offValue=", frontIFRD[3].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FL_onValue=", frontIFRD[4].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FL_offValue=", frontIFRD[4].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FR_onValue=", frontIFRD[5].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_FR_offValue=", frontIFRD[5].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SL_onValue=", frontIFRD[6].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SL_offValue=", frontIFRD[6].offValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SR_onValue=", frontIFRD[7].onValue, 1);
    SWRB_TestDataFileWriteData("IFRD->B_SR_offValue=", frontIFRD[7].offValue, 1);
}
