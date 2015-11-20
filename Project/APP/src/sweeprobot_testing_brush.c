#include "sweepRobot_testing_brush.h"

#include "usart.h"
#include "includes.h"

#define SWRB_BRUSH_CHAN_NUM     3

u16 swrbTestBrushOCThreshold[SWRB_BRUSH_CHAN_NUM] = { 300, 300, 1000 };
u16 swrbTestBrushCurLowBound[SWRB_BRUSH_CHAN_NUM] = { 5, 5, 50 };
u16 swrbTestBrushCurHighBound[SWRB_BRUSH_CHAN_NUM] = { 50, 50, 500 };

static BRUSH_TestTypeDef brush[SWRB_BRUSH_CHAN_NUM];

static void SWRB_BrushTestTaskInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_BRUSH_TEST_TASK_PRIO;
    
    str = "\r\n>>>BRUSH TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    printf("LBRUSH->SPEED=30\r\n");
    printf("RBRUSH->SPEED=30\r\n");
    printf("MBRUSH->SPEED=30\r\n");
    
    for(i=0;i<SWRB_BRUSH_CHAN_NUM;i++){
        brush[i].current = 0;
        brush[i].validCnt = 0;
        brush[i].validFlag = 0;
    }
}

static void SWRB_BrushTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(j=0;j<SWRB_BRUSH_CHAN_NUM;j++){
        if(!brush[j].validFlag){
            for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
                printf("LBRUSH->READ\r\n");
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    brush[i].current = usartRxNum;
                    Edit_Set_Value(ID_EDIT_U1+j, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
            if(swrbTestBrushCurLowBound[j]<brush[j].current && swrbTestBrushCurHighBound[j]>brush[j].current){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_BRUSH_L_STATE_POS+j));
                brush[j].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_BRUSH_L_STATE_POS+j));
                brush[j].validCnt=0;
            }
            
            if( brush[j].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                brush[j].validFlag = 1;
                printf("LBRUSH->SPEED=0\r\n");
            }
        }
    }

    if(brush[0].validFlag && brush[1].validFlag && brush[2].validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_L_CUR_POS] = brush[0].current;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_R_CUR_POS] = brush[1].current;
        gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_M_CUR_POS] = brush[2].current;
        SWRB_TestDataSaveToFile(Brush_TestDataSave);
        
        str = "BRUSH OK\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text("BRUSH OK\r\n");
        
        Checkbox_Set_Text_Color(ID_CHECKBOX_BRUSH, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_BRUSH, "BRUSH OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_BRUSH);

        SWRB_NextTestTaskResume(SWRB_BRUSH_TEST_TASK_PRIO);
    }
}

void SWRB_BrushTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("LBRUSH->SPEED=0\r\n");
    printf("RBRUSH->SPEED=0\r\n");
    printf("MBRUSH->SPEED=0\r\n");
    
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_L_CUR_POS] = brush[0].current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_R_CUR_POS] = brush[1].current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_M_CUR_POS] = brush[2].current;
    SWRB_TestDataSaveToFile(Brush_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        str = "ERROR->LBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        str = "ERROR->RBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text("ERROR->RBRUSH\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        str = "ERROR->MBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
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
    SWRB_TestDataFileWriteData("LBRUSH->CURRENT=", brush[0].current);
    SWRB_TestDataFileWriteData("RBRUSH->CURRENT=", brush[1].current);
    SWRB_TestDataFileWriteData("MBRUSH->CURRENT=", brush[2].current);
}
