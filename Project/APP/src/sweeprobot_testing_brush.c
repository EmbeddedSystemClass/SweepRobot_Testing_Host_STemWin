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

static BRUSH_TestTypeDef lBrush;
static BRUSH_TestTypeDef rBrush;
static BRUSH_TestTypeDef mBrush;

static void SWRB_BrushTestTaskInit(void)
{
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_BRUSH_TEST_TASK_PRIO;
    
    str = "\r\n>>>BRUSH TEST<<<";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);
    
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

static void SWRB_BrushTestProc(void)
{
    u8 i;
    if(!lBrush.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("\r\nLBRUSH->READ\r\n");
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
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_L_STATE_POS);
            lBrush.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_BRUSH_L_STATE_POS);
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
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_R_STATE_POS);
            rBrush.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_BRUSH_R_STATE_POS);
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
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_BRUSH_M_STATE_POS);
            mBrush.validCnt++;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_BRUSH_M_STATE_POS);
            mBrush.validCnt=0;
        }
        
        if(mBrush.validCnt > 5){
            mBrush.validFlag = 1;
            printf("MBRUSH->SPEED=0\r\n");
        }
    }

    if(lBrush.validFlag && rBrush.validFlag && mBrush.validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_L_CUR_POS] = lBrush.current;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_R_CUR_POS] = rBrush.current;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_M_CUR_POS] = mBrush.current;
        SWRB_TestDataSaveToFile(Brush_TestDataSave);
        
        MultiEdit_Add_Text("BRUSH OK\r\n");
        Checkbox_Set_Text_Color(ID_CHECKBOX_BRUSH, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_BRUSH);

        SWRB_NextTestTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
    }
}

void SWRB_BrushTestOverTimeProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("LBRUSH->SPEED=0\r\n");
    printf("RBRUSH->SPEED=0\r\n");
    printf("MBRUSH->SPEED=0\r\n");
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_L_CUR_POS] = lBrush.current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_R_CUR_POS] = rBrush.current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_M_CUR_POS] = mBrush.current;
    SWRB_TestDataSaveToFile(Brush_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        MultiEdit_Add_Text("ERROR->LBRUSH\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        MultiEdit_Add_Text("ERROR->RBRUSH\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        MultiEdit_Add_Text("ERROR->MBRUSH\r\n");
    }
    Checkbox_Set_Text_Color(ID_CHECKBOX_BRUSH, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_BRUSH);

    SWRB_NextTestTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
}

void SweepRobot_BrushTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(ID_CHECKBOX_BRUSH)){
            SWRB_NextTestTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SWRB_BrushTestTaskInit();
            }
            if(gSwrbTestTaskRunCnt > 5){
                SWRB_BrushTestProc();
            }
            if(gSwrbTestTaskRunCnt > 20){
                SWRB_BrushTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void Brush_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("LBRUSH->CURRENT=", lBrush.current);
    SWRB_TestDataFileWriteData("RBRUSH->CURRENT=", rBrush.current);
    SWRB_TestDataFileWriteData("MBRUSH->CURRENT=", mBrush.current);
}
