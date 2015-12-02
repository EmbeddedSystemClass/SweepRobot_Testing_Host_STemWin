#include "sweeprobot_testing_charge.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define CHARGE_TEST_CTRL_RCC        RCC_AHB1Periph_GPIOC
#define CHARGE_TEST_CTRL_GPIO       GPIOC
#define CHARGE_TEST_CTRL_PIN        GPIO_Pin_7

static u8 swrbChargeTestStateMap = 0;

#define SWRB_TEST_CHARGE_CUR_POS            0
#define SWRB_TEST_CHARGE_VOL_POS            1
#define SWRB_TEST_CHARGE_24V_POS            2

#define SWRB_TEST_FAULT_CHARGE_CUR_MASK     0x01
#define SWRB_TEST_FAULT_CHARGE_VOL_MASK     0x02
#define SWRB_TEST_FAULT_CHARGE_24V_MASK     0x04

#define SWRB_TEST_CHARGE_CUR_HIGH_BOUND     100
#define SWRB_TEST_CHARGE_CUR_LOW_BOUND      20

#define SWRB_TEST_CHARGE_OC_THRESHOLD       0

#define SWRB_TEST_CHARGE_VOL_HIGH_BOUND     4000
#define SWRB_TEST_CHARGE_VOL_LOW_BOUND      2000

#define SWRB_TEST_CHARGE_OV_THRESHOLD       0

static CHARGE_TestTypeDef charge;

static void SweepRobot_ChargeTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(CHARGE_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = CHARGE_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CHARGE_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Charge24VOn(void)
{
    GPIO_SetBits(CHARGE_TEST_CTRL_GPIO, CHARGE_TEST_CTRL_PIN);
}

void SweepRobot_Charge24VOff(void)
{
    GPIO_ResetBits(CHARGE_TEST_CTRL_GPIO, CHARGE_TEST_CTRL_PIN);
}

static void SweepRobot_ChargeTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
    
    str = "\r\n>>>CHARGE TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);

    SweepRobot_Charge24VOn();
    OSTimeDlyHMSM(0,0,1,0);

    printf("CHARGE->ON\r\n");
    charge.current = 0;
    charge.curValidCnt = 0;
    charge.curValidFlag = 0;
    charge.voltage = 0;
    charge.volValidCnt = 0;
    charge.volValidFlag = 0;
    charge.charge24vState = 0;
    charge.charge24vValidCnt = 0;
    charge.charge24vValidFlag = 0;
}

static void SweepRobot_ChargeTestProc(void)
{
    u8 i;
    char *str;
    
    if(!charge.curValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CHARGE->READ=1\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                Edit_Set_Value(ID_MAIN_EDIT_U1, usartRxNum);
                charge.current = usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                break;
            }else{
                continue;
            }
        }

        if( SWRB_TEST_CHARGE_CUR_LOW_BOUND<charge.current && SWRB_TEST_CHARGE_CUR_HIGH_BOUND>charge.current ){
            swrbChargeTestStateMap &= ~(1<<SWRB_TEST_CHARGE_CUR_POS);
            charge.curValidCnt++;
        }else{
            swrbChargeTestStateMap |= (1<<SWRB_TEST_CHARGE_CUR_POS);
        }

        if(charge.curValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.curValidFlag = 1;
        }
    }

    if(!charge.volValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CHARGE->READ=0\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                Edit_Set_Value(ID_MAIN_EDIT_U2, usartRxNum);
                charge.voltage = usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                break;
            }else{
                continue;
            }
        }

        if(SWRB_TEST_CHARGE_VOL_LOW_BOUND < charge.voltage && SWRB_TEST_CHARGE_VOL_HIGH_BOUND > charge.voltage){
            swrbChargeTestStateMap &= ~(1<<SWRB_TEST_CHARGE_VOL_POS);
            charge.volValidCnt++;
        }else{
            swrbChargeTestStateMap |= (1<<SWRB_TEST_CHARGE_VOL_POS);
        }

        if(charge.volValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.volValidFlag = 1;
        }
    }
    
    if(!charge.charge24vValidFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CHARGE->READ=2\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                Edit_Set_Value(ID_MAIN_EDIT_U3, usartRxNum);
                charge.charge24vState = usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
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
        }
    }

    if(charge.curValidFlag && charge.volValidFlag && charge.charge24vValidFlag){
        gSwrbTestTaskRunCnt = 0;
        printf("CHARGE->OFF\r\n");
        SweepRobot_Charge24VOff();
        
        SWRB_TestDataSaveToFile(CHARGE_TestDataSave);
        
        str = "CHARGE OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_CHARGE, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_CHARGE, "CHARGE OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_CHARGE_TEST_TASK_PRIO);
    }
}

static void SweepRobot_ChargeTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("CHARGE->OFF\r\n");
    SweepRobot_Charge24VOff();

    SWRB_TestDataSaveToFile(CHARGE_TestDataSave);
    
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_CUR_MASK){
        str = "ERROR->CHARGE CUR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_VOL_MASK){
        str = "ERROR->CHARGE VOL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_24V_MASK){
        str = "ERROR->CHARGE 24V\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_CHARGE, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_CHARGE, "CHARGE ERROR");
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_CHARGE_TEST_TASK_PRIO);
}

void SweepRobot_ChargeTestTask(void *pdata)
{
    SweepRobot_ChargeTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_CHARGE)){
            SWRB_NextTestTaskResumePreAct(SWRB_CHARGE_TEST_TASK_PRIO);
        }else{
        
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_ChargeTestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_ChargeTestProc();
            }

            if(gSwrbTestTaskRunCnt > 60){
                SweepRobot_ChargeTestOverTimeProc();
            }

            OSTimeDlyHMSM(0,0,0,50);
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
