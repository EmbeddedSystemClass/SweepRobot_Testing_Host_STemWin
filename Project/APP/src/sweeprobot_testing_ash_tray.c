#include "sweepRobot_testing_ash_tray.h"

#include "usart.h"
#include "includes.h"

#define ASH_TRAY_TEST_CTRL_RCC          RCC_AHB1Periph_GPIOE
#define ASH_TRAY_TEST_CTRL_GPIO         GPIOE
#define ASH_TRAY_TEST_CTRL_PIN          GPIO_Pin_5

#define SWRB_ASH_TRAY_LVL_VALID_THRESHOLD   120

static ASH_TRAY_TestTypeDef ashTrayIns;
static ASH_TRAY_TestTypeDef ashTrayLvl;

static void SweepRobot_AshTrayTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ASH_TRAY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ASH_TRAY_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_AshTrayTestInsCtrlOn(void)
{
    GPIO_SetBits(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN);
}

void SweepRobot_AshTrayTestInsCtrlOff(void)
{
    GPIO_ResetBits(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN);
}

static void SweepRobot_AshTrayTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_ASH_TRAY_TEST_TASK_PRIO;
    
    str = "\r\n>>>ASH TRAY TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);

    printf("SENSOR->IFRD_LED=0\r\n");
    SweepRobot_AshTrayTestInsCtrlOn();
    
    OSTimeDlyHMSM(0,0,1,0);
}

static void SweepRobot_AshTrayInsTestProc(void)
{
    u8 i;
    
    if(!ashTrayIns.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("ASH_TRAY->READ=0\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                ashTrayIns.value = usartRxNum;
                Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                usartRxNum = 0;
                usartRxFlag = 0;
                break;
            }else{
                continue;
            }
        }
        if(ashTrayIns.value){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_ASH_TRAY_INS_POS);
            ashTrayIns.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_ASH_TRAY_INS_POS);
            ashTrayIns.validCnt = 0;
        }
        if(ashTrayIns.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            ashTrayIns.validFlag = 1;
        }
        
        if(ashTrayIns.validFlag){
            SweepRobot_AshTrayTestInsCtrlOff();
        }
    }
}

static void SweepRobot_AshTrayLvlTestTxOffProc(void)
{
    u8 i;
    
    if(!ashTrayLvl.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SENSOR->READ=15\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                ashTrayLvl.offValue = usartRxNum;
                Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                usartRxFlag=0;
                usartRxNum=0;
                break;
            }else{
                continue;
            }
        }
        printf("SENSOR->IFRD_LED=1\r\n");
    }
}

static void SweepRobot_AshTrayLvlTestTxOnProc(void)
{
    u8 i;
    
    if(!ashTrayLvl.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SENSOR->READ=15\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                ashTrayLvl.onValue = usartRxNum;
                Edit_Set_Value(ID_EDIT_D1, usartRxNum);
                usartRxFlag = 0;
                usartRxNum = 0;
                break;
            }else{
                continue;
            }
        }
        printf("SENSOR->IFRD_LED=0\r\n");

        if(ashTrayLvl.offValue - ashTrayLvl.onValue > SWRB_ASH_TRAY_LVL_VALID_THRESHOLD){
            gSwrbTestStateMap &= ~( (u32)1<<SWRB_TEST_ASH_TRAY_LVL_POS);
            ashTrayLvl.validCnt++;
        }else{
            gSwrbTestStateMap |= ( (u32)1<<SWRB_TEST_ASH_TRAY_LVL_POS);
            ashTrayLvl.validCnt = 0;
        }
        
        if(ashTrayLvl.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            ashTrayLvl.validFlag = 1;
        }
    }
}

static void SweepRobot_AshTrayTestProc(void)
{
    char *str;
    
    if(gSwrbTestTaskRunCnt > 1){
        SweepRobot_AshTrayInsTestProc();
    }

    if(gSwrbTestTaskRunCnt%2){
        SweepRobot_AshTrayLvlTestTxOffProc();
    }else{
        SweepRobot_AshTrayLvlTestTxOnProc();
    }

    if(ashTrayIns.validFlag && ashTrayLvl.validFlag){
        gSwrbTestTaskRunCnt = 0;
        printf("SENSOR->IFRD_LED=0\r\n");
        SweepRobot_AshTrayTestInsCtrlOff();
        
        gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
        SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);
        
        str = "ASH TRAY OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(str);
        Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_ASH_TRAY);

        SWRB_NextTestTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_AshTrayTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("SENSOR->IFRD_LED=0\r\n");
    SweepRobot_AshTrayTestInsCtrlOff();

    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
    SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK){
        str = "ERROR->ASH TRAY INS\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK){
        str = "ERROR->ASH TRAY LVL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_ASH_TRAY);

    SWRB_NextTestTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO);
}

void SweepRobot_AshTrayTestTask(void *pdata)
{

    SweepRobot_AshTrayTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_ASH_TRAY)){
            SWRB_NextTestTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_AshTrayTestInit();
            }
            
            SweepRobot_AshTrayTestProc();

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_AshTrayTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void ASH_TRAY_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("ASH_TRAY->INS_Value=", ashTrayIns.value);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_onValue=", ashTrayLvl.onValue);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_offValue=", ashTrayLvl.offValue);
}
