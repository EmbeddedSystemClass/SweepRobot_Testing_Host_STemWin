#include "sweepRobot_testing_collision.h"

#include "usart.h"
#include "includes.h"

#define COLLISION_TEST_CTRL_RCC         RCC_AHB1Periph_GPIOB
#define COLLISION_TEST_CTRL_GPIO        GPIOB
#define COLLISION_TEST_CTRL_F_L_PIN     GPIO_Pin_6
#define COLLISION_TEST_CTRL_F_R_PIN     GPIO_Pin_7
#define COLLISION_TEST_CTRL_S_L_PIN     GPIO_Pin_10
#define COLLISION_TEST_CTRL_S_R_PIN     GPIO_Pin_11

void SweepRobot_Collision_Test_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(COLLISION_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = COLLISION_TEST_CTRL_F_L_PIN |\
                                  COLLISION_TEST_CTRL_F_R_PIN |\
                                  COLLISION_TEST_CTRL_S_L_PIN |\
                                  COLLISION_TEST_CTRL_S_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Collision_Chan_On(void)
{
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_L_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_R_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_L_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_R_PIN);
}

void SweepRobot_Collision_Chan_Off(void)
{
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_L_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_R_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_L_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_R_PIN);
}

void SweepRobot_Collision_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static COLLISION_TestTypeDef lCollision;
    static COLLISION_TestTypeDef flCollision;
    static COLLISION_TestTypeDef rCollision;
    static COLLISION_TestTypeDef frCollision;
    u8 i;

    SweepRobot_Collision_Test_Init();

    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>COLLISION TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            SweepRobot_Collision_Chan_On();
            lCollision.value = 0;
            lCollision.validCnt = 0;
            lCollision.validFlag = 0;
            flCollision.value = 0;
            flCollision.validCnt = 0;
            flCollision.validFlag = 0;
            rCollision.value = 0;
            rCollision.validCnt = 0;
            rCollision.validFlag = 0;
            frCollision.value = 0;
            frCollision.validCnt = 0;
            frCollision.validFlag = 0;
        }

        if(gSwrbTestStateMap > 1){
            if(!lCollision.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("COLLISION->READ=0\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        lCollision.value = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(!lCollision.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_COLLISION_L_POS);
                    lCollision.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_COLLISION_L_POS);
                    lCollision.validCnt = 0;
                }

                if(lCollision.validCnt > 5){
                    lCollision.validFlag = 1;
                }
            }

            if(!flCollision.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("COLLISION->READ=1\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        Edit_Set_Value(ID_EDIT_U2, usartRxNum);
                        flCollision.value = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(!flCollision.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_COLLISION_F_L_POS);
                    flCollision.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_COLLISION_F_L_POS);
                    flCollision.validCnt = 0;
                }

                if(flCollision.validCnt > 5){
                    flCollision.validFlag = 1;
                }
            }

            if(!rCollision.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("COLLISION->READ=2\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        rCollision.value = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(!rCollision.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_COLLISION_R_POS);
                    rCollision.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_COLLISION_R_POS);
                    rCollision.validCnt = 0;
                }

                if(rCollision.validCnt > 5){
                    rCollision.validFlag = 1;
                }
            }

            if(!frCollision.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("COLLISION->READ=3\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        frCollision.value = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(!frCollision.value){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_COLLISION_F_R_POS);
                    frCollision.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_COLLISION_F_R_POS);
                    frCollision.validCnt = 0;
                }

                if(frCollision.validCnt > 5){
                    frCollision.validFlag = 1;
                }
            }
            
            if(lCollision.validFlag && flCollision.validFlag && rCollision.validFlag && frCollision.validFlag){
                gSwrbTestTaskCnt = 0;
                SweepRobot_Collision_Chan_Off();
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                Checkbox_Set_State(ID_CHECKBOX_COLLISION, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION OK");
                Progbar_Set_Value( (u8)( ( (float)(SWRB_COLLISION_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

                OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_COLLISION_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_COLLISION_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);
//                OSTaskDel(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }

        if(gSwrbTestTaskCnt > 20){
            gSwrbTestTaskCnt = 0;
            SweepRobot_Collision_Chan_Off();
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            MultiEdit_Set_Text_Color(GUI_RED);
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK)
                MultiEdit_Add_Text("ERROR->COLLISION_L\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_L_MASK)
                MultiEdit_Add_Text("ERROR->COLLISION_F_L\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_R_MASK)
                MultiEdit_Add_Text("ERROR->COLLISION_F_R\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK)
                MultiEdit_Add_Text("ERROR->COLLISION_R\r\n");
            Checkbox_Set_State(ID_CHECKBOX_COLLISION, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION ERROR");
            Progbar_Set_Value( (u8)( ( (float)(SWRB_COLLISION_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

            OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_COLLISION_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_COLLISION_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
//            OSTaskDel(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }

    OSTimeDlyHMSM(0,0,0,50);
  }
}
