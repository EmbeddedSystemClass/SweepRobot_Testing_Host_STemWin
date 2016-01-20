#include "sweeprobot_testing_brush.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

const static u16 aSwrbBrushTestCurLowThreshold[SWRB_BRUSH_CHAN_NUM] = { 5, 5, 50 };
const static u16 aSwrbBrushTestCurHighThreshold[SWRB_BRUSH_CHAN_NUM] = { 100, 100, 1000 };

static BRUSH_TestTypeDef brush[SWRB_BRUSH_CHAN_NUM];

static void SWRB_BrushTestTaskInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_BRUSH_TEST_TASK_PRIO;
    
    str = "\r\n>>>BRUSH TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif
    
    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);
    
    for(i=0;i<SWRB_BRUSH_CHAN_NUM;i++){
        mymemset(&brush[i], 0, sizeof(brush[i]));
    }
    
    printf("LB->SPD=100\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("RB->SPD=100\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("MB->SPD=60\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);
}

static void SWRB_BrushTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_BRUSH_CHAN_NUM;i++){
        if(!brush[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("BRS->RD=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_READ_WAIT_TIME);
                if(usartRxFlag){
                    brush[i].current = usartRxNum;
                    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
                        Edit_Set_Value(hWin_SWRB_PCBTEST, ID_PCBTEST_EDIT_U1+i, usartRxNum);
                    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                        str = mymalloc(SRAMIN, sizeof(char)*10);
                        *str = 0;
                        sprintf(str, "%d", usartRxNum);
                        Listview_Set_Item_Text(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN, \
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS+i][0],\
                                                gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS+i][1],\
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
            if(aSwrbBrushTestCurLowThreshold[i]<brush[i].current && aSwrbBrushTestCurHighThreshold[i]>brush[i].current){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_BRUSH_L_STATE_POS+i));
                brush[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_BRUSH_L_STATE_POS+i));
            }
            
            if( brush[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                brush[i].validFlag = 1;
                printf("BRS->OFF=%d\r\n",i);
                
                if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
                    Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS+i][0],\
                                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS+i][1],\
                                                               GUI_LIGHTBLUE);
                }
            }
        }
    }

    if(brush[BRUSH_CHAN_L].validFlag && brush[BRUSH_CHAN_R].validFlag && brush[BRUSH_CHAN_M].validFlag){
        gSwrbTestTaskRunCnt = 0;
        
        SWRB_TestDataSaveToFile(Brush_TestDataSave);

        if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
            str = "BRUSH OK\r\n";
            SWRB_TestDataFileWriteString(str);
    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "BRUSH OK\r\n");
            
            Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_BRUSH, GUI_BLUE);
            Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, "BRUSH OK");
            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
            Edit_Clear();
        }
        
        SWRB_NextTestTaskResumePostAct(SWRB_BRUSH_TEST_TASK_PRIO);
    }
}

static void SWRB_BrushPCBTestTimeOutProc(void)
{
    char *str;
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        str = "ERROR->LBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        str = "ERROR->RBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "ERROR->RBRUSH\r\n");
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        str = "ERROR->MBRUSH\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  "ERROR->MBRUSH\r\n");
    }
    Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_BRUSH, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, "BRUSH ERROR");
    Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, GUI_LIGHTGRAY, CHECKBOX_CI_ENABLED);
    Edit_Clear();   
}

static void SWRB_BrushManulTestTimeOutProc(void)
{
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_L_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS][0],\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_L_CUR_POS][1],\
                                               GUI_LIGHTRED);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_R_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_R_CUR_POS][0],\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_R_CUR_POS][1],\
                                               GUI_LIGHTRED);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_BRUSH_M_MASK){
        Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_M_CUR_POS][0],\
                                               gSwrbManulTestListviewDispDataCoord[SWRB_MANUL_TEST_DATA_BRUSH_M_CUR_POS][1],\
                                               GUI_LIGHTRED);
    }
}

void SWRB_BrushTestTimeOutProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("BRS->OFF=%d\r\n",BRUSH_CHAN_L);
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("BRS->OFF=%d\r\n",BRUSH_CHAN_R);
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_USART_WRITE_WAIT_TIME);
    printf("BRS->OFF=%d\r\n",BRUSH_CHAN_M);
    
    SWRB_TestDataSaveToFile(Brush_TestDataSave);
    
    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        SWRB_BrushPCBTestTimeOutProc();
    }else if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        SWRB_BrushManulTestTimeOutProc();
    }
    
#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_BRUSH_TEST_TASK_PRIO);
#endif
}

void SweepRobot_BrushTestTask(void *pdata)
{
    while(1){
        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH)){
            SWRB_NextTestTaskResumePreAct(SWRB_BRUSH_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;
            
            Checkbox_Set_Box_Back_Color(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_BRUSH, GUI_GREEN, CHECKBOX_CI_ENABLED);

            if(gSwrbTestTaskRunCnt == 1){
                SWRB_BrushTestTaskInit();
            }
            if(gSwrbTestTaskRunCnt > 5){
                SWRB_BrushTestProc();
            }
            if(gSwrbTestTaskRunCnt > 50){
                SWRB_BrushTestTimeOutProc();
            }
            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
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
