#include "sweepRobot_testing_charge.h"

#include "usart.h"
#include "includes.h"

#define CHARGE_TEST_CTRL_RCC        RCC_AHB1Periph_GPIOB
#define CHARGE_TEST_CTRL_GPIO       GPIOC
#define CHARGE_TEST_CTRL_PIN        GPIO_Pin_7

static u8 swrbChargeTestStateMap = 0;

#define SWRB_TEST_CHARGE_CUR_POS            0
#define SWRB_TEST_CHARGE_VOL_POS            1

#define SWRB_TEST_FAULT_CHARGE_CUR_MASK     0x01
#define SWRB_TEST_FAULT_CHARGE_VOL_MASK     0x02

#define SWRB_TEST_CHARGE_CUR_HIGH_BOUND     100
#define SWRB_TEST_CHARGE_CUR_LOW_BOUND      0

#define SWRB_TEST_CHARGE_OC_THRESHOLD       0

#define SWRB_TEST_CHARGE_VOL_HIGH_BOUND     100
#define SWRB_TEST_CHARGE_VOL_LOW_BOUND      0

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
    MultiEdit_Add_Text(str);

    SweepRobot_Charge24VOn();
    OSTimeDlyHMSM(0,0,1,0);

    printf("CHARGE->ON\r\n");
    charge.current = 0;
    charge.curValidCnt = 0;
    charge.curValidFlag = 0;
    charge.voltage = 0;
    charge.volValidCnt = 0;
    charge.volValidFlag = 0;
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
                Edit_Set_Value(ID_EDIT_U1, usartRxNum);
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
            charge.curValidCnt = 0;
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
                Edit_Set_Value(ID_EDIT_U2, usartRxNum);
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
            charge.volValidCnt = 0;
        }

        if(charge.volValidCnt > SWRB_TEST_VALID_COMP_TIMES){
            charge.volValidFlag = 1;
        }
    }

    if(charge.curValidFlag && charge.volValidFlag){
        gSwrbTestTaskRunCnt = 0;
        printf("CHARGE->OFF\r\n");
        SweepRobot_Charge24VOff();
        
        gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_CUR_POS] = charge.current;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_VOL_POS] = charge.voltage;
        SWRB_TestDataSaveToFile(CHARGE_TestDataSave);
        
        str = "CHARGE OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(str);
        Checkbox_Set_Text_Color(ID_CHECKBOX_CHARGE, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_CHARGE, "CHARGE OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_CHARGE);
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
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_CUR_POS] = charge.current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_VOL_POS] = charge.voltage;
    SWRB_TestDataSaveToFile(CHARGE_TestDataSave);
    
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_CUR_MASK){
        str = "ERROR->CHARGE CUR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_VOL_MASK){
        str = "ERROR->CHARGE VOL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_CHARGE, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_CHARGE, "CHARGE ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_CHARGE);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_CHARGE_TEST_TASK_PRIO);
}

void SweepRobot_ChargeTestTask(void *pdata)
{
    SweepRobot_ChargeTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_CHARGE)){
            SWRB_NextTestTaskResumePreAct(SWRB_CHARGE_TEST_TASK_PRIO);
        }else{
        
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
               SweepRobot_ChargeTestInit();
            }

            if(gSwrbTestStateMap > 1){
                SweepRobot_ChargeTestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_ChargeTestOverTimeProc();
            }

            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void CHARGE_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("CHARGE->Vol=", charge.voltage, 1);
    SWRB_TestDataFileWriteData("CHARGE->Cur=", charge.current, 1);
}
