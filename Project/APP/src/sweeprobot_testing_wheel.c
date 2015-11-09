#include "sweepRobot_testing_wheel.h"

#include "usart.h"
#include "includes.h"

static u8 lwheel_speed = 0;
static u8 rwheel_speed = 0;

static u16 swrb_wheel_cnt = 0;

void SweepRobot_Testing_Wheel(void *pdata)
{
  u8 tmpflag;
  
  printf("LWHEEL->SPEED=30\r\n");
  printf("RWHEEL->SPEED=30\r\n");
  
  while(1){
    swrb_wheel_cnt++;
    printf("LWHEEL->READ\r\n");
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET);
    while(!usartRxFlag);
    lwheel_speed = (u8)usartRxNum;
    printf("RWHEEL->READ\r\n");
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET);
    while(!usartRxFlag);
    rwheel_speed = (u8)usartRxNum;
    
    if(0 < lwheel_speed && 10 > lwheel_speed){
      MultiEdit_Add_Text("LWHEEL OK\r\n");
    }
    if(0 < rwheel_speed && 10 > rwheel_speed){
      MultiEdit_Add_Text("RWHEEL OK\r\n");
    }
    
    if(swrb_wheel_cnt > 2000){
      MultiEdit_Set_Text("WHEEL ERROR\r\n");
      Checkbox_Set_Color(ID_CHECKBOX_0, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_0, "WHEEL ERROR");
      break;
    }
  }
}
