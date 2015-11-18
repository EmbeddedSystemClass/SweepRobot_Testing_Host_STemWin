#include "sweepRobot_testing_collision.h"

#include "usart.h"
#include "includes.h"

#define COLLISION_TEST_CTRL_RCC         RCC_AHB1Periph_GPIOB
#define COLLISION_TEST_CTRL_GPIO        GPIOB
#define COLLISION_TEST_CTRL_F_L_PIN     GPIO_Pin_6
#define COLLISION_TEST_CTRL_F_R_PIN     GPIO_Pin_7
#define COLLISION_TEST_CTRL_S_L_PIN     GPIO_Pin_10
#define COLLISION_TEST_CTRL_S_R_PIN     GPIO_Pin_11

#define COLLISION_CHAN_NUM  4

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
    static COLLISION_TestTypeDef collision[COLLISION_CHAN_NUM];
    u8 i;
    char *str;

    SweepRobot_Collision_Test_Init();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_COLLISION)){
            OS_ENTER_CRITICAL();
#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_COLLISION_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_COLLISION_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
            OS_EXIT_CRITICAL();
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
                MultiEdit_Set_Text_Color(GUI_BLACK);
                str = ">>>COLLISION TEST<<<\r\n";
                MultiEdit_Add_Text(str);
                mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
                mf_puts(str);
                OSTimeDlyHMSM(0,0,1,0);
                SweepRobot_Collision_Chan_On();
                for(i=0;i<COLLISION_CHAN_NUM;i++){
                    collision[i].value = 0;
                    collision[i].validCnt = 0;
                    collision[i].validFlag = 0;
                }
            }

            if(gSwrbTestTaskRunCnt > 1){
                for(i=0;i<COLLISION_CHAN_NUM;i++){
                    if(!collision[i].validFlag){
                        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                            printf("COLLISION->READ=%d\r\n",i);
                            OSTimeDlyHMSM(0,0,0,6);
                            if(usartRxFlag){
                                Edit_Set_Value(ID_EDIT_U1+i, usartRxNum);
                                collision[i].value = usartRxNum;
                                usartRxNum = 0;
                                usartRxFlag = 0;
                                break;
                            }else{
                                continue;
                            }
                        }

                        if(!collision[i].value){
                            gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+i));
                            collision[i].validCnt++;
                        }else{
                            gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
                            collision[i].validCnt = 0;
                        }

                        if(collision[i].validCnt > 5){
                            collision[i].validFlag = 1;
                        }
                    }
                }
                
                if(collision[0].validFlag && collision[1].validFlag && collision[2].validFlag && collision[3].validFlag){
                    gSwrbTestTaskRunCnt = 0;
                    SweepRobot_Collision_Chan_Off();
                    Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                    for(i=0;i<COLLISION_CHAN_NUM;i++){
                        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_VALUE_POS+i] = collision[i].value;
                    }
                    Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_BLUE);
                    Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION OK");
                    Progbar_Set_Value( (u8)( ( (float)(SWRB_COLLISION_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_TEST_TASK_PRIO_BOUND-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

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

            if(gSwrbTestTaskRunCnt > 20){
                gSwrbTestTaskRunCnt = 0;
                SweepRobot_Collision_Chan_Off();
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                for(i=0;i<COLLISION_CHAN_NUM;i++){
                    gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_VALUE_POS+i] = collision[i].value;
                }
                MultiEdit_Set_Text_Color(GUI_RED);
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK)
                    MultiEdit_Add_Text("ERROR->COLLISION_L\r\n");
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_L_MASK)
                    MultiEdit_Add_Text("ERROR->COLLISION_F_L\r\n");
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_R_MASK)
                    MultiEdit_Add_Text("ERROR->COLLISION_F_R\r\n");
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK)
                    MultiEdit_Add_Text("ERROR->COLLISION_R\r\n");
                Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_RED);
                Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION ERROR");
                Progbar_Set_Value( (u8)( ( (float)(SWRB_COLLISION_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_TEST_TASK_PRIO_BOUND-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

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
}
