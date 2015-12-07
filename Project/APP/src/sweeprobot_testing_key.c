#include "sweeprobot_testing_key.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define KEY_TEST_CTRL_RCC           RCC_AHB1Periph_GPIOB
#define KEY_TEST_CTRL_GPIO          GPIOB
#define KEY_TEST_CTRL_PIN           GPIO_Pin_7
#define KEY_TEST_CTRL_PIN_SOURCE    GPIO_PinSource7
#define KEY_TEST_CTRL_GPIO_AF_PPP   GPIO_AF_TIM4
#define KEY_TEST_CTRL_TIM_RCC       RCC_APB1Periph_TIM4
#define KEY_TEST_CTRL_TIM           TIM4

static KEY_TestTypeDef key;

static void SweepRobot_KeyTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(KEY_TEST_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(KEY_TEST_CTRL_RCC, ENABLE);
    
    GPIO_PinAFConfig(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN_SOURCE, KEY_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = KEY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(KEY_TEST_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(KEY_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(KEY_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(KEY_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(KEY_TEST_CTRL_TIM, ENABLE);
    
    TIM_SetCompare2(KEY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_KeyTestCtrlTestPos(void)
{
    TIM_SetCompare1(KEY_TEST_CTRL_TIM, 1000);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
    
//    GPIO_SetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

void SweepRobot_KeyTestCtrlIdlePos(void)
{
    TIM_SetCompare1(KEY_TEST_CTRL_TIM, 300);
    
//    OSTimeDlyHMSM(0,0,1,0);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
    
//    GPIO_ResetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

static void SweepRobot_KeyTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_KEY_TEST_TASK_PRIO;
    
    str = "\r\n>>>KEY TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    
    SweepRobot_KeyTestCtrlTestPos();
    
    OSTimeDlyHMSM(0,0,1,0);

    key.value = 0;
    key.validCnt = 0;
    key.validFlag = 0;
}

static void SweepRobot_KeyTestProc(void)
{
    u8 i;
    char *str;
    
    if(!key.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("KEY->READ\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                key.value = usartRxNum;
                Edit_Set_Value(ID_MAIN_EDIT_U1, usartRxNum);
                usartRxNum = 0;
                usartRxFlag = 0;
                USART_RX_STA = 0;
                break;
            }else{
                continue;
            }
        }
        
        if(key.value){
            key.validFlag = 1;
        }
        
        if(key.validFlag){
            SweepRobot_KeyTestCtrlIdlePos();
        }
    }
    
    if(key.validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_KeyTestCtrlIdlePos();

        SWRB_TestDataSaveToFile(KEY_TestDataSave);
        
        str = "ERROR->KEY\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  "KEY OK\r\n");
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_KEY, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_KEY, "KEY OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_KEY_TEST_TASK_PRIO);
    }
}

static void SweepRobot_KeyTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_KeyTestCtrlIdlePos();

    SWRB_TestDataSaveToFile(KEY_TestDataSave);

    str = "ERROR->KEY\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_KEY, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_KEY, "KEY ERROR");
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_KEY_TEST_TASK_PRIO);
}

void SweepRobot_KeyTestTask(void *pdata)
{
    
    SweepRobot_KeyTestGPIOInit();
    
    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_KEY)){
            SWRB_NextTestTaskResumePreAct(SWRB_KEY_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
        
            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_KeyTestInit();
            }
            
            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_KeyTestProc();
            }
        
            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_KeyTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void KEY_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_KEY_VALUE_POS] = key.value;
    SWRB_TestDataFileWriteData("KEY->Value=", key.value, 1);
}
