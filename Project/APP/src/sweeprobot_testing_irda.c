#include "sweepRobot_testing_irda.h"

#include "usart.h"
#include "includes.h"

#define IRDA_TEST_TX_RCC                RCC_AHB1Periph_GPIOA
#define IRDA_TEST_TX_GPIO               GPIOA
#define IRDA_TEST_TX_L_PIN              GPIO_Pin_4
#define IRDA_TEST_TX_R_PIN              GPIO_Pin_5
#define IRDA_TEST_TX_M_PIN              GPIO_Pin_6

#define IRDA_TEST_TX_PIN_SET(pin)       GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_SET)
#define IRDA_TEST_TX_PIN_RESET(pin)     GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_RESET)

#define SWRB_TEST_IRDA_CHAN_NUM         5

void SweepRobot_IrDA_Test_Tx_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(IRDA_TEST_TX_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IRDA_TEST_TX_L_PIN |\
                                  IRDA_TEST_TX_R_PIN |\
                                  IRDA_TEST_TX_M_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(IRDA_TEST_TX_GPIO, &GPIO_InitStructure);
}

void SweepRobot_IrDA_Test_Tx_SendCmd(u8 code)
{
    OS_CPU_SR cpu_sr;
    u8 i;

    OS_ENTER_CRITICAL();

    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_M_PIN);
    delay_us(3000);

    for(i=0;i<8;i++){
        if(code & 0x80){
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_L_PIN);
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_R_PIN);
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_M_PIN);
            delay_us(1600);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_L_PIN);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_R_PIN);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_M_PIN);
            delay_us(800);
        }else{
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_L_PIN);
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_R_PIN);
            IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_M_PIN);
            delay_us(800);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_L_PIN);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_R_PIN);
            IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_M_PIN);
            delay_us(1600);
        }
        code<<=1;
    }

    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_M_PIN);

    OS_EXIT_CRITICAL();
}

void SweepRobot_IrDA_Test_Task(void *pdata)
{
    static u16 swrbTestTaskCnt = 0;
    static IRDA_TestTypeDef IrDA[SWRB_TEST_IRDA_CHAN_NUM];
    OS_CPU_SR cpu_sr;

    u8 i,j;

    SweepRobot_IrDA_Test_Tx_Init();

    while(1){
        swrbTestTaskCnt++;

        if(swrbTestTaskCnt == 1){
            swrbTestRuningTaskPrio = SWRB_IRDA_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>IRDA TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            for(j=0;j<SWRB_TEST_IRDA_CHAN_NUM;j++){
                IrDA[j].code = 0;
                IrDA[j].validCnt = 0;
                IrDA[j].validFlag = 0;
                Edit_Set_Value(ID_EDIT_U1+j, 0);
            }
        }

        if(swrbTestTaskCnt > 1){
            for(j=0;j<SWRB_TEST_IRDA_CHAN_NUM;j++){
                if(!IrDA[j].validFlag){
                    printf("IRDA->ON=%d\r\n",j);
                    OSTimeDlyHMSM(0,0,0,1);
                    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
//                    SweepRobot_IrDA_Test_Tx_SendCmd(42);
//                    printf("IRDA->ON=42\r\n");
//                    OSTimeDlyHMSM(0,0,0,1);
                        printf("IRDA->READ\r\n");
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            IrDA[j].code = usartRxNum;
                            Edit_Set_Value(ID_EDIT_U1+j, IrDA[j].code);
                            usartRxNum = 0;
                            usartRxFlag = 0;
                            break;
                        }else{
                            IrDA[j].code = 0;
                            continue;
                        }
                    }
                    
                    if(IrDA[j].code == 42){
                        IrDA[j].validCnt++;
                    }else{
                        IrDA[j].validCnt = 0;
                    }
                    if(IrDA[j].validCnt){
                        IrDA[j].validFlag = 1;
                    }
                    
                    if(IrDA[j].validFlag){
                        swrbTestStateMap &= ~(1<<(SWRB_TEST_IRDA_B_POS+j));
                    }else{
                        swrbTestStateMap |= (1<<(SWRB_TEST_IRDA_B_POS+j));
                    }
                }
            }
            
            if(IrDA[0].validFlag && IrDA[1].validFlag && IrDA[2].validFlag && IrDA[3].validFlag && IrDA[4].validFlag){
                swrbTestTaskCnt = 0;
                Checkbox_Set_State(ID_CHECKBOX_IRDA, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_IRDA, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_IRDA, "IRDA OK");
                Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_IRDA / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

                OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_IRDA_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_IRDA_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }

        if(swrbTestTaskCnt > 20){
            swrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
            MultiEdit_Set_Text_Color(GUI_RED);
            if(swrbTestStateMap & SWRB_TEST_FAULT_IRDA_B_MSAK){
                MultiEdit_Add_Text("ERROR->IRDA_B\r\n");
            }
            if(swrbTestStateMap & SWRB_TEST_FAULT_IRDA_L_MSAK){
                MultiEdit_Add_Text("ERROR->IRDA_L\r\n");
            }
            if(swrbTestStateMap & SWRB_TEST_FAULT_IRDA_FL_MSAK){
                MultiEdit_Add_Text("ERROR->IRDA_FL\r\n");
            }
            if(swrbTestStateMap & SWRB_TEST_FAULT_IRDA_FR_MSAK){
                MultiEdit_Add_Text("ERROR->IRDA_FR\r\n");
            }
            if(swrbTestStateMap & SWRB_TEST_FAULT_IRDA_R_MSAK){
                MultiEdit_Add_Text("ERROR->IRDA_R\r\n");
            }
            Checkbox_Set_State(ID_CHECKBOX_IRDA, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_IRDA, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_IRDA, "IRDA ERROR");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_IRDA / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

            OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_IRDA_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_IRDA_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
        OSTimeDlyHMSM(0,0,0,50);
    }
}
