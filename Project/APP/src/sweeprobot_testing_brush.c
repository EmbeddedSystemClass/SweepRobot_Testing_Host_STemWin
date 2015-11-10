#include "sweepRobot_testing_brush.h"

#include "usart.h"
#include "includes.h"

#define SWRB_TEST_LBRUSH_OC_THRESHOLD    500
#define SWRB_TEST_RBRUSH_OC_THRESHOLD    500
#define SWRB_TEST_MBRUSH_OC_THRESHOLD    500

void SweepRobot_Brush_Test_Task(void *pdata)
{
  static u16 swrbBrushTaskCnt = 0;
  static u16 lBrushCur = 0;
  static u16 rBrushCur = 0;
  static u16 mBrushCur = 0;
  u8 i;

  MultiEdit_Set_Text_Color(GUI_BLACK);
  MultiEdit_Add_Text("BRUSH TEST START\r\n");

  while(1){
    swrbBrushTaskCnt++;

    printf("LBRUSH->SPEED=30\r\n");
    printf("RBRUSH->SPEED=30\r\n");
    printf("MBRUSH->SPEED=30\r\n");
    
    if(swrbBrushTaskCnt > 5){
      
      for(i=0;i<100;i++){
        printf("LWHEEL->READ\r\n");
        OSTimeDlyHMSM(0,0,0,10);
        if(usartRxFlag){
          lBrushCur = usartRxNum;
          usartRxNum = 0;
          break;
        }else{
          continue;
        }
      }
      if( 20 < lBrushCur &&  SWRB_TEST_LBRUSH_OC_THRESHOLD > lBrushCur ){
        swrbTestStateMap |= (1<<SWRB_TEST_STATE_BRUSH_L_MASK);
      }else{
        swrbTestStateMap &= ~(1<<SWRB_TEST_STATE_BRUSH_L_MASK);
      }
      
      for(i=0;i<100;i++){
        printf("RWHEEL->READ\r\n");
        OSTimeDlyHMSM(0,0,0,10);
        if(usartRxFlag){
          lBrushCur = usartRxNum;
          usartRxNum = 0;
          break;
        }else{
          continue;
        }
      }
      if( 20 < rBrushCur &&  SWRB_TEST_RBRUSH_OC_THRESHOLD > rBrushCur ){
        swrbTestStateMap |= (1<<SWRB_TEST_STATE_BRUSH_R_MASK);
      }else{
        swrbTestStateMap &= ~(1<<SWRB_TEST_STATE_BRUSH_R_MASK);
      }
      
      for(i=0;i<100;i++){
        printf("MWHEEL->READ\r\n");
        OSTimeDlyHMSM(0,0,0,10);
        if(usartRxFlag){
          mBrushCur = usartRxNum;
          usartRxNum = 0;
          break;
        }else{
          continue;
        }
      }
      if( 20 < mBrushCur &&  SWRB_TEST_MBRUSH_OC_THRESHOLD > mBrushCur ){
        swrbTestStateMap |= (1<<SWRB_TEST_STATE_BRUSH_M_MASK);
      }else{
        swrbTestStateMap &= ~(1<<SWRB_TEST_STATE_BRUSH_M_MASK);
      }
      
      if( (swrbTestStateMap & SWRB_TEST_STATE_BRUSH_MASK) && (lastSwrbTestStateMap == swrbTestStateMap) ){
        
      }
      
      lastSwrbTestStateMap = swrbTestStateMap;
      
    }

    if(swrbBrushTaskCnt > 20){
      swrbBrushTaskCnt = 0;
      printf("LBRUSH->SPEED=0\r\n");
      printf("RBRUSH->SPEED=0\r\n");
      printf("MBRUSH->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      if( !(swrbTestStateMap & SWRB_TEST_STATE_BRUSH_L_MASK) ){
        MultiEdit_Add_Text("->LBRUSH ERROR\r\n");
      }
      if( !(swrbTestStateMap & SWRB_TEST_STATE_BRUSH_R_MASK) ){
        MultiEdit_Add_Text("->RBRUSH ERROR\r\n");
      }
      if( !(swrbTestStateMap & SWRB_TEST_STATE_BRUSH_M_MASK) ){
        MultiEdit_Add_Text("->MBRUSH ERROR\r\n");
      }
      Checkbox_Set_State(ID_CHECKBOX_BRUSH, 1);
      Checkbox_Set_Color(ID_CHECKBOX_BRUSH, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH ERROR");
      Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_BRUSH / (float)SWRB_TEST_STATE_BOUND)*100) );

      OSTaskResume(SWRB_FAN_TEST_TASK_PRIO);
      OSTaskDel(OS_PRIO_SELF);
    }

    OSTimeDlyHMSM(0,0,0,50);
  }
}
