#include "sweepRobot_testing_ash_tray.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Ash_Tray_Test_Task(void *pdata)
{
  static u16 swrbAshTrayTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("ASH TRAY TEST START\r\n");
  
  while(1){
    
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
