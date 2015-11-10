#include "sweepRobot_testing_collision.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Collision_Test_Task(void *pdata)
{
  static u16 swrbCollisionTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("COLLISION TEST START\r\n");
  
  while(1){
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
