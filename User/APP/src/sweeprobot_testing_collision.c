#include "sweeprobot_testing_collision.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

static COLLISION_TestTypeDef collision[SWRB_COLLISION_CHAN_NUM];

static void SweepRobot_CollisionTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;

    str = "\r\n>>>COLLISION TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef __SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        collision[i].onValue = 0;
        collision[i].offValue = 0;
        collision[i].onValidCnt = 0;
        collision[i].offValidCnt = 0;
        collision[i].onValidFlag = 0;
        collision[i].validFlag = 0;
    }
}

static void SweepRobot_CollisionTestProc(void)
{
    u8 i,j;
    char *str;

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        if(!collision[i].validFlag){
            if(!collision[i].onValidFlag){
                SweepRobot_CollisionCtrlOn((enum CollisionChan)i);
                OSTimeDlyHMSM(0,0,0,2);
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("CLSN->RD=%d\r\n",i);
                    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                    if(usartRxFlag){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
                        collision[i].onValue = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        USART_RX_STA = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(!collision[i].onValue){
                    collision[i].onValidCnt++;
                }else{
                    gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
                    collision[i].onValidCnt = 0;
                }

                if(collision[i].onValidCnt > 1){
                    collision[i].onValidFlag = 1;
                }
            }else{
                SweepRobot_CollisionCtrlOff((enum CollisionChan)i);
                OSTimeDlyHMSM(0,0,0,2);
                for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                    printf("CLSN->RD=%d\r\n",i);
                    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                    if(usartRxFlag){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_D1+i, usartRxNum);
                        collision[i].offValue = usartRxNum;
                        usartRxNum = 0;
                        usartRxFlag = 0;
                        USART_RX_STA = 0;
                        break;
                    }else{
                        continue;
                    }
                }

                if(collision[i].offValue){
                    collision[i].offValidCnt++;
                    gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+i));
                }else{
                    gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
                    collision[i].offValidCnt = 0;
                }

                if(collision[i].offValidCnt > 1){
                    collision[i].validFlag = 1;
                }
            }
        }
    }

    if(collision[COLLISION_CHAN_L].validFlag && collision[COLLISION_CHAN_FL].validFlag && collision[COLLISION_CHAN_R].validFlag && collision[COLLISION_CHAN_FR].validFlag){
        gSwrbTestTaskRunCnt = 0;

        SweepRobot_CollisionCtrlOff(COLLISION_CHAN_ALL);

        SWRB_TestDataSaveToFile(Collision_TestDataSave);

        str = "COLLISION OK\r\n";
        SWRB_TestDataFileWriteString(str);

//        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_COLLISION, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION, "COLLISION OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
    }
}

static void SweepRobot_CollisionTestOverTimeProc(void)
{
    char *str;

    gSwrbTestTaskRunCnt = 0;

    SweepRobot_CollisionCtrlOff(COLLISION_CHAN_ALL);

    SWRB_TestDataSaveToFile(Collision_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK){
        str = "ERROR->COLLISION_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FL_MASK){
        str = "ERROR->COLLISION_FL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK){
        str = "ERROR->COLLISION_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FR_MASK){
        str = "ERROR->COLLISION_FR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }

    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_COLLISION, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_COLLISION, "COLLISION ERROR");
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
#endif
}

void SweepRobot_CollisionTestTask(void *pdata)
{

    SweepRobot_CollisionTestGPIOInit();

    while(1){

        if(!Checkbox_Get_State(ID_PCBTEST_CHECKBOX_COLLISION)){
            SWRB_NextTestTaskResumePreAct(SWRB_COLLISION_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_CollisionTestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_CollisionTestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_CollisionTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void Collision_TestDataSave(void)
{
    u8 i;

    for(i=0;i<SWRB_COLLISION_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_ON_VALUE_POS+i] = collision[i].onValue;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_OFF_VALUE_POS+i] = collision[i].offValue;
    }

    SWRB_TestDataFileWriteData("COLLISION->L_onValue=", collision[COLLISION_CHAN_L].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FL_onValue=", collision[COLLISION_CHAN_FL].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->R_onValue=", collision[COLLISION_CHAN_R].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FR_onValue=", collision[COLLISION_CHAN_FR].onValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->L_offValue=", collision[COLLISION_CHAN_L].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FL_offValue=", collision[COLLISION_CHAN_FL].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->R_offValue=", collision[COLLISION_CHAN_R].offValue, 1);
    SWRB_TestDataFileWriteData("COLLISION->FR_offValue=", collision[COLLISION_CHAN_FR].offValue, 1);
}