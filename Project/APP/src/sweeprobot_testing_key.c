#include "sweepRobot_testing_key.h"

#include "usart.h"
#include "includes.h"

#define KEY_TEST_CTRL_RCC       RCC_AHB1Periph_GPIOE
#define KEY_TEST_CTRL_GPIO      GPIOE
#define KEY_TEST_CTRL_PIN       GPIO_Pin_6

void SweepRobot_Key_Test_Ctrl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(KEY_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(KEY_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Key_Test_Ctrl_On(void)
{
    GPIO_SetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

void SweepRobot_Key_Test_Ctrl_Off(void)
{
    GPIO_ResetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

void SweepRobot_Key_Test_Task(void *pdata)
{
    static KEY_TestTypeDef key;
    OS_CPU_SR cpu_sr;
    
    u8 i;
  
    while(1){
        swrbTestTaskCnt++;
    
        if(swrbTestTaskCnt == 1){
            swrbTestRuningTaskPrio = SWRB_KEY_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>KEY TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            SweepRobot_Key_Test_Ctrl_On();
        }
        
        if(swrbTestTaskCnt > 1){
            if(!key.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("KEY->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        key.value = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                
                if(key.value){
                    key.validFlag = 1;
                }
                
                if(key.validFlag){
                    SweepRobot_Key_Test_Ctrl_Off();
                }
            }
            
            if(key.validFlag){
                swrbTestTaskCnt = 0;
                Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
                Checkbox_Set_State(ID_CHECKBOX_KEY, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_KEY, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_KEY, "KEY OK");
                Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_KEY / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

                OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_KEY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_KEY_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }
    
        if(swrbTestTaskCnt > 20){
            swrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
            MultiEdit_Set_Text_Color(GUI_RED);
            MultiEdit_Add_Text("ERROR->KEY\r\n");
            Checkbox_Set_State(ID_CHECKBOX_KEY, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_KEY, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_KEY, "KEY ERROR");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_KEY / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

            OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_KEY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_KEY_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
    OSTimeDlyHMSM(0,0,0,50);
  }
}
