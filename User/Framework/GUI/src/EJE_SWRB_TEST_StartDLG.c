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

extern GUI_CONST_STORAGE GUI_BITMAP bmDibeaLogo_300x64;

extern GUI_CONST_STORAGE GUI_BITMAP _bmPCBTestCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmPSTestCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmSetCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmInsertUDiskCHN;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       _aDialogStart
*/
static const GUI_WIDGET_CREATE_INFO _aDialogStart[] = {
    { WINDOW_CreateIndirect, "Window", ID_START_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "Image", ID_START_IMAGE_LOGO, 250, 32, 300, 64, 0, 0, 0 },
    { EDIT_CreateIndirect, "Date", ID_START_EDIT_DATE, 0,430,200,50,0, 0x64, 0 },
    { BUTTON_CreateIndirect, "PCB TEST", ID_START_BUTTON_PCB_TEST, 100, 205, 200, 180, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "MANUL", ID_START_BUTTON_MANUL, 500, 205, 200, 180, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "SET", ID_START_BUTTON_SET, 300, 205, 200, 180, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "SLAM", ID_START_BUTTON_SLAM, 300, 385, 100, 90, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "STEP MOTOR", ID_START_BUTTON_STEP_MOTOR, 400, 385, 100, 90, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Decrypto", ID_START_BUTTON_DECRYPTO, 500, 385, 100, 90, 0, 0x0, 0 },
#ifdef USE_RMT_CTRL
//    { BUTTON_CreateIndirect, "CONTROL", ID_START_BUTTON_CONTROL, 0, 0, 120, 120, 0, 0x0, 0 },
#endif
#ifdef USE_MANUL_MANUL_MODE
//    { BUTTON_CreateIndirect, "MANUL", ID_START_BUTTON_MANUL_MANUL_MODE, 680, 0, 120, 120, 0, 0x0, 0 },
#endif
    { BUTTON_CreateIndirect, "", ID_START_BUTTON_TITLE, 175, 100, 450, 60, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Storage Warning", ID_START_TEXT_STORAGE_WARNING, 250, 430, 300, 50, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_START_TEXT_VERSION, 600, 430, 200, 50, 0, 0x64, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogStartWarning[] = {
    { WINDOW_CreateIndirect, "Window", ID_START_WINDOW_WARNING, 100, 205, 600, 180, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Please Insert Udisk", ID_START_BUTTON_WARNING_INDICATE, 60, 20, 480, 70, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "OK", ID_START_BUTTON_WARNING_OK, 225, 110, 150, 60, 0, 0x0, 0 },
};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*/

static void Button_Init(WM_HWIN hItem)
{
//    BUTTON_SetSkinClassic(hItem);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
}

static void Button_WarningOkProc(void)
{
    gSwrbTestSubDialogSelectFlag = SWRB_TEST_SUB_DIALOG_NONE;
    GUI_EndDialog(hWin_SWRB_START_WARNING, 0);
}

#ifdef USE_RMT_CTRL
static void Button_ControlProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_CONTROL;
    
//    STD_UART_RX_DISABLE();
    STD_UART_ENABLE();
    
    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_CONTROL);
}
#endif

static void Button_ManualManualModeProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_MANUL;
    
    gSwrbTestRuningTaskPrio = SWRB_MANUAL_TEST_TASK_PRIO;
    
    gSwrbTestUDiskInsertCmpSkipFlag = DISABLE;
    
    SweepRobot_ManualSetBtnEnterManualModeProc();
    
    WM_HideWin(hWin_SWRB_START);
    WM_ShowWin(hWin_SWRB_MANUAL);
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN      hItem;
    int          NCode;
    int          Id;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_IMAGE_LOGO);
            IMAGE_SetBitmap(hItem, &bmDibeaLogo_300x64);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_EDIT_DATE);
            EDIT_SetFont(hItem, GUI_FONT_20_ASCII);
            EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
            EDIT_SetTextColor(hItem, EDIT_CI_ENABELD, GUI_BLUE);
            EDIT_SetFocussable(hItem, DISABLE);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_TITLE);
            BUTTON_SetSkinClassic(hItem);
            WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
            BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
            BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_WHITE);
            BUTTON_SetFocussable(hItem, DISABLE);
            BUTTON_DispSWRBTestTitleCHNStr(pMsg->hWin, ID_START_BUTTON_TITLE, 81, 13);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_PCB_TEST);
            BUTTON_SetText(hItem, " ");
            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_START_BUTTON_PCB_TEST, &_bmPCBTestCHN, 36, 74);
            Button_Init(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_SET);
            BUTTON_SetText(hItem, "");
            BUTTON_SetFont(hItem, GUI_FONT_32_ASCII);
            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_START_BUTTON_SET, &_bmSetCHN, 68, 74);
            Button_Init(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_CONTROL);
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            BUTTON_SetText(hItem, "CONTROL");
            Button_Init(hItem);

#ifdef USE_MANUL_MANUL_MODE
            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_MANUL_MANUL_MODE);
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            BUTTON_SetText(hItem, "MANUL");
            Button_Init(hItem);
#endif

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_SLAM);
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            BUTTON_SetText(hItem, "SLAM");
            Button_Init(hItem);
            WM_HideWin(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_STEP_MOTOR);
            BUTTON_SetFont(hItem, GUI_FONT_16_ASCII);
            BUTTON_SetText(hItem, "STEP MOTOR");
            Button_Init(hItem);
            WM_HideWin(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_MANUL);
            BUTTON_DispRobotTestTitleCHNStr(pMsg->hWin, ID_START_BUTTON_MANUL, 36, 74);
            Button_Init(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_DECRYPTO);
            BUTTON_SetText(hItem, "Decrypto");
            Button_Init(hItem);
            WM_HideWin(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_TEXT_STORAGE_WARNING);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
#ifdef USE_SD_CARD
            if(gSwrbTestSDCardInsertState){
                TEXT_SetTextColor(hItem, GUI_BLUE);
                TEXT_SetText(hItem, "SD Card Inserted");
            }else{
                TEXT_SetTextColor(hItem, GUI_RED);
                TEXT_SetText(hItem, "SD Card not been Inserted");
            }
#endif
            if(gSwrbTestUDiskInsertState){
                TEXT_SetTextColor(hItem, GUI_BLUE);
                TEXT_SetText(hItem, "UDisk Inserted");
            }else{
                TEXT_SetTextColor(hItem, GUI_RED);
                TEXT_SetText(hItem, "UDisk not been inserted");
            }
            WM_BringToTop(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_TEXT_VERSION);
            TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetText(hItem, SWRB_FIXTURE_VERSION);
            TEXT_SetTextColor(hItem, GUI_BLUE);
            break;
        case WM_PAINT:
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) {
                case ID_START_BUTTON_PCB_TEST: // Notifications sent by 'PCB TEST'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgPCBBtnClickProc();
                            break;
                    }
                    break;
                case ID_START_BUTTON_MANUL: // Notifications sent by 'MANUL'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgManualBtnClickProc();
                            break;
                    }
                    break;
                case ID_START_BUTTON_SET: // Notifications sent by 'SET'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgSetBtnClickProc();
                            break;
                    }
                    break;
#ifdef USE_SLAM
                case ID_START_BUTTON_SLAM: // Notifications sent by 'SLAM'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgSLAMBtnClickProc();
                            break;
                    }
                    break;
#endif
#ifdef USE_STEPMOTOR_CTRL
                case ID_START_BUTTON_STEP_MOTOR: // Notifications sent by 'STEPMOTOR'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgStepMotorBtnClickProc();
                            break;
                    }
                    break;
#endif
#ifdef USE_DECRYPTO
                case ID_START_BUTTON_DECRYPTO: // Notifications sent by 'Decrypto'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgDecryptoBtnClickProc();
                            break;
                    }
                    break;
#endif
#ifdef USE_RMT_CTRL
                case ID_START_BUTTON_CONTROL: // Notifications sent by 'Control'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_ControlProc();
                            break;
                    }
                    break;
#endif
#ifdef USE_MANUL_MANUL_MODE
                case ID_START_BUTTON_MANUL_MANUL_MODE: // Notifications sent by 'MANUL_MANUL_MODE'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_ManualManualModeProc();
                            break;
                    }
                    break;
#endif
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

static void _cbStartWarningDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN      hItem;
    int          NCode;
    int          Id;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_WARNING_INDICATE);
            Button_Set_BkColor(pMsg->hWin, ID_START_BUTTON_WARNING_INDICATE, GUI_WHITE);
            BUTTON_SetSkinClassic(hItem);
            WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
            BUTTON_SetFont(hItem, GUI_FONT_32_ASCII);
//            BUTTON_SetText(hItem, "Please Insert Udisk!");
            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_START_BUTTON_WARNING_INDICATE, &_bmInsertUDiskCHN, 164, 19);
            BUTTON_SetFocussable(hItem, DISABLE);
        
            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_WARNING_OK);
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            BUTTON_SetText(hItem, "OK");
            
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) {
                case ID_START_BUTTON_WARNING_OK:
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Button_WarningOkProc();
                            break;
                    }
                    break;
            }
            break;
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

WM_HWIN hWin_SWRB_START;
WM_HWIN hWin_SWRB_START_WARNING;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void Swrb_StartDlgWarningOKBtnProc(void)
{
    Button_WarningOkProc();
}

/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateEJE_SWRB_TEST_StartDLG(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogStart, GUI_COUNTOF(_aDialogStart), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

WM_HWIN CreateEJE_SWRB_TEST_StartWarningDLG(void)
{
    WM_HWIN hWin;

    gSwrbTestSubDialogSelectFlag = SWRB_TEST_SUB_DIALOG_WARNING;

    hWin = GUI_CreateDialogBox(_aDialogStartWarning, GUI_COUNTOF(_aDialogStartWarning), _cbStartWarningDialog, hWin_SWRB_START, 0, 0);
    return hWin;
}
/*************************** End of file ****************************/
