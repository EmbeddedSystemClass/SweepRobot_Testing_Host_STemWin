#include "sweepRobot_testing_buzzer.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Buzzer_Test_Task(void *pdata)
{
    static u16 gSwrbTestTaskRunCnt = 0;
    OS_CPU_SR cpu_sr;
    char *str;
  
    while(1){
      
        if(!Checkbox_Get_State(ID_CHECKBOX_BUZZER)){
            OS_ENTER_CRITICAL();
#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_BUZZER_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_BUZZER_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
            OS_EXIT_CRITICAL();
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                gSwrbTestRuningTaskPrio = SWRB_BUZZER_TEST_TASK_PRIO;
                MultiEdit_Set_Text_Color(GUI_BLACK);
                str = ">>>BUZZER TEST<<<\r\n";
                MultiEdit_Add_Text(str);
                mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
                mf_puts(str);
                OSTimeDlyHMSM(0,0,2,0);
            }

            if(gSwrbTestTaskRunCnt == 20){
                printf("BUZZER->ON=1\r\n");
            }else if(gSwrbTestTaskRunCnt == 60){
                printf("BUZZER->ON=2\r\n");
            }else if(gSwrbTestTaskRunCnt == 100){
                printf("BUZZER->ON=3\r\n");
            }

            if(gSwrbTestTaskRunCnt > 140){
                gSwrbTestTaskRunCnt = 0;
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);

                MultiEdit_Set_Text_Color(GUI_RED);
                MultiEdit_Add_Text("ERROR->BUZZER\r\n");
                Checkbox_Set_Text_Color(ID_CHECKBOX_BUZZER, GUI_RED);
                Checkbox_Set_Text(ID_CHECKBOX_BUZZER, "BUZZER ERROR");
                Progbar_Set_Value( (u8)((float)( (float)(SWRB_BUZZER_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM) / (float)(SWRB_BUZZER_TEST_TASK_PRIO-SWRB_TEST_TASK_PRIO_BOUND_MINUS_NUM))*100) );

                OS_ENTER_CRITICAL();

    #ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_BUZZER_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_BUZZER_TEST_TASK_PRIO+1);
    #endif
                OSTaskSuspend(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }

            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}
