#include "sweeprobot_testing_ifrd.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_IFRD_CHAN_NUM   8

const static u16 SWRB_IFRD_VALID_THRESHOLD[SWRB_IFRD_CHAN_NUM] = { 800, 800, 250, 250, 150, 150, 150, 150 };

static IFRD_TestTypeDef ifrd[SWRB_IFRD_CHAN_NUM];

static void SweepRobot_IFRDTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_IFRD_TEST_TASK_PRIO;
    
    str = "\r\n>>>IFRD TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    
    printf("SENSOR->IFRD_LED=0\r\n");
    printf("SENSOR->B_SWITCH=0\r\n");
    
    OSTimeDlyHMSM(0,0,1,0);
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        ifrd[i].offValue = 0;
        ifrd[i].onValue = 0;
        ifrd[i].validCnt = 0;
        ifrd[i].validFlag = 0;
    }
}

static void SweepRobot_IFRDTestTxOffProc(void)
{
    u8 i,j;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!ifrd[i].validFlag){
            if(i==6){
                printf("SENSOR->B_SWITCH=1\r\n");
                OSTimeDlyHMSM(0,0,0,1);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SENSOR->READ=%d\r\n", i+1);
                }else{
                    printf("SENSOR->READ=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    ifrd[i].offValue = usartRxNum;
                    Edit_Set_Value(ID_MAIN_EDIT_U1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
        }
    }
    printf("SENSOR->B_SWITCH=0\r\n");
    printf("SENSOR->IFRD_LED=1\r\n");
}

static void SweepRobot_IFRDTestTxOnProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
        if(!ifrd[i].validFlag){
            if(i==6){
                printf("SENSOR->B_SWITCH=1\r\n");
                OSTimeDlyHMSM(0,0,0,1);
            }
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                if(i<6){
                    printf("SENSOR->READ=%d\r\n", i+1);
                }else{
                    printf("SENSOR->READ=%d\r\n", i-1);
                }
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    ifrd[i].onValue = usartRxNum;
                    Edit_Set_Value(ID_MAIN_EDIT_D1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
            
            if(ifrd[i].onValue){
                if( (ifrd[i].offValue - ifrd[i].onValue) > SWRB_IFRD_VALID_THRESHOLD[i] ){
                    gSwrbTestStateMap &= ~(0<<(SWRB_TEST_IFRD_FL_POS+i));
                    ifrd[i].validCnt++;
                }else{
                    gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
                }
                
                if(ifrd[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                    ifrd[i].validFlag = 1;
                }
            }else{
                gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_FL_POS+i);
            }
        }
    }
    printf("SENSOR->B_SWITCH=0\r\n");
    printf("SENSOR->IFRD_LED=0\r\n");

    if( ifrd[0].validFlag && ifrd[1].validFlag && ifrd[2].validFlag && ifrd[3].validFlag && \
        ifrd[4].validFlag && ifrd[5].validFlag && ifrd[6].validFlag && ifrd[7].validFlag
        ){
        gSwrbTestTaskRunCnt = 0;
        printf("SENSOR->IFRD_LED=0\r\n");
        printf("SENSOR->B_SWITCH=0\r\n");
        
        SWRB_TestDataSaveToFile(IFRD_TestDataSave);
        
        str = "IFRD OK\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_IFRD, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IFRD, "IFRD OK");
        Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IFRDTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("SENSOR->IFRD_LED=0\r\n");
    printf("SENSOR->B_SWITCH=0\r\n");

    SWRB_TestDataSaveToFile(IFRD_TestDataSave);

    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FL_MASK){
        str = "ERROR->IFRD_F_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_FR_MASK){
        str = "ERROR->IFRD_F_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_L_MASK){
        str = "ERROR->IFRD_S_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_R_MASK){
        str = "ERROR->IFRD_S_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FL_MASK){
        str = "ERROR->IFRD_B_FL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FR_MASK){
        str = "ERROR->IFRD_B_FR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SL_MASK){
        str = "ERROR->IFRD_B_SL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SR_MASK){
        str = "ERROR->IFRD_B_SR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_IFRD, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IFRD, "IFRD ERROR");
    Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_IFRD, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_IFRD_TEST_TASK_PRIO);
}

void SweepRobot_IFRDTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_IFRD)){
            SWRB_NextTestTaskResumePreAct(SWRB_IFRD_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
            
            Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_IFRD, GUI_GREEN, CHECKBOX_CI_ENABLED);

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_IFRDTestInit();
            }

            if(gSwrbTestTaskRunCnt%2){
                SweepRobot_IFRDTestTxOffProc();
            }else{
                SweepRobot_IFRDTestTxOnProc();
            }
            
            if(gSwrbTestTaskRunCnt > 50){
                SweepRobot_IFRDTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,10);
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
