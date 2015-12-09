#include "sweeprobot_testing_ash_tray.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define ASH_TRAY_TEST_CTRL_RCC          RCC_AHB1Periph_GPIOB
#define ASH_TRAY_TEST_CTRL_GPIO         GPIOB
#define ASH_TRAY_TEST_CTRL_PIN          GPIO_Pin_6
#define ASH_TRAY_TEST_CTRL_PIN_SOURCE   GPIO_PinSource6
#define ASH_TRAY_TEST_CTRL_GPIO_AF_PPP  GPIO_AF_TIM4
#define ASH_TRAY_TEST_CTRL_TIM          TIM4
#define ASH_TRAY_TEST_CTRL_TIM_RCC      RCC_APB1Periph_TIM4

const static int SWRB_ASH_TRAY_LVL_VALID_THRESHOLD = 120;

static ASH_TRAY_TestTypeDef ashTrayIns;
static ASH_TRAY_TestTypeDef ashTrayLvl;

static void SweepRobot_AshTrayTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_RCC, ENABLE);
    
    GPIO_PinAFConfig(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN_SOURCE, ASH_TRAY_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = ASH_TRAY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ASH_TRAY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(ASH_TRAY_TEST_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ASH_TRAY_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(ASH_TRAY_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(ASH_TRAY_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
    
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlTestPos(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 1000);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlIdlePos(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlOff(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    OSTimeDlyHMSM(0,0,0,50);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, DISABLE);
}

static void SweepRobot_AshTrayTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_ASH_TRAY_TEST_TASK_PRIO;
    
    str = "\r\n>>>ASH TRAY TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);

    printf("SENSOR->IFRD_LED=0\r\n");
    SweepRobot_AshTrayTestInsCtrlTestPos();
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SweepRobot_AshTrayInsTestProc(void)
{
    u8 i;
    
    if(!ashTrayIns.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("ASH_TRAY->READ=0\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayIns.value = usartRxNum;
                Edit_Set_Value(hWin_SWRB_MAIN, ID_MAIN_EDIT_U1, usartRxNum);
                usartRxNum = 0;
                usartRxFlag = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        if(!ashTrayIns.value){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_ASH_TRAY_INS_POS);
            ashTrayIns.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_ASH_TRAY_INS_POS);
        }
        if(ashTrayIns.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            ashTrayIns.validFlag = 1;
        }
        
        if(ashTrayIns.validFlag){
            SweepRobot_AshTrayTestInsCtrlIdlePos();
        }
    }
}

static void SweepRobot_AshTrayLvlTestTxOffProc(void)
{
    u8 i;
    
    if(!ashTrayLvl.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("SENSOR->READ=15\r\n");
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayLvl.offValue = usartRxNum;
                Edit_Set_Value(hWin_SWRB_MAIN, ID_MAIN_EDIT_U2, usartRxNum);
                usartRxFlag=0;
                usartRxNum=0;
                USART_RX_STA = 0;
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
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
            if(usartRxFlag){
                ashTrayLvl.onValue = usartRxNum;
                Edit_Set_Value(hWin_SWRB_MAIN, ID_MAIN_EDIT_D2, usartRxNum);
                usartRxFlag = 0;
                usartRxNum = 0;
                USART_RX_STA = 0;
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
        SweepRobot_AshTrayTestInsCtrlIdlePos();

        SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);
        
        str = "ASH TRAY OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_ASH_TRAY, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_ASH_TRAY, "ASH TRAY OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_AshTrayTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("SENSOR->IFRD_LED=0\r\n");
    SweepRobot_AshTrayTestInsCtrlIdlePos();

    SWRB_TestDataSaveToFile(ASH_TRAY_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK){
        str = "ERROR->ASH TRAY INS\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK){
        str = "ERROR->ASH TRAY LVL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN, str);
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_ASH_TRAY, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_ASH_TRAY, "ASH TRAY ERROR");
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
}

void SweepRobot_AshTrayTestTask(void *pdata)
{

    SweepRobot_AshTrayTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_ASH_TRAY)){
            SWRB_NextTestTaskResumePreAct(SWRB_ASH_TRAY_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_AshTrayTestInit();
            }
            
            SweepRobot_AshTrayTestProc();

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_AshTrayTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void ASH_TRAY_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
    SWRB_TestDataFileWriteData("ASH_TRAY->INS_Value=", ashTrayIns.value, 1);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_onValue=", ashTrayLvl.onValue, 1);
    SWRB_TestDataFileWriteData("ASH_TRAY->LVL_offValue=", ashTrayLvl.offValue, 1);
}
