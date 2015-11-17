#include "sweepRobot_testing_ifrd.h"

#include "usart.h"
#include "includes.h"

#define SWRB_IFRD_CHAN_NUM   8

static u16 SWRB_IFRD_VALID_THRESHOLD[SWRB_IFRD_CHAN_NUM] = { 800, 800, 250, 250, 150, 150, 150, 150 };

void SweepRobot_IFRD_Test_Task(void *pdata)
{
    OS_CPU_SR cpu_sr;
    static u16 gSwrbTestTaskCnt = 0;
    static IFRD_TestTypeDef ifrd[SWRB_IFRD_CHAN_NUM];
    char *str;

    int i,j;

    while(1){
        gSwrbTestTaskCnt++;

        if(gSwrbTestTaskCnt == 1){
            gSwrbTestStateMap &= ~SWRB_TEST_FAULT_IFRD_MASK;
            gSwrbTestRuningTaskPrio = SWRB_IFRD_TEST_TASK_PRIO;
            MultiEdit_Set_Text_Color(GUI_BLACK);
            str = ">>>IFRD TEST<<<\r\n";
            MultiEdit_Add_Text(str);
            mf_open("0:/test/sn20151117.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
            mf_puts_with_offset(str);
            mf_close();
            printf("SENSOR->IFRD_LED=0\r\n");
            printf("SENSOR->B_SWITCH=0\r\n");
            OSTimeDlyHMSM(0,0,1,0);
            for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
                ifrd[i].offValue = 0;
                ifrd[i].onValue = 0;
                ifrd[i].validCnt = 0;
                ifrd[i].validFlag = 0;
            }
        }

        if(gSwrbTestTaskCnt%2){
            for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
                if(!ifrd[i].validFlag){
                    if(i==6){
                        printf("SENSOR->B_SWITCH=1\r\n");
                        OSTimeDlyHMSM(0,0,0,1);
                    }
                    for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                        if(i<6){
                            printf("SENSOR->READ=%d\r\n", i+1);
                        }else{
                            printf("SENSOR->READ=%d\r\n", i-1);
                        }
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            ifrd[i].offValue = usartRxNum;
                            Edit_Set_Value(ID_EDIT_U1+i, usartRxNum);
                            usartRxNum = 0;
                            usartRxFlag = 0;
                            break;
                        }else{
                            continue;
                        }
                    }
                }
            }
            printf("SENSOR->B_SWITCH=0\r\n");
            printf("SENSOR->IFRD_LED=1\r\n");
        }else{
            for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
                if(!ifrd[i].validFlag){
                    if(i==6){
                        printf("SENSOR->B_SWITCH=1\r\n");
                        OSTimeDlyHMSM(0,0,0,1);
                    }
                    for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                        if(i<6){
                            printf("SENSOR->READ=%d\r\n", i+1);
                        }else{
                            printf("SENSOR->READ=%d\r\n", i-1);
                        }
                        OSTimeDlyHMSM(0,0,0,6);
                        if(usartRxFlag){
                            ifrd[i].onValue = usartRxNum;
                            Edit_Set_Value(ID_EDIT_D1+i, usartRxNum);
                            usartRxNum = 0;
                            usartRxFlag = 0;
                            break;
                        }else{
                            continue;
                        }
                    }
                    
                    if(ifrd[i].onValue){
                        if( (ifrd[i].offValue - ifrd[i].onValue) > SWRB_IFRD_VALID_THRESHOLD[i] ){
                            gSwrbTestStateMap &= ~(0<<(SWRB_TEST_IFRD_F_L_POS+i));
                            ifrd[i].validCnt++;
                        }else{
                            gSwrbTestStateMap |= 1<<(SWRB_TEST_IFRD_F_L_POS+i);
                            ifrd[i].validCnt = 0;
                        }
                        
                        if(ifrd[i].validCnt > 5){
                            ifrd[i].validFlag = 1;
                        }
                    }
                }
            }
            printf("SENSOR->B_SWITCH=0\r\n");
            printf("SENSOR->IFRD_LED=0\r\n");

            if( ifrd[0].validFlag && ifrd[1].validFlag && ifrd[2].validFlag && ifrd[3].validFlag && \
                ifrd[4].validFlag && ifrd[5].validFlag && ifrd[6].validFlag && ifrd[7].validFlag
                ){
                gSwrbTestTaskCnt = 0;
                Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
                for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
                    gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOn_POS+i] = ifrd[i].onValue;
                    gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOff_POS+i] = ifrd[i].offValue;
                }
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
        }

        if(gSwrbTestTaskCnt > 50){
            gSwrbTestTaskCnt = 0;
            Edit_Set_Value(ID_EDIT_HEX, gSwrbTestStateMap);
            for(i=0;i<SWRB_IFRD_CHAN_NUM;i++){
                gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOn_POS+i] = ifrd[i].onValue;
                gSwrbTestAcquiredData[SWRB_TEST_DATA_IFRD_FL_TxOff_POS+i] = ifrd[i].offValue;
            }
            printf("SENSOR->IFRD_LED=0\r\n");
            printf("SENSOR->B_SWITCH=0\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_F_L_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_F_L\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_F_R_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_F_R\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_S_L_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_S_L\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_S_R_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_S_R\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FL_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_FL\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_FR_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_FR\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SL_MASK)
                MultiEdit_Add_Text("ERROR->IFRD_B_SL\r\n");
            if( gSwrbTestStateMap & SWRB_TEST_FAULT_IFRD_B_SR_MASK)
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
