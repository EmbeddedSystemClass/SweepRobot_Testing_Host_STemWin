#include "sweepRobot_testing_fan.h"

#include "usart.h"
#include "includes.h"

#define SWRB_TEST_FAN_OC_THRESHOLD      1000
#define SWRB_TEST_FAN_CUR_LOW_BOUND     20
#define SWRB_TEST_FAN_CUR_HIGH_BOUND    500

void SweepRobot_Fan_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static FAN_TestTypeDef fan;
    u8 i;
    char *str;

    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_FAN_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            str = ">>>FAN TEST<<<\r\n";
            MultiEdit_Add_Text(str);
            mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
            mf_puts_with_offset(str);
            mf_close();
            OSTimeDlyHMSM(0,0,1,0);
            printf("FAN->SPEED=30\r\n");
            fan.current = 0;
            fan.validCnt = 0;
            fan.validFlag = 0;
        }

        if(gSwrbTestTaskCnt > 5){
            for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                printf("FAN->READ\r\n");
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    fan.current = usartRxNum;
                    Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                    /* TODO: add fan over current protection here */
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
            if(SWRB_TEST_FAN_CUR_LOW_BOUND < fan.current &&  SWRB_TEST_FAN_CUR_HIGH_BOUND > fan.current){
                gSwrbTestStateMap &= ~(1<<SWRB_TEST_FAN_POS);
                fan.validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<SWRB_TEST_FAN_POS);
                fan.validCnt = 0;
            }
            
            if(fan.validCnt > 5){
                fan.validFlag = 1;
                printf("FAN->SPEED=0\r\n");
            }

            if(fan.validFlag){
                gSwrbTestTaskCnt = 0;
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                gSwrbTestAcquiredData[SWRB_TEST_DATA_FAN_CUR_POS] = fan.current;
                Checkbox_Set_State(ID_CHECKBOX_FAN, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_FAN, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN OK");
                Progbar_Set_Value( (u8)(( (float)(SWRB_FAN_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

                OS_ENTER_CRITICAL();

                #ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_FAN_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_FAN_TEST_TASK_PRIO+1);
                #endif
                OSTaskSuspend(OS_PRIO_SELF);
                //        OSTaskDel(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }

    if(gSwrbTestTaskCnt > 20){
      gSwrbTestTaskCnt = 0;
      Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
      gSwrbTestAcquiredData[SWRB_TEST_DATA_FAN_CUR_POS] = fan.current;
      printf("FAN->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      MultiEdit_Add_Text("ERROR->FAN\r\n");
      Checkbox_Set_State(ID_CHECKBOX_FAN, 1);
      Checkbox_Set_Text_Color(ID_CHECKBOX_FAN, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN ERROR");
      Progbar_Set_Value( (u8)(( (float)(SWRB_FAN_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );

      OS_ENTER_CRITICAL();

#ifdef  SWRB_TEST_TASK_RUN_OBO
      if(SWRB_FAN_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
        OSTaskResume(SWRB_FAN_TEST_TASK_PRIO+1);
#endif
      OSTaskSuspend(OS_PRIO_SELF);
//      OSTaskDel(OS_PRIO_SELF);

      OS_EXIT_CRITICAL();
    }

    OSTimeDlyHMSM(0,0,0,50);
  }
}
