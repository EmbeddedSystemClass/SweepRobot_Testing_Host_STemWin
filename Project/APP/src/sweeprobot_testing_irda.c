#include "sweeprobot_testing_irda.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

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

static void SweepRobot_IrDATestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(IRDA_TEST_TX_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IRDA_TEST_TX_L_PIN |\
                                  IRDA_TEST_TX_R_PIN |\
                                  IRDA_TEST_TX_M_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(IRDA_TEST_TX_GPIO, &GPIO_InitStructure);
}

static void SweepRobot_IrDATestGPIOPINSet(void)
{
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_M_PIN);
}

static void SweepRobot_IrDATestGPIOPINReset(void)
{
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_M_PIN);
}

void SweepRobot_IrDATestTxSendCmd(u8 code)
{
    u8 i;
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL();

    SweepRobot_IrDATestGPIOPINSet();
    delay_us(3000);

    for(i=0;i<8;i++){
        if(code & 0x80){
            SweepRobot_IrDATestGPIOPINReset();
            delay_us(1600);
            SweepRobot_IrDATestGPIOPINSet();
            delay_us(800);
        }else{
            SweepRobot_IrDATestGPIOPINReset();
            delay_us(800);
            SweepRobot_IrDATestGPIOPINSet();
            delay_us(1600);
        }
        code<<=1;
    }

    SweepRobot_IrDATestGPIOPINReset();

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
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);
    
    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        IrDA[i].code = 0;
        IrDA[i].validCnt = 0;
        IrDA[i].validFlag = 0;
        Edit_Set_Value(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1+i, 0);
    }
}

static void SweepRobot_IrDATestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        if(!IrDA[i].validFlag){
            printf("IRDA->ON=%d\r\n",i);
            OSTimeDlyHMSM(0,0,0,1);
            
            /* Send IrDA control code from Host */
            SweepRobot_IrDATestTxSendCmd(42);
            /* Send IrDA control code from DUT */
//            printf("IRDA->ON=42\r\n");
            
            OSTimeDlyHMSM(0,0,0,24);
            for(j=0;j<SWRB_TEST_USART_IRDA_READ_TIMES;j++){
                printf("IRDA->READ\r\n");
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    IrDA[i].code = usartRxNum;
                    Edit_Set_HexMode(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1+i, 0, 0, 255);
                    Edit_Set_Value(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1+i, IrDA[i].code);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    USART_RX_STA = 0;
                    break;
                }else{
                    IrDA[i].code = 0;
                    continue;
                }
            }

            if(IrDA[i].code == 0x40 || IrDA[i].code == 0x41 || IrDA[i].code == 0x42 || IrDA[i].code == 0x43 ||\
               IrDA[i].code == 0x44 || IrDA[i].code == 0x45 || IrDA[i].code == 0x46){
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
        }
    }

    if(IrDA[0].validFlag && IrDA[1].validFlag && IrDA[2].validFlag && IrDA[3].validFlag && IrDA[4].validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        printf("IRDA->OFF\r\n");
        
        SWRB_TestDataSaveToFile(IRDA_TestDataSave);
        
        str = "IRDA OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_IRDA, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IRDA, "IRDA OK");
        for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
            Edit_Set_DecMode(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1+i, 0, 0, 65536, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
        }
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_IRDA_TEST_TASK_PRIO);
    }
}

static void SweepRobot_IrDATestOverTimeProc(void)
{
    u8 i;
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    
    printf("IRDA->OFF\r\n");

    SWRB_TestDataSaveToFile(IRDA_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_B_MSAK){
        str = "ERROR->IRDA_B\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_L_MSAK){
        str = "ERROR->IRDA_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FL_MSAK){
        str = "ERROR->IRDA_FL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_FR_MSAK){
        str = "ERROR->IRDA_FR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_IRDA_R_MSAK){
        str = "ERROR->IRDA_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_IRDA, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_IRDA, "IRDA ERROR");
    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        Edit_Set_DecMode(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1+i, 0, 0, 65536, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
    }
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_IRDA_TEST_TASK_PRIO);
}

void SweepRobot_IrDATestTask(void *pdata)
{
    
    SweepRobot_IrDATestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_IRDA)){
            SWRB_NextTestTaskResumePreAct(SWRB_IRDA_TEST_TASK_PRIO);
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
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void IRDA_TestDataSave(void)
{
    u8 i;
    
    for(i=0;i<SWRB_TEST_IRDA_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_IRDA_B_RxCODE_POS+i] = IrDA[i].code;
    }
    
    SWRB_TestDataFileWriteData("IRDA->B_Code=", IrDA[0].code, 1);
    SWRB_TestDataFileWriteData("IRDA->L_Code=", IrDA[1].code, 1);
    SWRB_TestDataFileWriteData("IRDA->FL_Code=", IrDA[2].code, 1);
    SWRB_TestDataFileWriteData("IRDA->FR_Code=", IrDA[3].code, 1);
    SWRB_TestDataFileWriteData("IRDA->R_Code=", IrDA[4].code, 1);
}
