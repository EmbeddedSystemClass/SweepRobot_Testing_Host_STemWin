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

static IRDA_TestTypeDef IrDA[SWRB_TEST_IRDA_CHAN_NUM];

void SweepRobot_IrDATestGPIOInit(void)
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

void SweepRobot_IrDATestTxSendCmd(u8 code)
{
    u8 i;
    OS_CPU_SR cpu_sr;
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

static void SweepRobot_IrDATestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_IRDA_TEST_TASK_PRIO;
    
    str = "\r\n>>>IRDA TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);

    OSTimeDlyHMSM(0,0,1,0);
    
    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        IrDA[i].code = 0;
        IrDA[i].validCnt = 0;
        IrDA[i].validFlag = 0;
        Edit_Set_Value(ID_EDIT_U1+i, 0);
    }
}

static void SweepRobot_IrDATestProc(void)
{
    u8 i,j;
    
    for(j=0;j<SWRB_TEST_IRDA_CHAN_NUM;j++){
        if(!IrDA[j].validFlag){
            printf("IRDA->ON=%d\r\n",j);
            OSTimeDlyHMSM(0,0,0,1);
            for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
//                    SweepRobot_IrDATestTxSendCmd(42);
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
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_IRDA_B_POS+j));
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_IRDA_B_POS+j));
            }
        }
    }

    if(IrDA[0].validFlag && IrDA[1].validFlag && IrDA[2].validFlag && IrDA[3].validFlag && IrDA[4].validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
            gSwrbTestAcquiredData[SWRB_TEST_DATA_IRDA_B_RxCODE_POS+i] = IrDA[i].code;
        }
        SWRB_TestDataSaveToFile(IRDA_TestDataSave);
        
        MultiEdit_Add_Text("IRDA OK\r\n");
        Checkbox_Set_Text_Color(ID_CHECKBOX_IRDA, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_IRDA, "IRDA OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_IRDA);

        SWRB_NextTestTaskResume(SWRB_IRDA_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IrDATestOverTimeProc(void)
{
    u8 i;
    
    gSwrbTestTaskRunCnt = 0;

    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IRDA_B_RxCODE_POS+i] = IrDA[i].code;
    }
    SWRB_TestDataSaveToFile(IRDA_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_B_MSAK){
        MultiEdit_Add_Text("ERROR->IRDA_B\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_L_MSAK){
        MultiEdit_Add_Text("ERROR->IRDA_L\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FL_MSAK){
        MultiEdit_Add_Text("ERROR->IRDA_FL\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FR_MSAK){
        MultiEdit_Add_Text("ERROR->IRDA_FR\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_R_MSAK){
        MultiEdit_Add_Text("ERROR->IRDA_R\r\n");
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_IRDA, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_IRDA, "IRDA ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_IRDA);

    SWRB_NextTestTaskResume(SWRB_IRDA_TEST_TASK_PRIO);
}

void SweepRobot_IrDATestTask(void *pdata)
{
    u8 i,j;
    char *str;

    SweepRobot_IrDATestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_IRDA)){
            SWRB_NextTestTaskResume(SWRB_IRDA_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_IrDATestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_IrDATestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_IrDATestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void IRDA_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("IRDA->B_Code=", IrDA[0].code);
    SWRB_TestDataFileWriteData("IRDA->L_Code=", IrDA[1].code);
    SWRB_TestDataFileWriteData("IRDA->FL_Code=", IrDA[2].code);
    SWRB_TestDataFileWriteData("IRDA->FR_Code=", IrDA[3].code);
    SWRB_TestDataFileWriteData("IRDA->R_Code=", IrDA[4].code);
}
