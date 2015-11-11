#include "sweepRobot_testing_ifrd.h"

#include "usart.h"
#include "includes.h"

#define SWRB_IFRD_CHAN_NUM   8

static u16 SWRB_IFRD_VALID_THRESHOLD[SWRB_IFRD_CHAN_NUM] = { 800, 800, 250, 250, 150, 150, 150, 150 };

void SweepRobot_IFRD_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static u16 swrbTestTaskCnt = 0;
    static u16 gIFRDTxOffRxValue[SWRB_IFRD_CHAN_NUM+1] = {0};
    static u16 gIFRDTxOnRxValue[SWRB_IFRD_CHAN_NUM+1] = {0};

    int i,j;

    while(1){
        swrbTestTaskCnt++;

        if(swrbTestTaskCnt == 1){
            swrbTestStateMap &= ~SWRB_TEST_FAULT_IFRD_MASK;
            swrbTestRuningTaskPrio = SWRB_IFRD_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            MultiEdit_Add_Text(">>>IFRD TEST<<<\r\n");
            printf("SENSOR->IFRD_LED=0\r\n");
            printf("SENSOR->B_SWITCH=0\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            for(i=0;i<SWRB_IFRD_CHAN_NUM+1;i++){
                gIFRDTxOnRxValue[i] = 0;
                gIFRDTxOffRxValue[i] = 0;
            }
        }

        if(swrbTestTaskCnt%2){
            for(i=1;i<=6;i++){
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("SENSOR->READ=%d\r\n", i);
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        gIFRDTxOffRxValue[i] = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1+i-1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
            }
            printf("SENSOR->B_SWITCH=1\r\n");
            OSTimeDlyHMSM(0,0,0,1);
            for(i=7;i<=8;i++){
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("SENSOR->READ=%d\r\n", i-2);
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        gIFRDTxOffRxValue[i] = usartRxNum;
                        Edit_Set_Value(ID_EDIT_U1+i-1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
            }
            printf("SENSOR->B_SWITCH=0\r\n");
            printf("SENSOR->IFRD_LED=1\r\n");
        }else{
            for(i=1;i<=6;i++){
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("SENSOR->READ=%d\r\n", i);
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        gIFRDTxOnRxValue[i] = usartRxNum;
                        Edit_Set_Value(ID_EDIT_D1+i-1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
            }
            printf("SENSOR->B_SWITCH=1\r\n");
            OSTimeDlyHMSM(0,0,0,1);
            for(i=7;i<=8;i++){
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("SENSOR->READ=%d\r\n", i-2);
                    OSTimeDlyHMSM(0,0,0,6);
                    if(usartRxFlag){
                        gIFRDTxOnRxValue[i] = usartRxNum;
                        Edit_Set_Value(ID_EDIT_D1+i-1, usartRxNum);
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        break;
                    }else{
                        continue;
                    }
                }
            }

            printf("SENSOR->B_SWITCH=0\r\n");
            printf("SENSOR->IFRD_LED=0\r\n");

            for(i=1;i<=SWRB_IFRD_CHAN_NUM;i++){
                if( (gIFRDTxOffRxValue[i] - gIFRDTxOnRxValue[i]) > SWRB_IFRD_VALID_THRESHOLD[i-1] ){
                    swrbTestStateMap &= ~(((u32)0)<<(SWRB_TEST_IFRD_F_L_POS+i-1));
                }else{
                    swrbTestStateMap |= (((u32)1)<<(SWRB_TEST_IFRD_F_L_POS+i-1));
                }
            }

            if( !(swrbTestStateMap & SWRB_TEST_FAULT_IFRD_MASK) && (lastSwrbTestStateMap == swrbTestStateMap) ){
                swrbTestTaskCnt = 0;
                Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
                printf("SENSOR->IFRD_LED=0\r\n");
                Checkbox_Set_State(ID_CHECKBOX_IFRD, 1);
                Checkbox_Set_Text_Color(ID_CHECKBOX_IFRD, GUI_BLUE);
                Checkbox_Set_Text(ID_CHECKBOX_IFRD, "IFRD OK");
                Progbar_Set_Value( (u8)((float)( (SWRB_FAN_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );
                for(i=ID_EDIT_1;i<ID_EDIT_BOUND;i++){
                    Edit_Set_Value(i, 0);
                }

                OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
                if(SWRB_IFRD_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                    OSTaskResume(SWRB_IFRD_TEST_TASK_PRIO+1);
#endif
                OSTaskSuspend(OS_PRIO_SELF);
//                OSTaskDel(OS_PRIO_SELF);
                OS_EXIT_CRITICAL();
            }
            lastSwrbTestStateMap = swrbTestStateMap;
        }

        if(swrbTestTaskCnt > 50){
            swrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, swrbTestStateMap);
            printf("SENSOR->IFRD_LED=0\r\n");
            printf("SENSOR->B_SWITCH=0\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_F_L_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_F_L\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_F_R_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_F_R\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_S_L_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_S_L\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_S_R_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_S_R\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FL_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_FL\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FR_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_FR\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SL_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_SL\r\n");
            if( swrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SR_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_SR\r\n");
            Checkbox_Set_State(ID_CHECKBOX_IFRD, 1);
            Checkbox_Set_Text_Color(ID_CHECKBOX_IFRD, GUI_RED);
            Checkbox_Set_Text(ID_CHECKBOX_IFRD, "IFRD ERROR");
            Progbar_Set_Value( (u8)((float)( (SWRB_FAN_TEST_TASK_PRIO-3) / (float)(SWRB_TEST_TASK_PRIO_BOUND-4))*100) );
            for(i=ID_EDIT_1;i<ID_EDIT_BOUND;i++){
                Edit_Set_Value(i, 0);
            }

            OS_ENTER_CRITICAL();

#ifdef SWRB_TEST_TASK_RUN_OBO
            if(SWRB_IFRD_TEST_TASK_PRIO+1 < SWRB_TEST_TASK_PRIO_BOUND)
                OSTaskResume(SWRB_IFRD_TEST_TASK_PRIO+1);
#endif
            OSTaskSuspend(OS_PRIO_SELF);
//            OSTaskDel(OS_PRIO_SELF);

            OS_EXIT_CRITICAL();
        }
        OSTimeDlyHMSM(0,0,0,20);
    }
}
