#include "sweepRobot_testing_ifrd.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_IFRD_Test_Task(void *pdata)
{
  static u16 swrbIFRDTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("IFRD TEST START\r\n");
  
  while(1){
    swrbIFRDTaskCnt++;
    
    printf("IFRD->ON\r\n");
    
    if(swrbIFRDTaskCnt > 2){
      
    }
    
    if(swrbIFRDTaskCnt > 20){
      printf("FAN->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      MultiEdit_Add_Text("IFRD ERROR\r\n");
      Checkbox_Set_State(ID_CHECKBOX_IFRD, 1);
      Checkbox_Set_Color(ID_CHECKBOX_IFRD, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_IFRD, "IFRD ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_IFRD / (float)SWRB_TEST_STATE_BOUND)*100) );
      
      OSTaskResume(SWRB_COLLISION_TEST_TASK_PRIO);
      OSTaskDel(OS_PRIO_SELF);
    }
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
