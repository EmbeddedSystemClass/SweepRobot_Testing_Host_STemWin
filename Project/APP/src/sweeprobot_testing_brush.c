#include "sweepRobot_testing_brush.h"

#include "usart.h"
#include "includes.h"

#define SWRB_TEST_LBRUSH_OC_THRESHOLD       300
#define SWRB_TEST_RBRUSH_OC_THRESHOLD       300
#define SWRB_TEST_MBRUSH_OC_THRESHOLD       1000

#define SWRB_TEST_LBRUSH_CUR_LOW_BOUND      5
#define SWRB_TEST_LBRUSH_CUR_HIGH_BOUND     50
#define SWRB_TEST_RBRUSH_CUR_LOW_BOUND      5
#define SWRB_TEST_RBRUSH_CUR_HIGH_BOUND     50
#define SWRB_TEST_MBRUSH_CUR_LOW_BOUND      50
#define SWRB_TEST_MBRUSH_CUR_HIGH_BOUND     500

void SweepRobot_Brush_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static BRUSH_TestTypeDef lBrush;
    static BRUSH_TestTypeDef rBrush;
    static BRUSH_TestTypeDef mBrush;

    u8 i;
  
    while(1){
        swrbTestTaskCnt++;

        if(swrbTestTaskCnt == 1){
            swrbTestRuningTaskPrio = SWRB_BRUSH_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>BRUSH TEST<<<\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            printf("LBRUSH->SPEED=30\r\n");
            printf("RBRUSH->SPEED=30\r\n");
            printf("MBRUSH->SPEED=30\r\n");
            lBrush.current = 0;
            lBrush.validCnt = 0;
            lBrush.validFlag = 0;
            rBrush.current = 0;
            rBrush.validCnt = 0;
            rBrush.validFlag = 0;
        }

        if(swrbTestTaskCnt > 5){
            if(!lBrush.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("LBRUSH->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        lBrush.current = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                if(SWRB_TEST_LBRUSH_CUR_LOW_BOUND<lBrush.current && SWRB_TEST_LBRUSH_CUR_HIGH_BOUND>lBrush.current){
                    swrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_L_STATE_POS);
                    lBrush.validCnt++;
                }else{
                    swrbTestStateMap |= (1<<SWRB_TEST_BRUSH_L_STATE_POS);
                    lBrush.validCnt=0;
                }
                
                if( lBrush.validCnt > 5){
                    lBrush.validFlag = 1;
                    printf("LBRUSH->SPEED=0\r\n");
                }
            }
            
            if(!rBrush.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("RBRUSH->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        lBrush.current = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U2, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                if(SWRB_TEST_RBRUSH_CUR_LOW_BOUND<rBrush.current && SWRB_TEST_RBRUSH_CUR_HIGH_BOUND>rBrush.current){  
                    swrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_R_STATE_POS);
                    rBrush.validCnt++;
                }else{
                    swrbTestStateMap |= (1<<SWRB_TEST_BRUSH_R_STATE_POS);
                    rBrush.validCnt=0;
                }
                
                if(rBrush.validCnt > 5){
                    rBrush.validFlag = 1;
                    printf("RBRUSH->SPEED=0\r\n");
                }
            }
            
            if(!mBrush.validFlag){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("MBRUSH->READ\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        mBrush.current = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U3, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                if(SWRB_TEST_MBRUSH_CUR_LOW_BOUND<mBrush.current &&  SWRB_TEST_MBRUSH_CUR_HIGH_BOUND>mBrush.current){
                    swrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_M_STATE_POS);
                    mBrush.validCnt++;
                }else{
                    swrbTestStateMap |= (1<<SWRB_TEST_BRUSH_M_STATE_POS);
                    mBrush.validCnt=0;
                }
                
                if(mBrush.validCnt > 5){
                    mBrush.validFlag = 1;
                    printf("MBRUSH->SPEED=0\r\n");
                }
            }

            if(lBrush.validFlag && rBrush.validFlag && mBrush.validFlag){
                swrbTestTaskCnt = 0;
                swrbTestAcquiredData[SWRB_TEST_BRUSH_L_STATE_POS] = lBrush.current;
                swrbTestAcquiredData[SWRB_TEST_BRUSH_R_STATE_POS] = rBrush.current;
                swrbTestAcquiredData[SWRB_TEST_BRUSH_M_STATE_POS] = mBrush.current;
                Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
                Checkbox_Set_State(ID_CHECKBOX_BRUSH, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_BRUSH, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH OK");
                Progbar_Set_Value( (u8)(( (float)(SWRB_BRUSH_TEST_TASK_PRIO-3) / (float)(SWRB_BRUSH_TEST_TASK_PRIO-4))*100) );

                OS_ENTER_CRITICAL();

#ifdef  SWRB_TEST_TASK_RUN_OBO
                if(SWRB_BRUSH_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_BRUSH_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);
                //        OSTaskDel(OS_PRIO_SELF);

                OS_EXIT_CRITICAL();
            }
        }

    if(swrbTestTaskCnt > 20){
      swrbTestTaskCnt = 0;
      Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
      printf("LBRUSH->SPEED=0\r\n");
      printf("RBRUSH->SPEED=0\r\n");
      printf("MBRUSH->SPEED=0\r\n");
      MultiEdit_Set_Text_Color(GUI_RED);
      if(swrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        MultiEdit_Add_Text("ERROR->LBRUSH\r\n");
      }
      if(swrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        MultiEdit_Add_Text("ERROR->RBRUSH\r\n");
      }
      if(swrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        MultiEdit_Add_Text("ERROR->MBRUSH\r\n");
      }
      Checkbox_Set_State(ID_CHECKBOX_BRUSH, 1);
      Checkbox_Set_Text_Color(ID_CHECKBOX_BRUSH, GUI_RED);
      Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH ERROR");
      Progbar_Set_Value( (u8)(( (float)(SWRB_BRUSH_TEST_TASK_PRIO-3) / (float)(SWRB_BRUSH_TEST_TASK_PRIO-4))*100) );

      OS_ENTER_CRITICAL();
      
    #ifdef  SWRB_TEST_TASK_RUN_OBO
      if(SWRB_BRUSH_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
        OSTaskResume(SWRB_BRUSH_TEST_TASK_PRIO+1);
    #endif
      OSTaskSuspend(OS_PRIO_SELF);
    //      OSTaskDel(OS_PRIO_SELF);
      
      OS_EXIT_CRITICAL();
    }

    OSTimeDlyHMSM(0,0,0,50);
    }
}
