#include "sweeprobot_testing_wheel.h"

#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"
#include "timer.h"

#define SWRB_WHEEL_TEST_TIME_OUT_CNT    20

const static int aSwrbWheelTestSpeedLowThreshold[SWRB_WHEEL_CHAN_NUM] = { 0, 0 };
const static int aSwrbWheelTestSpeedHighThreshold[SWRB_WHEEL_CHAN_NUM] = { 50, 50 };

static WHEEL_TestTypeDef wheel[SWRB_WHEEL_CHAN_NUM];

static void SWRB_WheelTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_WHEEL_TEST_TASK_PRIO;

    str = "\r\n>>>WHEEL TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    for(i=0;i<SWRB_WHEEL_CHAN_NUM;i++){
        mymemset(&wheel[i], 0, sizeof(wheel[i]));
    }
    
    printf("WHL->DIR=1\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("LW->SPD=50\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("RW->SPD=50\r\n");
    
    /*
    printf("WHL->ON=%d\r\n",WHEEL_CHAN_L);
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("WHL->ON=%d\r\n",WHEEL_CHAN_R);
    */

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SWRB_WheelTestProc(void)
{
    u8 i,j;
    char *str;

    for(i=0;i<SWRB_WHEEL_CHAN_NUM;i++){
        if(!wheel[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("WHL->RD=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    wheel[i].speed = (u8)usartRxNum;
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1, wheel[i].speed);
                    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        str = mymalloc(SRAMIN, sizeof(char)*10);
                        *str = 0;
                        sprintf(str, "%d", usartRxNum);
                        Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS+i][0],\
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS+i][1],\
                                                str);
                        myfree(SRAMIN, str);
                    }
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    USART_RX_STA = 0;
                    break;
                }else{
                    continue;
                }
            }

            if(aSwrbWheelTestSpeedLowThreshold[i]<wheel[i].speed && aSwrbWheelTestSpeedHighThreshold[i]>wheel[i].speed){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_L_STATE_POS+i));
                wheel[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_L_STATE_POS+i));
            }

            if(wheel[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                wheel[i].validFlag = 1;
                printf("WHL->OFF=%d\r\n",i);

                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS+i][0],\
                                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS+i][1],\
                                                               SWRB_MANUL_TEST_OK_BK_COLOR);
                }
            }
        }
    }

    if(wheel[WHEEL_CHAN_L].validFlag && wheel[WHEEL_CHAN_R].validFlag ){
        gSwrbTestTaskRunCnt = 0;

        SWRB_TestDataSaveToFile(Wheel_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "WHEEL OK\r\n";
            SWRB_TestDataFileWriteString(str);

//            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN, str);
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL, "WHEEL OK");
            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
            Edit_Clear();
        }

        SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_TEST_TASK_PRIO);
    }
}

static void SWRB_WheelPCBTestTimeOutProc(void)
{
    char *str;

    SWRB_TestDataSaveToFile(Wheel_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_L_MASK){
            str = "ERROR->LEFT WHEEL\r\n";
            SWRB_TestDataFileWriteString(str);
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_R_MASK){
            str = "ERROR->RIGHT WHEEL\r\n";
            SWRB_TestDataFileWriteString(str);
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }

    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_WHEEL, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL, "WHEEL ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();
}

static void SWRB_WheelManulTestTimeOutProc(void)
{
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_L_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS][0],\
                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_L_SPEED_POS][1],\
                                                   SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_R_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_R_SPEED_POS][0],\
                                                   gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_WHEEL_R_SPEED_POS][1],\
                                                   SWRB_MANUL_TEST_FAULT_BK_COLOR);
    }
}

static void SWRB_WheelTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;

    printf("WHL->OFF=0\r\n");
    printf("WHL->OFF=1\r\n");

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SWRB_WheelPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SWRB_WheelManulTestTimeOutProc();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    /* TODO: should fix bug */
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_TEST_TASK_PRIO);
#endif
}

static void SWRB_WheelTestProgDisp(void)
{
    Progbar_ManulTest_Set_Percent(gSwrbTestTaskRunCnt, SWRB_WHEEL_TEST_TIME_OUT_CNT);
}

void SweepRobot_WheelTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL)){
            SWRB_NextTestTaskResumePreAct(SWRB_WHEEL_TEST_TASK_PRIO);
        }else{
            SWRB_WheelTestProgDisp();
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SWRB_WheelTestInit();
            }

            if(4 < gSwrbTestTaskRunCnt){
                SWRB_WheelTestProc();
            }

            if(SWRB_WHEEL_TEST_TIME_OUT_CNT < gSwrbTestTaskRunCnt){
                SWRB_WheelTestTimeOutProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void Wheel_TestDataSave(void)
{
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_L_SPEED_POS] = wheel[WHEEL_CHAN_L].speed;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_R_SPEED_POS] = wheel[WHEEL_CHAN_R].speed;

    SWRB_TestDataFileWriteData("LWHEEL->SPEED=", wheel[WHEEL_CHAN_L].speed, 1);
    SWRB_TestDataFileWriteData("RWHEEL->SPEED=", wheel[WHEEL_CHAN_R].speed, 1);
}
