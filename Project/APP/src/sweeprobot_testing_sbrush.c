#include "sweepRobot_testing_sbrush.h"

#include "usart.h"
#include "includes.h"

static u16 swrbSBrushTestCnt = 0;

void SweepRobot_SBrush_Test_Task(void *pdata)
{
  printf("LBRUSH->SPEED=30\r\n");
  printf("RBRUSH->SPEED=30\r\n");
  
  while(1){
    swrbSBrushTestCnt++;
    
    if(swrbSBrushTestCnt > 20){
      printf("LBRUSH->SPEED=0\r\n");
      printf("RBRUSH->SPEED=0\r\n");
      Checkbox_Set_State(ID_CHECKBOX_BRUSH, 1);
      Checkbox_Set_Color(ID_CHECKBOX_BRUSH, GUI_BLUE);
      Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "SBRUSH OK");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_SBRUSH / (float)SWRB_TEST_STATE_BOUND)*100) );
      
      /* TODO: Resume next test task here */
      
      OSTaskDel(OS_PRIO_SELF);
    }
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
