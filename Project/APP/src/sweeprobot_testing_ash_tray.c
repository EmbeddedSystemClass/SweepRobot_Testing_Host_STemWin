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
    
    SweepRobot_Ash_Tray_Test_Ctrl_Init();

    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_ASH_TRAY_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>ASH TRAY TEST<<<\r\n");
            printf("SENSOR->IFRD_LED=0\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            SweepRobot_Ash_Tray_Test_Ins_Ctrl_On();
        }
        
        if(gSwrbTestTaskCnt > 1){
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
            if(gSwrbTestTaskCnt%2){
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
            gSwrbTestTaskCnt = 0;
            SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off();
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            printf("SENSOR->IFRD_LED=0\r\n");
            Checkbox_Set_State(ID_CHECKBOX_ASH_TRAY, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_BLUE);
            Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY OK");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_ASH_TRAY / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

            OS_ENTER_CRITICAL();

            
#ifdef  SWRB_TEST_TASK_RUN_OBO
            if(SWRB_ASH_TRAY_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_ASH_TRAY_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }

        if(gSwrbTestTaskCnt > 20){
            gSwrbTestTaskCnt = 0;
            SweepRobot_Ash_Tray_Test_Ins_Ctrl_Off();
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            MultiEdit_Set_Text_Color(GUI_RED);
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_INS_MASK)
                MultiEdit_Add_Text("ERROR->ASH TRAY INS\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK)
                MultiEdit_Add_Text("ERROR->ASH TRAY LVL\r\n");
            Checkbox_Set_State(ID_CHECKBOX_ASH_TRAY, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_ASH_TRAY, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_ASH_TRAY, "ASH TRAY ERROR");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_ASH_TRAY / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

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
