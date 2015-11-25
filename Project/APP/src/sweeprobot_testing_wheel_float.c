#include "sweepRobot_testing_wheel_float.h"

#include "usart.h"
#include "includes.h"

#define WHEEL_FLOAT_TEST_CTRL_RCC       RCC_AHB1Periph_GPIOB
#define WHEEL_FLOAT_TEST_CTRL_GPIO      GPIOB
#define WHEEL_FLOAT_TEST_CTRL_L_PIN     GPIO_Pin_10
#define WHEEL_FLOAT_TEST_CTRL_R_PIN     GPIO_Pin_11
#define WHEEL_FLOAT_TEST_CTRL_TIM       TIM2

#define WHEEL_FLOAT_CHAN_NUM    2

enum WheelFloagChan{
    WHEEL_FLOAT_CHAN_L,
    WHEEL_FLOAT_CHAN_R, 
};

static WHEEL_FLOAT_TestTypeDef wheelFloat[WHEEL_FLOAT_CHAN_NUM];

static void SweepRobot_WheelFloatTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WHEEL_FLOAT_TEST_CTRL_L_PIN | WHEEL_FLOAT_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(WHEEL_FLOAT_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(WHEEL_FLOAT_TEST_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 2500-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_OCInitStructure);
    TIM_OC4Init(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC3PreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, DISABLE);
    
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, 2000);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, 2000);
}

void SweepRobot_WheelFloatCtrlOn(void)
{
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, 2400);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, 2400);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
    
//    GPIO_SetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_L_PIN);
//    GPIO_SetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_R_PIN);
}

void SweepRobot_WheelFloatCtrlOff(void)
{
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, 2000);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, 2000);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, DISABLE);
    
//    GPIO_ResetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_L_PIN);
//    GPIO_ResetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_R_PIN);
}

void SweepRobot_WheelFloatTestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_WHEEL_FLOAT_TEST_TASK_PRIO;
    
    str = "\r\n>>>WHEEL FLOAT TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    SweepRobot_WheelFloatCtrlOn();
    
    for(i=0;i<WHEEL_FLOAT_CHAN_NUM;i++){
        wheelFloat[i].value = 0;
        wheelFloat[i].validCnt = 0;
        wheelFloat[i].validFlag = 0;
    }
}

static void SweepRobot_WheelFloatTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<WHEEL_FLOAT_CHAN_NUM;i++){
        
        if(!wheelFloat[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("WHEEL_FLOAT->READ=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    wheelFloat[i].value = usartRxNum;
                    Edit_Set_Value(ID_EDIT_U1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
            if(wheelFloat[i].value){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_FLOAT_L_POS+i));
                wheelFloat[i].validCnt = 0;
            }
            if(wheelFloat[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                wheelFloat[i].validFlag = 1;
            }
        }
    }
    
    if(wheelFloat[WHEEL_FLOAT_CHAN_L].validFlag && wheelFloat[WHEEL_FLOAT_CHAN_R].validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_WheelFloatCtrlOff();

        gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_L_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_L].value;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_R_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_R].value;
        SWRB_TestDataSaveToFile(WHEEL_FLOAT_TestDataSave);
        
        str = "WHEEL FLOAT OK\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);

        Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL_FLOAT, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_WHEEL_FLOAT);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
    }
}

static void SweepRobot_WheelFloatTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_WheelFloatCtrlOff();
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_L_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_L].value;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_FLOAT_R_VALUE_POS] = wheelFloat[WHEEL_FLOAT_CHAN_R].value;
    SWRB_TestDataSaveToFile(WHEEL_FLOAT_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK){
        str = "ERROR->WHEEL FLOAT L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK){
        str = "ERROR->WHEEL FLOAT R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL_FLOAT, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT ERR");
    Progbar_Set_Percent(SWRB_TEST_STATE_WHEEL_FLOAT);
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
}

void SweepRobot_WheelFloatTestTask(void *pdata)
{
    
    SweepRobot_WheelFloatTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_WHEEL_FLOAT)){
            SWRB_NextTestTaskResumePreAct(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO);
        }else{
        
            gSwrbTestTaskRunCnt++;
            
            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_WheelFloatTestInit();
            }
            
            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_WheelFloatTestProc();
            }
        
            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_WheelFloatTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void WHEEL_FLOAT_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->L_Value=", wheelFloat[WHEEL_FLOAT_CHAN_L].value, 1);
    SWRB_TestDataFileWriteData("WHEEL_FLOAT->R_Value=", wheelFloat[WHEEL_FLOAT_CHAN_R].value, 1);
}
