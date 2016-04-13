#include "sweeprobot_testing_brush.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define SWRB_BRUSH_CHAN_NUM     3

enum BrushChan{
    BRUSH_CHAN_L,
    BRUSH_CHAN_R,
    BRUSH_CHAN_M,
};

//const static u16 swrbTestBrushOCThreshold[SWRB_BRUSH_CHAN_NUM] = { 300, 300, 1000 };
const static u16 swrbTestBrushCurLowBound[SWRB_BRUSH_CHAN_NUM] = { 5, 5, 50 };
const static u16 swrbTestBrushCurHighBound[SWRB_BRUSH_CHAN_NUM] = { 50, 50, 500 };

static BRUSH_TestTypeDef brush[SWRB_BRUSH_CHAN_NUM];

static void SWRB_BrushTestTaskInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_BRUSH_TEST_TASK_PRIO;
    
    str = "\r\n>>>BRUSH TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    
    OSTimeDlyHMSM(0,0,1,0);
    
    for(i=0;i<SWRB_BRUSH_CHAN_NUM;i++){
        brush[i].current = 0;
        brush[i].validCnt = 0;
        brush[i].validFlag = 0;
    }
    
    printf("LBRUSH->SPEED=50\r\n");
    printf("RBRUSH->SPEED=50\r\n");
    printf("MBRUSH->SPEED=40\r\n");
    /* TODO: if using default brush startup speed,uncomment this */
    /*
    printf("BRUSH->ON=%d\r\n",BRUSH_CHAN_L);
    printf("BRUSH->ON=%d\r\n",BRUSH_CHAN_R);
    printf("BRUSH->ON=%d\r\n",BRUSH_CHAN_M);
    */
}

static void SWRB_BrushTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_BRUSH_CHAN_NUM;i++){
        if(!brush[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("BRUSH->READ=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    brush[i].current = usartRxNum;
                    Edit_Set_Value(ID_MAIN_EDIT_U1+i, usartRxNum);
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }
            if(swrbTestBrushCurLowBound[i]<brush[i].current && swrbTestBrushCurHighBound[i]>brush[i].current){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_BRUSH_L_STATE_POS+i));
                brush[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_BRUSH_L_STATE_POS+i));
            }
            
            if( brush[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                brush[i].validFlag = 1;
                printf("BRUSH->OFF=%d\r\n",i);
            }
        }
    }

    if(brush[BRUSH_CHAN_L].validFlag && brush[BRUSH_CHAN_R].validFlag && brush[BRUSH_CHAN_M].validFlag){
        gSwrbTestTaskRunCnt = 0;

        SWRB_TestDataSaveToFile(Brush_TestDataSave);
        
        str = "BRUSH OK\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  "BRUSH OK\r\n");
        
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_BRUSH, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_BRUSH, "BRUSH OK");
        Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_BRUSH, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_BRUSH_TEST_TASK_PRIO);
    }
}

void SWRB_BrushTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    printf("BRUSH->OFF=%d\r\n",BRUSH_CHAN_L);
    printf("BRUSH->OFF=%d\r\n",BRUSH_CHAN_R);
    printf("BRUSH->OFF=%d\r\n",BRUSH_CHAN_M);
    
    SWRB_TestDataSaveToFile(Brush_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        str = "ERROR->LBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        str = "ERROR->RBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  "ERROR->RBRUSH\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        str = "ERROR->MBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  "ERROR->MBRUSH\r\n");
    }
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_BRUSH, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_BRUSH, "BRUSH ERROR");
    Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_BRUSH, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();
    
    SWRB_NextTestTaskResumePostAct(SWRB_BRUSH_TEST_TASK_PRIO);
}

void SweepRobot_BrushTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_BRUSH)){
            SWRB_NextTestTaskResumePreAct(SWRB_BRUSH_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
            
            Checkbox_Set_Box_Back_Color(ID_MAIN_CHECKBOX_BRUSH, GUI_GREEN, CHECKBOX_CI_ENABLED);

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
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_L_CUR_POS] = brush[BRUSH_CHAN_L].current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_R_CUR_POS] = brush[BRUSH_CHAN_R].current;
    gSwrbTestAcquiredData[SWRB_TEST_DATA_BRUSH_M_CUR_POS] = brush[BRUSH_CHAN_M].current;
    
    SWRB_TestDataFileWriteData("LBRUSH->CURRENT=", brush[BRUSH_CHAN_L].current, 1);
    SWRB_TestDataFileWriteData("RBRUSH->CURRENT=", brush[BRUSH_CHAN_R].current, 1);
    SWRB_TestDataFileWriteData("MBRUSH->CURRENT=", brush[BRUSH_CHAN_M].current, 1);
}
