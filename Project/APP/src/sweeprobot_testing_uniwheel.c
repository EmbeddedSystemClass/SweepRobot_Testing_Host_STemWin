#include "sweepRobot_testing_uniwheel.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_UniWheel_Test_Task(void *pdata)
{
  static u16 swrbUniWheelTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("UNIWHEEL TEST START\r\n");
  
  while(1){
    printf("LBRUSH->READ\r\n");
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
