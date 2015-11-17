#include "sweepRobot_testing_rgb_led.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_RGB_LED_Test_Task(void *pdata)
{
  static u16 gSwrbTestTaskCnt = 0;
  OS_CPU_SR cpu_sr;
  char *str;
  
  while(1){
    gSwrbTestTaskCnt++;
      
    if(gSwrbTestTaskCnt ==1){
        gSwrbTestRuningTaskPrio = SWRB_RGB_LED_TEST_TASK_PRIO;
        MultiEdit_Set_Text_Color(GUI_BLACK);
        str = ">>>RGB LED TEST<<<\r\n";
        MultiEdit_Add_Text(str);
        mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
        mf_puts(str);
        mf_close();
        OSTimeDlyHMSM(0,0,2,0);
    }
    
    if(gSwrbTestTaskCnt == 20){
      printf("RGB_LED->ON=1\r\n");
    }else if(gSwrbTestTaskCnt == 60){
      printf("RGB_LED->ON=2\r\n");
    }else if(gSwrbTestTaskCnt == 100){
      printf("RGB_LED->ON=4\r\n");
    }
    
    if(gSwrbTestTaskCnt > 140){
      gSwrbTestTaskCnt = 0;
      Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
      if(gSwrbTestStateMap){
        printf("RGB_LED->ON=1\r\n");
      }else{
        printf("RGB_LED->ON=2\r\n");
      }
      MultiEdit_Set_Text_Color(GUI_RED);
      if( !(gSwrbTestStateMap & SWRB_TEST_FAULT_RGB_LED_MASK) )
        MultiEdit_Add_Text("ERROR->RGB LED\r\n");
      Checkbox_Set_State(ID_CHECKBOX_RGB_LED, 1);
      Checkbox_Set_Text_Color(ID_CHECKBOX_RGB_LED, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_RGB_LED, "RGB LED ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_RGB_LED / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

      OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
      if(SWRB_RGB_LED_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
        OSTaskResume(SWRB_RGB_LED_TEST_TASK_PRIO+1);
#endif
      OSTaskSuspend(OS_PRIO_SELF);
      
      OS_EXIT_CRITICAL();
    }
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
