#include "sweepRobot_testing_mbrush.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_MBrush_Test_Task(void *pdata)
{
  while(1){
    printf("LBRUSH->READ\r\n");
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
