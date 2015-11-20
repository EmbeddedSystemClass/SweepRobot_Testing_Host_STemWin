#include "sweepRobot_testing_wheel.h"

#include "usart.h"
#include "includes.h"
#include "timer.h"

#define SWRB_TEST_LWHEEL_SPEED_LOW_BOUND        0
#define SWRB_TEST_LWHEEL_SPEED_HIGH_BOUND       50
#define SWRB_TEST_RWHEEL_SPEED_LOW_BOUND        5
#define SWRB_TEST_RWHEEL_SPEED_HIGH_BOUND       50

static WHEEL_TestTypeDef lWheel;
static WHEEL_TestTypeDef rWheel;

static void SWRB_WheelTestInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_WHEEL_TEST_TASK_PRIO;
    
    str = "\r\n>>>WHEEL TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    lWheel.speed = 0;
    lWheel.validCnt = 0;
    lWheel.validFlag = 0;
    rWheel.speed = 0;
    rWheel.validCnt = 0;
    rWheel.validFlag = 0;
    
    printf("LWHEEL->SPEED=30\r\n");
    printf("RWHEEL->SPEED=30\r\n");
}

static void SWRB_WheelTestProc(void)
{
    u8 i;
    char *str;
    
    if(!lWheel.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("LWHEEL->READ\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                lWheel.speed = (u8)usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                Edit_Set_Value(ID_EDIT_U1, lWheel.speed);
                break;
            }else{
                continue;
            }
        }
        if(SWRB_TEST_LWHEEL_SPEED_LOW_BOUND<lWheel.speed && SWRB_TEST_LWHEEL_SPEED_HIGH_BOUND>lWheel.speed){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_L_STATE_POS);
            lWheel.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_L_STATE_POS);
            lWheel.validCnt = 0;
        }
        
        if(lWheel.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            lWheel.validFlag = 1;
            printf("LWHEEL->SPEED=0\r\n");
        }
    }

    if(!rWheel.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("RWHEEL->READ\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                rWheel.speed = (u8)usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                Edit_Set_Value(ID_EDIT_D1, rWheel.speed);
                break;
            }else{
                continue;
            }
        }
        if(SWRB_TEST_RWHEEL_SPEED_LOW_BOUND<rWheel.speed && SWRB_TEST_RWHEEL_SPEED_HIGH_BOUND>rWheel.speed){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_WHEEL_R_STATE_POS);
            rWheel.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_WHEEL_R_STATE_POS);
            rWheel.validCnt = 0;
        }
        
        if(rWheel.validCnt > SWRB_TEST_VALID_COMP_TIMES){
            rWheel.validFlag = 1;
            printf("RWHEEL->SPEED=0\r\n");
        }
    }

    if(lWheel.validFlag && rWheel.validFlag ){
        gSwrbTestTaskRunCnt = 0;

        gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_L_SPEED_POS] = lWheel.speed;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_R_SPEED_POS] = rWheel.speed;
        SWRB_TestDataSaveToFile(Wheel_TestDataSave);

        str = "WHEEL OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(str);
        Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_WHEEL);

        SWRB_NextTestTaskResume(SWRB_WHEEL_TEST_TASK_PRIO);
    }      
}

static void SWRB_WheelTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;

    printf("LWHEEL->SPEED=0\r\n");
    printf("RWHEEL->SPEED=0\r\n");
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_L_SPEED_POS] = lWheel.speed;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_WHEEL_R_SPEED_POS] = rWheel.speed;
    SWRB_TestDataSaveToFile(Wheel_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_L_MASK){
        str = "ERROR->LEFT WHEEL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_WHEEL_R_MASK){
        str = "ERROR->RIGHT WHEEL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_WHEEL, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_WHEEL, "WHEEL ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_WHEEL);

    SWRB_NextTestTaskResume(SWRB_WHEEL_TEST_TASK_PRIO);
}

void SweepRobot_WheelTestTask(void *pdata)
{
    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_WHEEL)){
            SWRB_NextTestTaskResume(SWRB_WHEEL_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SWRB_WheelTestInit();
            }

            if(gSwrbTestTaskRunCnt > 4){
                SWRB_WheelTestProc();
            }

            if(20 < gSwrbTestTaskRunCnt){
                SWRB_WheelTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void Wheel_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("LWHEEL->SPEED=", lWheel.speed);
    SWRB_TestDataFileWriteData("RWHEEL->SPEED=", rWheel.speed);
}
