#include "sweepRobot_testing_buzzer.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Buzzer_Test_Task(void *pdata)
{
  static u16 swrbTestTaskCnt = 0;
  OS_CPU_SR cpu_sr;
  
  swrbTestRuningTaskPrio = SWRB_BUZZER_TEST_TASK_PRIO;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text(">>>BUZZER TEST<<<\r\n");
  OSTimeDlyHMSM(0,0,2,0);
  
  while(1){
    swrbTestTaskCnt++;
    
    if(swrbTestTaskCnt == 20){
      printf("BUZZER->ON=1\r\n");
    }else if(swrbTestTaskCnt == 60){
      printf("BUZZER->ON=2\r\n");
    }else if(swrbTestTaskCnt == 100){
      printf("BUZZER->ON=3\r\n");
    }
    
    if(swrbTestTaskCnt > 140){
      swrbTestTaskCnt = 0;
      Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
      MultiEdit_Set_Text_Color(GUI_RED);
      MultiEdit_Add_Text("ERROR->BUZZER\r\n");
      Checkbox_Set_State(ID_CHECKBOX_BUZZER, 1);
      Checkbox_Set_Text_Color(ID_CHECKBOX_BUZZER, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_BUZZER, "BUZZER ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_BUZZER / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

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
