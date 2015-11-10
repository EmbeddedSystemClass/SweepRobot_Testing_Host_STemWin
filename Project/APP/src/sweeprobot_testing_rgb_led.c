#include "sweepRobot_testing_rgb_led.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_RGB_LED_Test_Task(void *pdata)
{
  static u16 swrbRGBLEDTaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("RGB LED TEST START\r\n");
  
  while(1){
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
