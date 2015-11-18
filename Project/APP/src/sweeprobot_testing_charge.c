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

void SweepRobot_Charge_Test_Init(void)
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

void SweepRobot_Charge_24V_On(void)
{
    GPIO_SetBits(CHARGE_TEST_CTRL_GPIO, CHARGE_TEST_CTRL_PIN);
}

void SweepRobot_Charge_24V_Off(void)
{
    GPIO_ResetBits(CHARGE_TEST_CTRL_GPIO, CHARGE_TEST_CTRL_PIN);
}

void SweepRobot_Charge_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static CHARGE_TestTypeDef charge;
    u8 i;
    char *str;

    SweepRobot_Charge_Test_Init();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_CHARGE)){
            OS_ENTER_CRITICAL();
#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_CHARGE_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_CHARGE_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
            OS_EXIT_CRITICAL();
        }else{
        
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
                MultiEdit_Set_Text_Color(GUI_BLACK);
                str = ">>>CHARGE TEST<<<\r\n";
                MultiEdit_Add_Text(str);
                mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
                mf_puts(str);
                OSTimeDlyHMSM(0,0,1,0);
                SweepRobot_Charge_24V_On();
                printf("CHARGE->ON\r\n");
                charge.current = 0;
                charge.curValidCnt = 0;
                charge.curValidFlag = 0;
                charge.voltage = 0;
                charge.volValidCnt = 0;
                charge.volValidFlag = 0;
            }

            if(gSwrbTestStateMap > 1){
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

                    if(charge.curValidCnt > 5){
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

                    if(charge.volValidCnt > 5){
                        charge.volValidFlag = 1;
                    }
                }

                if(charge.curValidFlag && charge.volValidFlag){
                    gSwrbTestTaskRunCnt = 0;
                    printf("CHARGE->OFF\r\n");
                    SweepRobot_Charge_24V_Off();
                    Edit_Set_Value(ID_EDIT_HEX, swrbChargeTestStateMap);
                    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_CUR_POS] = charge.current;
                    gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_VOL_POS] = charge.voltage;
                    Checkbox_Set_Text_Color(ID_CHECKBOX_CHARGE, GUI_BLUE);
                    Checkbox_Set_Text(ID_CHECKBOX_CHARGE, "CHARGE OK");
                    Progbar_Set_Value( (u8)((float)( (float)(SWRB_CHARGE_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_CHARGE_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

                    OS_ENTER_CRITICAL();

    #ifdef SWRB_TEST_TASK_RUN_OBO
                    if(SWRB_CHARGE_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                        OSTaskResume(SWRB_CHARGE_TEST_TASK_PRIO+1);
    #endif
                    OSTaskSuspend(OS_PRIO_SELF);
    //                OSTaskDel(OS_PRIO_SELF);

                    OS_EXIT_CRITICAL();
                }
            }

            if(gSwrbTestTaskRunCnt > 20){
                gSwrbTestTaskRunCnt = 0;
                printf("CHARGE->OFF\r\n");
                SweepRobot_Charge_24V_Off();
                gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_CUR_POS] = charge.current;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_CHARGE_VOL_POS] = charge.voltage;
                if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_CUR_MASK)
                    MultiEdit_Add_Text("ERROR->CHARGE CUR\r\n");
                if(swrbChargeTestStateMap & SWRB_TEST_FAULT_CHARGE_VOL_MASK)
                    MultiEdit_Add_Text("ERROR->CHARGE VOL\r\n");
                Checkbox_Set_Text_Color(ID_CHECKBOX_CHARGE, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_CHARGE, "CHARGE OK");
                Progbar_Set_Value( (u8)((float)( (float)(SWRB_CHARGE_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_CHARGE_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

                OS_ENTER_CRITICAL();

    #ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_CHARGE_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_CHARGE_TEST_TASK_PRIO+1);
    #endif
                OSTaskSuspend(OS_PRIO_SELF);
    //            OSTaskDel(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }

            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}
