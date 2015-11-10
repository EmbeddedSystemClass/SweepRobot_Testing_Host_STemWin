#include "sweepRobot_testing_irda.h"

#include "usart.h"
#include "includes.h"

void SweepRobot_IrDA_Test_Task(void *pdata)
{
  static u16 swrbIrDATaskCnt = 0;
  
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("IRDA TEST START\r\n");
  
  while(1){
    printf("LBRUSH->READ\r\n");
    
    OSTimeDlyHMSM(0,0,0,50);
  }
}
