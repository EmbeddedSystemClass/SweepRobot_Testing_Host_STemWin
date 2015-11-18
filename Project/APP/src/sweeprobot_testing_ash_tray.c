#include "sweepRobot_testing_ash_tray.h"

#include "usart.h"
#include "includes.h"

#define ASH_TRAY_TEST_CTRL_RCC       RCC_AHB1Periph_GPIOE
#define ASH_TRAY_TEST_CTRL_GPIO      GPIOE
#define ASH_TRAY_TEST_CTRL_PIN     GPIO_Pin_5

#define SWRB_ASH_TRAY_LVL_VALID_THRESHOLD   120

void SweepRobot_Ash_Tray_Test_Ctrl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ASH_TRAY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ASH_TRAY_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Ash_Tray_Test_Ins_Ctrl_On(void)
{
    GPIO_SetBits(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN);
}

void SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off(void)
{
    GPIO_ResetBits(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN);
}

void SweepRobot_Ash_Tray_Test_Task(void *pdata)
{
    static ASH_TRAY_TestTypeDef ashTrayIns;
    static ASH_TRAY_TestTypeDef ashTrayLvl;
    OS_CPU_SR cpu_sr;

    u8 i;
    char *str;
    
    SweepRobot_Ash_Tray_Test_Ctrl_Init();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_ASH_TRAY)){
            OS_ENTER_CRITICAL();
#ifdef  SWRB_TEST_TASK_RUN_OBO
            if(SWRB_ASH_TRAY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
            OS_EXIT_CRITICAL();
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                gSwrbTestRuningTaskPrio = SWRB_ASH_TRAY_TEST_TASK_PRIO;
                MultiEdit_Set_Text_Color(GUI_BLACK);
                str = ">>>ASH TRAY TEST<<<\r\n";
                MultiEdit_Add_Text(str);
                mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
                mf_puts(str);
                printf("SENSOR->IFRD_LED=0\r\n");
                OSTimeDlyHMSM(0,0,1,0);
                SweepRobot_Ash_Tray_Test_Ins_Ctrl_On();
            }
            
            if(gSwrbTestTaskRunCnt > 1){
                if(!ashTrayIns.validFlag){
                    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                        printf("WHEEL_FLOAT->READ=0\r\n");
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            ashTrayIns.value = usartRxNum;
                            Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                            usartRxNum = 0;
                            usartRxFlag = 0;
                            break;
                        }else{
                            continue;
                        }
                    }
                    if(ashTrayIns.value){
                        gSwrbTestStateMap &= ~(1<<SWRB_TEST_ASH_TRAY_INS_POS);
                        ashTrayIns.validCnt++;
                    }else{
                        gSwrbTestStateMap |= (1<<SWRB_TEST_ASH_TRAY_INS_POS);
                        ashTrayIns.validCnt = 0;
                    }
                    if(ashTrayIns.validCnt > 5){
                        ashTrayIns.validFlag = 1;
                    }
                    
                    if(ashTrayIns.validFlag){
                        SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off();
                    }
                }
            }
        
            if(!ashTrayLvl.validFlag){
                if(gSwrbTestTaskRunCnt%2){
                    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                        printf("SENSOR->READ=15\r\n");
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            ashTrayLvl.offValue = usartRxNum;
                            Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                            usartRxFlag=0;
                            usartRxNum=0;
                            break;
                        }else{
                            continue;
                        }
                    }
                    printf("SENSOR->IFRD_LED=1\r\n");

                }else{
                    for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                        printf("SENSOR->READ=15\r\n");
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            ashTrayLvl.onValue = usartRxNum;
                            Edit_Set_Value(ID_EDIT_D1, usartRxNum);
                            usartRxFlag = 0;
                            usartRxNum = 0;
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
                        ashTrayLvl.validCnt = 0;
                    }
                    
                    if(ashTrayLvl.validCnt > 5){
                        ashTrayLvl.validFlag = 1;
                    }
                }
            }

            if(ashTrayIns.validFlag && ashTrayLvl.validFlag){
                gSwrbTestTaskRunCnt = 0;
                SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off();
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
                printf("SENSOR->IFRD_LED=0\r\n");
                Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY OK");
                Progbar_Set_Value( (u8)((float)( (float)(SWRB_ASH_TRAY_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_TEST_TASK_PRIO_BOUND-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

                OS_ENTER_CRITICAL();

                
    #ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_ASH_TRAY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO+1);
    #endif
                OSTaskSuspend(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }

            if(gSwrbTestTaskRunCnt > 20){
                gSwrbTestTaskRunCnt = 0;
                SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off();
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_INS_VALUE_POS] = ashTrayIns.value;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOn_POS] = ashTrayLvl.onValue;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_ASH_TRAY_LVL_VALUE_TxOff_POS] = ashTrayLvl.offValue;
                MultiEdit_Set_Text_Color(GUI_RED);
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK)
                    MultiEdit_Add_Text("ERROR->ASH TRAY INS\r\n");
                if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK)
                    MultiEdit_Add_Text("ERROR->ASH TRAY LVL\r\n");
                Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_RED);
                Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY ERROR");
                Progbar_Set_Value( (u8)((float)( (float)(SWRB_ASH_TRAY_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_TEST_TASK_PRIO_BOUND-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

                OS_ENTER_CRITICAL();

    #ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_ASH_TRAY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO+1);
    #endif
                OSTaskSuspend(OS_PRIO_SELF);
    //            OSTaskDel(OS_PRIO_SELF);
              
                OS_EXIT_CRITICAL();
            }
            
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}
