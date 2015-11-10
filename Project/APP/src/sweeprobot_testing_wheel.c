#include "sweepRobot_testing_wheel.h"

#include "usart.h"
#include "includes.h"
#include "timer.h"



void SweepRobot_Wheel_Test_Task(void *pdata)
{
  static u16 swrbWheelTestCnt = 0;
  static u8 lwheel_speed = 0;
  static u8 rwheel_speed = 0;
  
  u8 i;
  
  MultiEdit_Set_Text("\r\n");
  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("WHEEL TEST START\r\n");

  while(1){
    swrbWheelTestCnt++;

    printf("LWHEEL->SPEED=30\r\n");
    printf("RWHEEL->SPEED=30\r\n");

    if(swrbWheelTestCnt > 5){
      
      for(i=0;i<100;i++){
        printf("LWHEEL->READ\r\n");
        OSTimeDlyHMSM(0,0,0,10);
        if(usartRxFlag){
          lwheel_speed = (u8)usartRxNum;
          usartRxNum = 0;
          break;
        }else{
          continue;
        }
      }
      if(0 < lwheel_speed && 50 > lwheel_speed){
        swrbTestStateMap |= (1<<SWRB_TEST_WHEEL_L_STATE_POS);
      }else{
        swrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_L_STATE_POS);
      }
      
      for(i=0;i<100;i++){
        printf("RWHEEL->READ\r\n");
        OSTimeDlyHMSM(0,0,0,10);
        if(usartRxFlag){
          rwheel_speed = (u8)usartRxNum;
          usartRxNum = 0;
          break;
        }else{
          continue;
        }
      }
      if(0 < rwheel_speed && 50 > rwheel_speed){
        swrbTestStateMap |= (1<<SWRB_TEST_WHEEL_R_STATE_POS);
      }else{
        swrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_R_STATE_POS);
      }

      if( (swrbTestStateMap & SWRB_TEST_STATE_WHEEL_MASK) && (lastSwrbTestStateMap == swrbTestStateMap) ){
        swrbWheelTestCnt = 0;
        printf("LWHEEL->SPEED=0\r\n");
        printf("RWHEEL->SPEED=0\r\n");
        MultiEdit_Add_Text("WHEEL OK\r\n");
        Checkbox_Set_State(ID_CHECKBOX_WHEEL, 1);
        Checkbox_Set_Color(ID_CHECKBOX_WHEEL, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL OK");
        Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_WHEEL / (float)SWRB_TEST_STATE_BOUND)*100) );
        OSTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
        OSTaskDel(OS_PRIO_SELF);
      }
      lastSwrbTestStateMap = swrbTestStateMap;
    }

    if(40 < swrbWheelTestCnt){
      swrbWheelTestCnt = 0;
      printf("LWHEEL->SPEED=0\r\n");
      printf("RWHEEL->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      if(!(swrbTestStateMap & SWRB_TEST_STATE_WHEEL_L_MASK))
        MultiEdit_Add_Text("->LEFT WHEEL ERROR\r\n");
      if(!(swrbTestStateMap & SWRB_TEST_STATE_WHEEL_R_MASK))
        MultiEdit_Add_Text("->RIGHT WHEEL ERROR\r\n");
      Checkbox_Set_State(ID_CHECKBOX_WHEEL, 1);
      Checkbox_Set_Color(ID_CHECKBOX_WHEEL, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_WHEEL / (float)SWRB_TEST_STATE_BOUND)*100) );
      
      OSTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
      OSTaskDel(OS_PRIO_SELF);
    }

    OSTimeDlyHMSM(0,0,0,50);
  }
}
