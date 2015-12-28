#include "sweeprobot_testing_uniwheel.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static UNIWHEEL_TestTypeDef uniwheel;

static const int SWRB_UNIWHEEL_VALID_THRESHOLD = 120;

static void SweepRobot_UniWheelTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_UNIWHEEL_TEST_TASK_PRIO;
    
    str = "\r\n>>>UNIWHEEL TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef __SHOW_TEST_TITLE    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
    
    printf("SENSOR->IFRD_LED=0\r\n");
    uniwheel.onValue = 0;
    uniwheel.offValue = 0;
    uniwheel.validCnt = 0;
    uniwheel.validFlag = 0;
}

static void SweepRobot_UniWheelTestTxOffProc(void)
{
    u8 i;
    
    if(!uniwheel.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SENSOR->READ=8\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                uniwheel.offValue = usartRxNum;
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, usartRxNum);
                usartRxFlag = 0;
                usartRxNum = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        printf("SENSOR->IFRD_LED=1\r\n");
    }
}

static void SweepRobot_UniWheelTestTxOnProc(void)
{
    u8 i;
    char *str;
    
    if(!uniwheel.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SENSOR->READ=8\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                uniwheel.onValue = usartRxNum;
                Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1, usartRxNum);
                usartRxFlag = 0;
                usartRxNum = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        printf("SENSOR->IFRD_LED=0\r\n");
  
        if(uniwheel.offValue - uniwheel.onValue > SWRB_UNIWHEEL_VALID_THRESHOLD){
            gSwrbTestStateMap &= ~( (u32)1<<SWRB_TEST_UNIWHEEL_POS);
            uniwheel.validCnt++;
        }else{
            gSwrbTestStateMap |= ( (u32)1<<SWRB_TEST_UNIWHEEL_POS);
        }
        
        if(uniwheel.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            uniwheel.validFlag = 1;
        }
        
        if(uniwheel.validFlag){
            gSwrbTestTaskRunCnt = 0;
            printf("SENSOR->IFRD_LED=0\r\n");

            SWRB_TestDataSaveToFile(UNIWHEEL_TestDataSave);
            
            str = "UNIWHEEL OK\r\n";
            SWRB_TestDataFileWriteString(str);
            
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_UNIWHEEL, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_UNIWHEEL, "UNIWHEEL OK");
            Edit_Clear();

            SWRB_NextTestTaskResumePostAct(SWRB_UNIWHEEL_TEST_TASK_PRIO);
        }
    }
}

static void SweepRobot_UniwheelTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("SENSOR->IFRD_LED=0\r\n");
    
    SWRB_TestDataSaveToFile(UNIWHEEL_TestDataSave);
    
    str = "ERROR->UNIWHEEL\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_UNIWHEEL, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_UNIWHEEL, "UNIWHEEL ERROR");
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_UNIWHEEL_TEST_TASK_PRIO);
#endif
}

void SweepRobot_UniWheel_Test_Task(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_UNIWHEEL)){
            SWRB_NextTestTaskResumePreAct(SWRB_UNIWHEEL_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_UniWheelTestInit();
            }
            
            if(gSwrbTestTaskRunCnt%2){
                SweepRobot_UniWheelTestTxOffProc();
            }else{
                SweepRobot_UniWheelTestTxOnProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_UniwheelTestOverTimeProc();
            }
            
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void UNIWHEEL_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOn_POS] = uniwheel.onValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOff_POS] = uniwheel.offValue;
    SWRB_TestDataFileWriteData("UNIWHEEL->onValue=", uniwheel.onValue, 1);
    SWRB_TestDataFileWriteData("UNIWHEEL->offValue=", uniwheel.offValue, 1);
}
