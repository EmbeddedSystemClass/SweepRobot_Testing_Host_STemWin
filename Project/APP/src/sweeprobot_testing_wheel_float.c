#include "sweepRobot_testing_wheel_float.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Wheel_Float_Test_Task(void *pdata)
{
  static u16 swrbWheelFloatTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("WHEEL FLOAT TEST START\r\n");
  
  while(1){
    printf("LBRUSH->READ\r\n");
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
