#include "sweepRobot_testing_wheel.h"

#include "usart.h"
#include "includes.h"
#include "timer.h"

void SweepRobot_Wheel_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static WHEEL_TestTypeDef lWheel;
    static WHEEL_TestTypeDef rWheel;

    u8 i;

    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_WHEEL_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>WHEEL TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            printf("LWHEEL->SPEED=30\r\n");
            printf("RWHEEL->SPEED=30\r\n");
            lWheel.speed = 0;
            lWheel.validCnt = 0;
            lWheel.validFlag = 0;
            rWheel.speed = 0;
            rWheel.validCnt = 0;
            rWheel.validFlag = 0;
        }

        if(gSwrbTestTaskCnt > 4){
            if(!lWheel.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("LWHEEL->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        lWheel.speed = (u8)usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        Edit_Set_Value(ID_EDIT_U1, lWheel.speed);
                        break;
                    }else{
                        continue;
                    }
                }
                if(0<lWheel.speed && 50>lWheel.speed){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_L_STATE_POS);
                    lWheel.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_L_STATE_POS);
                    lWheel.validCnt = 0;
                }
                
                if(lWheel.validCnt > 5){
                    lWheel.validFlag = 1;
                    printf("LWHEEL->SPEED=0\r\n");
                }
            }

            if(!rWheel.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("RWHEEL->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        rWheel.speed = (u8)usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        Edit_Set_Value(ID_EDIT_D1, rWheel.speed);
                        break;
                    }else{
                        continue;
                    }
                }
                if(0<rWheel.speed && 50>rWheel.speed){
                    gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_R_STATE_POS);
                    rWheel.validCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_R_STATE_POS);
                    rWheel.validCnt = 0;
                }
                
                if(rWheel.validCnt > 5){
                    rWheel.validFlag = 1;
                    printf("RWHEEL->SPEED=0\r\n");
                }
            }
          
            if(lWheel.validFlag && rWheel.validFlag ){
                gSwrbTestTaskCnt = 0;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_L_SPEED_POS] = lWheel.speed;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_R_SPEED_POS] = rWheel.speed;
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                MultiEdit_Add_Text("WHEEL OK\r\n");
                Checkbox_Set_State(ID_CHECKBOX_WHEEL, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL OK");
                Progbar_Set_Value( (u8)(( (float)(SWRB_WHEEL_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

                OS_ENTER_CRITICAL();

    #ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_WHEEL_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_WHEEL_TEST_TASK_PRIO+1);
    #endif
                OSTaskSuspend(OS_PRIO_SELF);
                //        OSTaskDel(OS_PRIO_SELF);
                OS_EXIT_CRITICAL();
            }
        }

        if(20 < gSwrbTestTaskCnt){
            gSwrbTestTaskCnt = 0;
            gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_L_SPEED_POS] = lWheel.speed;
            gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_R_SPEED_POS] = rWheel.speed;
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            printf("LWHEEL->SPEED=0\r\n");
            printf("RWHEEL->SPEED=0\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_L_MASK)
                MultiEdit_Add_Text("ERROR->LEFT WHEEL\r\n");
            if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_R_MASK)
                MultiEdit_Add_Text("ERROR->RIGHT WHEEL\r\n");
            Checkbox_Set_State(ID_CHECKBOX_WHEEL, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL ERROR");
            Progbar_Set_Value( (u8)(( (float)(SWRB_WHEEL_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

            OS_ENTER_CRITICAL();

#ifdef  SWRB_TEST_TASK_RUN_OBO
            if(SWRB_WHEEL_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_WHEEL_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
//              OSTaskDel(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
        OSTimeDlyHMSM(0,0,0,50);
    }
}
