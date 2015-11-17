#include "sweepRobot_testing_uniwheel.h"

#include "usart.h"
#include "includes.h"

#define SWRB_UNIWHEEL_VALID_THRESHOLD     120

void SweepRobot_UniWheel_Test_Task(void *pdata)
{
    static UNIWHEEL_TestTypeDef uniwheel;
    OS_CPU_SR cpu_sr;
    u8 i=0;
    char *str;
  
    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestRuningTaskPrio = SWRB_UNIWHEEL_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            str = ">>>UNIWHEEL TEST<<<\r\n";
            MultiEdit_Add_Text(str);
            mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
            mf_puts(str);
            mf_close();
            OSTimeDlyHMSM(0,0,1,0);
            printf("SENSOR->IFRD_LED=0\r\n");
            uniwheel.onValue = 0;
            uniwheel.offValue = 0;
            uniwheel.validCnt = 0;
            uniwheel.validFlag = 0;
        }
        
        if(!uniwheel.validFlag){
            if(gSwrbTestTaskCnt%2){
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("SENSOR->READ=8\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        uniwheel.offValue = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1, usartRxNum);
                        usartRxFlag = 0;
                        usartRxNum = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                printf("SENSOR->IFRD_LED=1\r\n");
            }else{
                for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                    printf("SENSOR->READ=8\r\n");
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        uniwheel.onValue = usartRxNum;
                        Edit_Set_Value(ID_EDIT_D1, usartRxNum);
                        usartRxFlag = 0;
                        usartRxNum = 0;
                        break;
                    }else{
                        continue;
                    }
                }
                printf("SENSOR->IFRD_LED=0\r\n");
          
                if(uniwheel.offValue - uniwheel.onValue > SWRB_UNIWHEEL_VALID_THRESHOLD){
                    gSwrbTestStateMap &= ~( (u32)1<<SWRB_TEST_UNIWHEEL_POS);
                    uniwheel.validCnt++;
                }else{
                    gSwrbTestStateMap |= ( (u32)1<<SWRB_TEST_UNIWHEEL_POS);
                    uniwheel.validCnt = 0;
                }
                
                if(uniwheel.validCnt > 5){
                    uniwheel.validFlag = 1;
                }
            }
        }

        if(uniwheel.validFlag){
            gSwrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOn_POS] = uniwheel.onValue;
            gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOff_POS] = uniwheel.offValue;
            printf("SENSOR->IFRD_LED=0\r\n");
            Checkbox_Set_State(ID_CHECKBOX_UNIWHEEL, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_UNIWHEEL, GUI_BLUE);
            Checkbox_Set_Text(ID_CHECKBOX_UNIWHEEL, "UNIWHEEL OK");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_UNIWHEEL / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

            OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_UNIWHEEL_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_UNIWHEEL_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
    
        if(gSwrbTestTaskCnt > 20){
            gSwrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOn_POS] = uniwheel.onValue;
            gSwrbTestAcquiredData[SWRB_TEST_DATA_UNIWHEEL_VALUE_TxOff_POS] = uniwheel.offValue;
            MultiEdit_Set_Text_Color(GUI_RED);
            MultiEdit_Add_Text("ERROR->UNIWHEEL\r\n");
            Checkbox_Set_State(ID_CHECKBOX_UNIWHEEL, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_UNIWHEEL, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_UNIWHEEL, "UNIWHEEL ERROR");
            Progbar_Set_Value( (u8)((float)( (float)SWRB_TEST_STATE_UNIWHEEL / (float)(SWRB_TEST_STATE_BOUND-1))*100) );

            OS_ENTER_CRITICAL();

    #ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_UNIWHEEL_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_UNIWHEEL_TEST_TASK_PRIO+1);
    #endif
            OSTaskSuspend(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
        OSTimeDlyHMSM(0,0,0,50);
    }
}
