/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

#include "EJE_SWRB_TEST_DLG_Conf.h"

#include "sweeprobot_testing.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

extern GUI_CONST_STORAGE GUI_BITMAP _bmTimeCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmWhiteTimeCHN;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static int  lastLwIndex[6];
static RTC_DateTypeDef rtcDate;
static RTC_TimeTypeDef rtcTime;

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "TEST SETTING", ID_TIMESET_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
//    { TEXT_CreateIndirect, "Set Time", ID_TIMESET_TEXT_TITLE, 20, 10, 680, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Title", ID_TIMESET_BUTTON_TITLE, 20, 10, 680, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Confirm", ID_TIMESET_BUTTON_CONFIRM, 700, 0, 100, 120, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Check", ID_TIMESET_BUTTON_CHECK, 700, 120, 100, 120, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Reset", ID_TIMESET_BUTTON_RESET, 700, 240, 100, 120, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Cancel", ID_TIMESET_BUTTON_CANCEL, 700, 360, 100, 120, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "SNSet", ID_TIMESET_BUTTON_SNSET, 0, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "TestSel", ID_TIMESET_BUTTON_TESTSELECT, 100, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Rsrv2", ID_TIMESET_BUTTON_RESERVE2, 200, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Rsrv3", ID_TIMESET_BUTTON_RESERVE3, 300, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Rsrv4", ID_TIMESET_BUTTON_RESERVE4, 400, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Rsrv5", ID_TIMESET_BUTTON_RESERVE5, 500, 420, 100, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "TimeSet", ID_TIMESET_BUTTON_TIMESET, 600, 420, 100, 60, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwYear", ID_TIMESET_LISTWHEEL_YEAR, 20, 60, 110, 230, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwMonth", ID_TIMESET_LISTWHEEL_MONTH, 130, 60, 110, 230, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwDay", ID_TIMESET_LISTWHEEL_DAY, 240, 60, 110, 230, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwHour", ID_TIMESET_LISTWHEEL_HOUR, 350, 60, 110, 230, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwMin", ID_TIMESET_LISTWHEEL_MIN, 460, 60, 110, 230, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "lwSec", ID_TIMESET_LISTWHEEL_SEC, 570, 60, 110, 230, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "editSetValue", ID_TIMESET_EDIT_SETVALUE, 20, 340, 330, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editActValue", ID_TIMESET_EDIT_ACTVALUE, 350, 340, 330, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editYear", ID_TIMESET_EDIT_YEAR, 20, 300, 110, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editMonth", ID_TIMESET_EDIT_MONTH, 130, 300, 110, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editDay", ID_TIMESET_EDIT_DAY, 240, 300, 110, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editHour", ID_TIMESET_EDIT_HOUR, 350, 300, 110, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editMin", ID_TIMESET_EDIT_MIN, 460, 300, 110, 40, 0 ,0x0, 0 },
    { EDIT_CreateIndirect, "editSec", ID_TIMESET_EDIT_SEC, 570, 300, 110, 40, 0 ,0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void ListWheel_ResetToLastPos(WM_HWIN hWin);
static void ListWheel_ResetToZero(WM_HWIN hWin);
static void TimeStr_Comb(WM_HWIN hWin, int editId);


static void Button_Init(WM_HWIN hItem)
{
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetSkinClassic(hItem);
    BUTTON_SetFocussable(hItem, DISABLE);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
}

static void Button_ConfirmProc(WM_HWIN hWin)
{
    WM_HideWin(hWin_SWRB_TIMESET);
    WM_ShowWin(hWin_SWRB_LOGIN);
}

static void Button_CheckProc(WM_HWIN hWin)
{
    TimeStr_Comb(hWin, ID_TIMESET_EDIT_SETVALUE);
}

static void Button_ResetProc(WM_HWIN hWin)
{
    ListWheel_ResetToZero(hWin);
}

static void Button_CancelProc(WM_HWIN hWin)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;
    
    ListWheel_ResetToLastPos(hWin);
    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_START);
}

static void Button_SNSetProc(WM_HWIN hWin)
{
    Button_Set_BkColor(hWin_SWRB_SNSET, ID_SNSET_BUTTON_SNSET, GUI_LIGHTRED);
    gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_SN;
    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_SNSET);
}

static void Button_TestSelSetProc(WM_HWIN hWin)
{
    Button_Set_BkColor(hWin_SWRB_TESTSEL, ID_TESTSEL_BUTTON_TESTSEL, GUI_LIGHTRED);
    gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_TESTSEL;
    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_TESTSEL);
}

static void ListWheel_Init(WM_HWIN hItem)
{
    LISTWHEEL_SetFont(hItem, GUI_FONT_32_ASCII);
    LISTWHEEL_SetDeceleration(hItem, 10);
    LISTWHEEL_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTWHEEL_SetSnapPosition(hItem, 115);
    LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_SEL, GUI_LIGHTRED);
    LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_UNSEL, GUI_BLACK);
    LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_SEL, GUI_WHITE);
    LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_UNSEL, GUI_GRAY);
}

static void ListWheel_SelChangeProc(WM_HWIN hWin, int lwId, int editId)
{
    int  lwItemIndex;
    char *lwBuf;
    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin, lwId);
    lwItemIndex = LISTWHEEL_GetPos(hItem);
    LISTWHEEL_SetSel(hItem, lwItemIndex);
    lwBuf = mymalloc(SRAMIN, sizeof(char)*10);
    LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 10);
    hItem = WM_GetDialogItem(hWin, editId);
    EDIT_SetText(hItem, lwBuf);
    myfree(SRAMIN, lwBuf);
}

static void ListWheel_ResetToLastPos(WM_HWIN hWin)
{
    WM_HWIN hItem;
    int i;
    u8 j;

    for(i=ID_TIMESET_LISTWHEEL_YEAR,j=0;i<=ID_TIMESET_LISTWHEEL_SEC;i++,j++){
        hItem = WM_GetDialogItem(hWin, i);
        LISTWHEEL_SetPos(hItem, lastLwIndex[j]);
    }
}

static void ListWheel_ResetToZero(WM_HWIN hWin)
{
    WM_HWIN hItem;
    int i;

    for(i=ID_TIMESET_LISTWHEEL_YEAR;i<=ID_TIMESET_LISTWHEEL_SEC;i++){
        hItem = WM_GetDialogItem(hWin, i);
        LISTWHEEL_MoveToPos(hItem, 0);
    }
}

static void TimeStr_Comb(WM_HWIN hWin, int editId)
{
    int     lwItemIndex;
    int     tmp;
    char    *lwBuf;
    char    *timeStr;
    WM_HWIN hItem;

    timeStr = mymalloc(SRAMIN, sizeof(char)*30);
    if(timeStr != NULL){
        *timeStr = 0;
        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_YEAR);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        lwBuf = mymalloc(SRAMIN, sizeof(char)*5);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s/", lwBuf);
        tmp = atoi(lwBuf);
        rtcDate.RTC_Year = tmp%2000;

        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_MONTH);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s%s/", timeStr,lwBuf);
        rtcDate.RTC_Month = atoi(lwBuf);

        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_DAY);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s%s ", timeStr, lwBuf);
        rtcDate.RTC_Date = atoi(lwBuf);

        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_HOUR);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s%s:", timeStr, lwBuf);
        rtcTime.RTC_Hours = atoi(lwBuf);

        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_MIN);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s%s:", timeStr, lwBuf);
        rtcTime.RTC_Minutes = atoi(lwBuf);

        hItem = WM_GetDialogItem(hWin, ID_TIMESET_LISTWHEEL_SEC);
        lwItemIndex = LISTWHEEL_GetPos(hItem);
        *lwBuf = 0;
        LISTWHEEL_GetItemText(hItem, lwItemIndex, lwBuf, 5);
        sprintf(timeStr, "%s%s", timeStr, lwBuf);
        myfree(SRAMIN, lwBuf);
        rtcTime.RTC_Seconds = atoi(lwBuf);

        hItem = WM_GetDialogItem(hWin, editId);
        EDIT_SetText(hItem, timeStr);

        myfree(SRAMIN, timeStr);
    }
}



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    int     i;


    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:

            hItem = pMsg->hWin;
            FRAMEWIN_SetTextColor(hItem, 0x00000000);

//            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_TEXT_0);
//            TEXT_SetFont(hItem, &GUI_Font32_ASCII);
//            TEXT_SetTextColor(hItem, GUI_WHITE);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_BUTTON_TITLE);
            BUTTON_SetSkinClassic(hItem);
            WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
            BUTTON_SetText(hItem, "");
            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TIMESET_BUTTON_TITLE, &_bmWhiteTimeCHN, 0, 13);
            Button_Set_BkColor(pMsg->hWin, ID_TIMESET_BUTTON_TITLE, GUI_BLACK);

            for(i=ID_TIMESET_BUTTON_CONFIRM;i<=ID_TIMESET_BUTTON_RESERVE5;i++){
                hItem = WM_GetDialogItem(pMsg->hWin, i);
                Button_Init(hItem);
                if(i%2){
                    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTGRAY);
                }else{

                }
            }
            BUTTON_DispConfirmCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_CONFIRM, 18, 43);
            BUTTON_DispCheckCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_CHECK, 18, 43);
            BUTTON_DispResetCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_RESET, 18, 43);
            BUTTON_DispCancelCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_CANCEL, 18, 43);
            BUTTON_DispSerialNumCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_SNSET, 14, 18);
            BUTTON_DispTimeCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_TIMESET, 26, 18);
            BUTTON_DispTestSelCHNStr(pMsg->hWin, ID_TIMESET_BUTTON_TESTSELECT, 14, 18);
            //
            // Initialization of 'lwYear'
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_LISTWHEEL_YEAR);
            ListWheel_Init(hItem);
            ListWheel_AddNumString(hItem, 2015, 2025);
            //
            // Initialization of 'lwMonth'
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_LISTWHEEL_MONTH);
            ListWheel_Init(hItem);
            ListWheel_AddNumString(hItem, 1, 12);
            //
            // Initialization of 'lwDay'
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_LISTWHEEL_DAY);
            ListWheel_Init(hItem);
            ListWheel_AddNumString(hItem, 1, 31);
            //
            // Initialization of 'lwHour'
            //
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_LISTWHEEL_HOUR);
            ListWheel_Init(hItem);
            ListWheel_AddNumString(hItem, 0, 23);
            //
            // Initialization of 'lwMin,lwSec'
            //
            for(i=ID_TIMESET_LISTWHEEL_MIN;i<=ID_TIMESET_LISTWHEEL_SEC;i++){
                hItem = WM_GetDialogItem(pMsg->hWin, i);
                ListWheel_Init(hItem);
                ListWheel_AddNumString(hItem, 0, 59);
            }
            //
            // Initialization of 'edit'
            //
            for(i=ID_TIMESET_EDIT_SETVALUE;i<=ID_TIMESET_EDIT_SEC;i++){
                hItem = WM_GetDialogItem(pMsg->hWin, i);
                EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
                EDIT_SetTextMode(hItem);
                EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            }
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_EDIT_SETVALUE);
            EDIT_SetMaxLen(hItem, 30);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIMESET_EDIT_ACTVALUE);
            EDIT_SetMaxLen(hItem, 30);

            WM_HideWin(pMsg->hWin);

            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) {
                case ID_TIMESET_BUTTON_CONFIRM: // Notifications sent by 'Confirm'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_ConfirmProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_BUTTON_CHECK: // Notifications sent by 'Check'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_CheckProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_BUTTON_RESET: // Notifications sent by 'Reset'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_ResetProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_BUTTON_CANCEL: // Notifications sent by 'Cancel'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_CancelProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_BUTTON_SNSET:
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_SNSetProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_BUTTON_TESTSELECT:
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_TestSelSetProc(pMsg->hWin);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_YEAR: // Notifications sent by 'lwYear'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, ID_TIMESET_LISTWHEEL_YEAR, ID_TIMESET_EDIT_YEAR);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_MONTH: // Notifications sent by 'lwMonth'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, ID_TIMESET_LISTWHEEL_MONTH, ID_TIMESET_EDIT_MONTH);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_DAY: // Notifications sent by 'lwDay'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, ID_TIMESET_LISTWHEEL_DAY, ID_TIMESET_EDIT_DAY);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_HOUR: // Notifications sent by 'lwHour'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, Id, ID_TIMESET_EDIT_HOUR);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_MIN: // Notifications sent by 'lwMin'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, Id, ID_TIMESET_EDIT_MIN);
                            break;
                    }
                    break;
                case ID_TIMESET_LISTWHEEL_SEC: // Notifications sent by 'lwSec'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            ListWheel_SelChangeProc(pMsg->hWin, Id, ID_TIMESET_EDIT_SEC);
                            break;
                    }
                    break;
                }
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

WM_HWIN hWin_SWRB_TIMESET;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSettingDLG
*/
WM_HWIN CreateTimeSettingDLG(void) {
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin_SWRB_PCBTEST, 0, 0);
    return hWin;
}

void SWRB_TimeSettingsChangeConfirmProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;
    gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_NONE;
    
    TimeStr_Comb(hWin_SWRB_TIMESET, ID_TIMESET_EDIT_SETVALUE);
    
    PWR_BackupAccessCmd(ENABLE);
    while(!(RTC_SetDate(RTC_Format_BIN, &rtcDate)));
    while(!(RTC_SetTime(RTC_Format_BIN, &rtcTime)));
    while(!RTC_WaitForSynchro());
    PWR_BackupAccessCmd(DISABLE);
}

/*************************** End of file ****************************/
