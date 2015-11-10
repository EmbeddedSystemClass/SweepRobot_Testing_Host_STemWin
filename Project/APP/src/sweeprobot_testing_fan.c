#include "sweepRobot_testing_fan.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Fan_Test_Task(void *pdata)
{
  static u16 swrbFanTestCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("FAN TEST START\r\n");
  
  while(1){
    swrbFanTestCnt++;
    
    printf("FAN->SPEED=30\r\n");
    
    if(swrbFanTestCnt > 2){
      
    }
    
    if(swrbFanTestCnt > 20){
      printf("FAN->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      MultiEdit_Add_Text("FAN ERROR\r\n");
      Checkbox_Set_State(ID_CHECKBOX_FAN, 1);
      Checkbox_Set_Color(ID_CHECKBOX_FAN, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_FAN, "FAN ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_FAN / (float)SWRB_TEST_STATE_BOUND)*100) );
      
//     OSTaskResume(SWRB_IFRD_TEST_TASK_PRIO);
      OSTaskDel(OS_PRIO_SELF);
    }
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
