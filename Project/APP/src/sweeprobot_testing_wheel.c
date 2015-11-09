#include "sweepRobot_testing_wheel.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_Testing_Wheel(void *pdata)
{
  u8 lwheel_speed = 0;
  u8 rwheel_speed = 0;
  
  printf("LWHEEL->SPEED=30\r\n");
  printf("RWHEEL->SPEED=30\r\n");
  
  while(1){
    printf("LWHEEL->READ\r\n");
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
