#include "sweeprobot_testing_wheel.h"

#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"
#include "timer.h"

#define SWRB_WHEEL_CHAN_NUM     2

enum WheelChan{
    
    WHEEL_CHAN_L,
    WHEEL_CHAN_R,
};

const static int SWRB_TEST_WHEEL_SPEED_LOW_BOUND[SWRB_WHEEL_CHAN_NUM] = { 0, 0 };
const static int SWRB_TEST_WHEEL_SPEED_HIGH_BOUND[SWRB_WHEEL_CHAN_NUM] = { 50, 50 };

static WHEEL_TestTypeDef wheel[SWRB_WHEEL_CHAN_NUM];

static void SWRB_WheelTestInit(void)
{
    u8 i;
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_WHEEL_TEST_TASK_PRIO;

    str = "\r\n>>>WHEEL TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_MAIN_MULTIEDIT_MAIN,  str);

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);

    for(i=0;i<SWRB_WHEEL_CHAN_NUM;i++){
        wheel[i].speed = 0;
        wheel[i].validCnt = 0;
        wheel[i].validFlag = 0;
    }

    printf("WHEEL->DIR=1\r\n");

    printf("LWHEEL->SPEED=40\r\n");
    printf("RWHEEL->SPEED=40\r\n");
    /*
    printf("WHEEL->ON=%d\r\n",WHEEL_CHAN_L);
    printf("WHEEL->ON=%d\r\n",WHEEL_CHAN_R);
    */
}

static void SWRB_WheelTestProc(void)
{
    u8 i,j;
    char *str;

    for(i=0;i<SWRB_WHEEL_CHAN_NUM;i++){
        if(!wheel[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("WHEEL->READ=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    wheel[i].speed = (u8)usartRxNum;
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    USART_RX_STA = 0;
                    Edit_Set_Value(hWin_SWRB_PCBTEST, ID_MAIN_EDIT_U1, wheel[i].speed);
                    break;
                }else{
                    continue;
                }
            }

            if(SWRB_TEST_WHEEL_SPEED_LOW_BOUND[i]<wheel[i].speed && SWRB_TEST_WHEEL_SPEED_HIGH_BOUND[i]>wheel[i].speed){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_WHEEL_L_STATE_POS+i));
                wheel[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_WHEEL_L_STATE_POS+i));
            }

            if(wheel[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                wheel[i].validFlag = 1;
                printf("WHEEL->OFF=%d\r\n",i);
            }
        }
    }

    if(wheel[WHEEL_CHAN_L].validFlag && wheel[WHEEL_CHAN_R].validFlag ){
        gSwrbTestTaskRunCnt = 0;

        SWRB_TestDataSaveToFile(Wheel_TestDataSave);

        str = "WHEEL OK\r\n";
        SWRB_TestDataFileWriteString(str);

        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_MAIN_MULTIEDIT_MAIN, str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_WHEEL, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_MAIN_CHECKBOX_WHEEL, "WHEEL OK");
        Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_MAIN_CHECKBOX_WHEEL, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_TEST_TASK_PRIO);
    }
}

static void SWRB_WheelTestOverTimeProc(void)
{
    char *str;

    gSwrbTestTaskRunCnt = 0;

    printf("WHEEL->OFF=0\r\n");
    printf("WHEEL->OFF=1\r\n");

    SWRB_TestDataSaveToFile(Wheel_TestDataSave);

    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_L_MASK){
        str = "ERROR->LEFT WHEEL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_R_MASK){
        str = "ERROR->RIGHT WHEEL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_WHEEL, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_MAIN_CHECKBOX_WHEEL, "WHEEL ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_MAIN_CHECKBOX_WHEEL, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_WHEEL_TEST_TASK_PRIO);
#endif
}

void SweepRobot_WheelTestTask(void *pdata)
{
    while(1){

        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_WHEEL)){
            SWRB_NextTestTaskResumePreAct(SWRB_WHEEL_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SWRB_WheelTestInit();
            }

            if(4 < gSwrbTestTaskRunCnt){
                SWRB_WheelTestProc();
            }

            if(20 < gSwrbTestTaskRunCnt){
                SWRB_WheelTestOverTimeProc();
            }
//            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
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
