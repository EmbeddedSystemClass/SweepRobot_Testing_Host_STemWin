#include "sweepRobot_testing_wheel_float.h"

#include "usart.h"
#include "includes.h"

#define WHEEL_FLOAT_TEST_CTRL_RCC       RCC_AHB1Periph_GPIOD
#define WHEEL_FLOAT_TEST_CTRL_GPIO      GPIOD
#define WHEEL_FLOAT_TEST_CTRL_L_PIN     GPIO_Pin_6
#define WHEEL_FLOAT_TEST_CTRL_R_PIN     GPIO_Pin_7

void SweepRobot_Wheel_Float_Test_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WHEEL_FLOAT_TEST_CTRL_L_PIN | WHEEL_FLOAT_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(WHEEL_FLOAT_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Wheel_Float_Ctrl_On(void)
{
    GPIO_SetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_L_PIN);
    GPIO_SetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_R_PIN);
}

void SweepRobot_Wheel_Float_Ctrl_Off(void)
{
    GPIO_ResetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_L_PIN);
    GPIO_ResetBits(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TEST_CTRL_R_PIN);
}

void SweepRobot_Wheel_Float_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static WHEEL_FLOAT_TestTypeDef lWheelFloat;
    static WHEEL_FLOAT_TestTypeDef rWheelFloat;
    u8 i;

    SweepRobot_Wheel_Float_Test_Init();

    while(1){
        gSwrbTestTaskCnt++;
        
        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_WHEEL_FLOAT_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>WHEEL FLOAT TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            SweepRobot_Wheel_Float_Ctrl_On();
            lWheelFloat.value = 0;
            lWheelFloat.validCnt = 0;
            lWheelFloat.validFlag = 0;
            rWheelFloat.value = 0;
            rWheelFloat.validCnt = 0;
            rWheelFloat.validFlag = 0;
        }
        
        if(gSwrbTestTaskCnt > 1){
            if(!lWheelFloat.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("WHEEL_FLOAT->READ=0\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        lWheelFloat.value = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                if(lWheelFloat.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_FLOAT_L_POS);
                    lWheelFloat.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_FLOAT_L_POS);
                    lWheelFloat.validCnt = 0;
                }
                if(lWheelFloat.validCnt > 5){
                    lWheelFloat.validFlag = 1;
                }
            }
            
            if(!rWheelFloat.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("WHEEL_FLOAT->READ=1\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        rWheelFloat.value = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U2, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                if(rWheelFloat.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_FLOAT_R_POS);
                    rWheelFloat.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_FLOAT_R_POS);
                    rWheelFloat.validCnt = 0;
                }
                if(rWheelFloat.validCnt > 5){
                    rWheelFloat.validFlag = 1;
                }
            }
            
            if(lWheelFloat.validFlag && rWheelFloat.validFlag){
                gSwrbTestTaskCnt = 0;
                SweepRobot_Wheel_Float_Ctrl_Off();
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                Checkbox_Set_State(ID_CHECKBOX_WHEEL_FLOAT, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL_FLOAT, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT OK");
                Progbar_Set_Value( (u8)( ( (float)(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

                OS_ENTER_CRITICAL();

#ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);
//                OSTaskDel(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }
    
        if(gSwrbTestTaskCnt > 20){
            gSwrbTestTaskCnt = 0;
            SweepRobot_Wheel_Float_Ctrl_Off();
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            MultiEdit_Set_Text_Color(GUI_RED);
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK){
                MultiEdit_Add_Text("ERROR->WHEEL FLOAT L\r\n");
            }
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK){
                MultiEdit_Add_Text("ERROR->WHEEL FLOAT R\r\n");
            }
            Checkbox_Set_State(ID_CHECKBOX_WHEEL_FLOAT, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL_FLOAT, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_WHEEL_FLOAT, "WHEEL FLOAT ERR");
            Progbar_Set_Value( (u8)( ( (float)(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

            OS_ENTER_CRITICAL();

#ifdef  SWRB_TEST_TASK_RUN_OBO
            if(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_WHEEL_FLOAT_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
//            OSTaskDel(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
