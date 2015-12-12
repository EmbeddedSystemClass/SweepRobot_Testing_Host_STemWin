/******************** (C) COPYRIGHT 2008 EJE ***********************************************************
* File Name          : EJE_SWRB_TEST_DLG_Conf.c
* Author             : MeredithRowe@163.com
* Version            : V1.0
* Date               : 12/10/2015
* Description        : User GUI function
*********************************************************************************************************/

#include "EJE_SWRB_TEST_DLG_Conf.h"

#include "sweeprobot_testing.h"

extern GUI_CONST_STORAGE GUI_BITMAP _bmStartCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmPauseCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmResumeCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmSetCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmStopCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmExitCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmConfirmCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmCheckCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmResetCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmCancelCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmSerialNumCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmTimeCHN;

static u8 gMainDLGIndicateFlag = 0;
static WM_HWIN hLastShowWin = 0;
static int gLastShowId = 0;

void Button_Set_Text(WM_HWIN hWin, int buttonId, char *str)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, buttonId);
    BUTTON_SetText(hItem, str);
}

void Button_Set_BkColor(WM_HWIN hWin, int buttonId, GUI_COLOR color)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, buttonId);
    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, color);
    BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, color);
}

void Button_SetEnable(WM_HWIN hWin, int buttonId)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, buttonId);
    WM_EnableWindow(hItem);
}

void Button_SetDisable(WM_HWIN hWin, int buttonId)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, buttonId);
    WM_DisableWindow(hItem);
}

void BUTTON_Set_Bitmap_Ex(WM_HWIN hWin, int buttonId, const GUI_BITMAP *pBitmap, int x, int y)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, buttonId);
    BUTTON_SetText(hItem, " ");
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_DISABLED, pBitmap, x, y);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_PRESSED, pBitmap, x, y);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED, pBitmap, x, y);
}

void BUTTON_DispStartCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmStartCHN, x, y);
}

void BUTTON_DispPauseCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmPauseCHN, x, y);
}

void BUTTON_DispResumeCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmResumeCHN, x, y);
}

void BUTTON_DispSetCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmSetCHN, x, y);
}

void BUTTON_DispStopCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmStopCHN, x, y);
}

void BUTTON_DispConfirmCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmConfirmCHN, x, y);
}

void BUTTON_DispCheckCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmCheckCHN, x, y);
}

void BUTTON_DispResetCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmResetCHN, x, y);
}

void BUTTON_DispCancelCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmCancelCHN, x, y);
}

void BUTTON_DispExitCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmExitCHN, x, y);
}

void BUTTON_DispSerialNumCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmSerialNumCHN, x, y);
}

void BUTTON_DispTimeCHNStr(WM_HWIN hWin, int buttonId, int x, int y)
{
    BUTTON_Set_Bitmap_Ex(hWin, buttonId, &_bmTimeCHN, x, y);
}

void Progbar_Set_Value(int progbarValue)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWin_SWRB_MAIN, ID_MAIN_PROGBAR_0);
	PROGBAR_SetValue(hItem, progbarValue);
}

void Progbar_Set_Percent(void)
{
    Progbar_Set_Value( (float)(gSwrbTestValidTaskCntTotal-gSwrbTestValidTaskCnt) / (float)(gSwrbTestValidTaskCntTotal)*100 );
}

void Edit_Set_Value(WM_HWIN hWin, int editId, long editValue)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_SetValue(hItem, editValue);
}

void Edit_Set_Text(WM_HWIN hWin, int editId, char *str)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_SetText(hItem, str);
}

void Edit_Get_Text(WM_HWIN hWin, int editId, char *str)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_GetText(hItem, str, EDIT_GetNumChars(hItem)+1);
}
 
void Edit_Set_HexMode(WM_HWIN hWin, int editId, long value, long min, long max)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_SetHexMode(hItem, value, min, max);
}

void Edit_Set_DecMode(WM_HWIN hWin, int editId, long value, long min, long max, int shift, unsigned char flags)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_SetDecMode(hItem, value, min, max, shift, flags);
}

void Edit_Clear(void)
{
    int i;
    
    for(i=ID_MAIN_EDIT_1;i<ID_MAIN_EDIT_SN;i++){
        Edit_Set_Value(hWin_SWRB_MAIN , i, 0);
    }
}

void Text_Set_Text(WM_HWIN hWin, int textId, char *str)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, textId);
    TEXT_SetText(hItem, str);
}
void Text_Set_Color(WM_HWIN hWin, int textId, GUI_COLOR color)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, textId);
    TEXT_SetTextColor(hItem, color);
}

void Checkbox_Set_Text(WM_HWIN hWin, int checkboxId, char *string)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, checkboxId);
    CHECKBOX_SetText(hItem, string);
}

int Checkbox_Get_State(int checkboxId)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, checkboxId);
    return(CHECKBOX_GetState(hItem));
}

void Checkbox_Set_State(WM_HWIN hWin, int checkboxId, unsigned int checkboxState)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, checkboxId);
    CHECKBOX_SetState(hItem, checkboxState);
}

void SWRB_TestCheckboxStateSet(u8 stateNum)
{
    int i;
    WM_HWIN hItem;
    
    for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
        CHECKBOX_SetState(hItem, stateNum);
    }
}

void SWRB_TestCheckboxEnable(void)
{
    int i;
    WM_HWIN hItem;
    
    for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
        WM_EnableWindow(hItem);
    }
}

void SWRB_TestCheckboxDisable(void)
{
    int i;
    WM_HWIN hItem;
    
    for(i=ID_MAIN_CHECKBOX_WHEEL;i<ID_MAIN_CHECKBOX_BOUND;i++){
        hItem = WM_GetDialogItem(hWin_SWRB_MAIN, i);
        WM_DisableWindow(hItem);
    }
}

void SWRB_TestCheckboxStateGet(WM_HWIN hWin, int id, int taskPrio){
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, id);
    if(gSwrbTestMode == SWRB_TEST_MODE_IDLE){
        if(CHECKBOX_GetState(hItem)){
            gSwrbTestValidTaskCnt++;
            gSwrbTestValidTaskCntTotal++;
        }else{
            gSwrbTestValidTaskCnt--;
            gSwrbTestValidTaskCntTotal--;
        }
    }
}

void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxtextcolor)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, checkboxId);
    CHECKBOX_SetTextColor(hItem, checkboxtextcolor);
}

void Checkbox_Set_TextAlign(int checkboxId, int align)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, checkboxId);
    CHECKBOX_SetTextAlign(hItem, align);
}

void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, checkboxId);
    CHECKBOX_SetBkColor(hItem, checkboxbkcolor);
}

void Checkbox_Set_Box_Back_Color(WM_HWIN hWin, int checkboxId, GUI_COLOR boxBkColor, int Index)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, checkboxId);
    CHECKBOX_SetBoxBkColor(hItem, boxBkColor, Index);
}

void ListWheel_AddNumString(WM_HWIN hItem, int startStringNum, int endStringNum)
{
    int i;
    char *str;
    
    for(i=startStringNum;i<=endStringNum;i++){
        str = mymalloc(SRAMIN, sizeof(char)*10);
        sprintf(str, "%d", i);
        LISTWHEEL_AddString(hItem, str);
        myfree(SRAMIN, str);
    }
}


void Multiedit_Set_Buffer_Size(int size)
{
    WM_HWIN hItem;
    
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN);
    MULTIEDIT_SetBufferSize(hItem, size);
}

void MultiEdit_Set_Text(WM_HWIN hWin, int multiEditId, char *s)
{
	WM_HWIN hItem;
    
	hItem = WM_GetDialogItem(hWin, multiEditId);
	MULTIEDIT_SetText(hItem, s);
}

void MultiEdit_Add_Text(WM_HWIN hWin, int multiEditId, char *s)
{
    WM_HWIN hItem;
    
    hItem = WM_GetDialogItem(hWin, multiEditId);
    MULTIEDIT_AddText(hItem, s);
}

void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_0);
    MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_EDIT, multieditTextColor);
}

GRAPH_DATA_Handle Graph_Data_YT_Create(GUI_COLOR color, u32 maxNumItems, int16_t *pData, u32 numItems)
{
    GRAPH_DATA_Handle hGraphData;
    hGraphData = GRAPH_DATA_YT_Create(color, maxNumItems, pData, numItems);
    return hGraphData;
}

void SweepRobot_MainTestIndicateBtnToggle(void)
{
    if(++gMainDLGIndicateFlag%2){
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_DARKRED);
    }else{
        Button_Set_BkColor(hWin_SWRB_MAIN, ID_MAIN_BUTTON_INDICATE, GUI_LIGHTGRAY);
    }
}

void SWRB_WM_EnableWindow(WM_HWIN hWin, int id)
{
    WM_HWIN hItem;
    
    hItem = WM_GetDialogItem(hWin, id);
    WM_EnableWindow(hItem);
}

void SWRB_WM_DisableWindow(WM_HWIN hWin, int id)
{
    WM_HWIN hItem;
    
    hItem = WM_GetDialogItem(hWin, id);
    WM_DisableWindow(hItem);
}

void SWRB_RTC_TIME_Disp(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    char *str;
    
    str = mymalloc(SRAMIN, sizeof(char)*30);
    sprintf(str,"%d/%d/%d %d:%d:%d", date->RTC_Year, date->RTC_Month, date->RTC_Date, time->RTC_Hours, time->RTC_Minutes, time->RTC_Seconds);
    if(gSwrbTestMode == SWRB_TEST_MODE_SET){
        Edit_Set_Text(hWin_SWRB_TIMESETTING, ID_TIMESET_EDIT_ACTVALUE, str);
    }else{
        Edit_Set_Text(hWin_SWRB_MAIN, ID_MAIN_EDIT_DATE, str);
    }
    myfree(SRAMIN, str);
}

void SWRB_LastCallNumpadEditSave(WM_MESSAGE *pMsg)
{
    hLastShowWin = pMsg->hWin;
    gLastShowId = WM_GetId(pMsg->hWinSrc);
}

WM_HWIN SWRB_LastCallNumpadEditWinGet(void)
{
    return hLastShowWin;
}

int SWRB_LastCallNumpadEditIdGet(void)
{
    return gLastShowId;
}
