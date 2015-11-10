#include "sweepRobot_testing_buzzer.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Buzzer_Test_Task(void *pdata)
{
  static u16 swrbBuzzerTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("BUZZER TEST START\r\n");
  
  while(1){
    swrbBuzzerTaskCnt++;
    
    
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
