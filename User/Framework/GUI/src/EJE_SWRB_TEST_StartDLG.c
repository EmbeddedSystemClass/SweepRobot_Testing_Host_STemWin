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
    { BUTTON_CreateIndirect, "", ID_START_BUTTON_TITLE, 175, 100, 450, 60, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "SD Warning", ID_START_TEXT_SD_WARNING, 325, 430, 150, 50, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_START_TEXT_VERSION, 600, 430, 200, 50, 0, 0x64, 0 },
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

//            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_POWER_STATION);
//            BUTTON_SetText(hItem, " ");
//            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_START_BUTTON_POWER_STATION, &_bmPSTestCHN, 19, 28);
//            Button_Init(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_SET);
            BUTTON_SetText(hItem, "");
            BUTTON_SetFont(hItem, GUI_FONT_32_ASCII);
            BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_START_BUTTON_SET, &_bmSetCHN, 68, 74);
            Button_Init(hItem);

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

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_TEXT_SD_WARNING);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
            if(gSwrbTestSDCardInsertState){
                TEXT_SetTextColor(hItem, GUI_BLUE);
                TEXT_SetText(hItem, "SD Card Inserted");
            }else{
                TEXT_SetTextColor(hItem, GUI_RED);
                TEXT_SetText(hItem, "No SD Card!");
            }
            WM_BringToTop(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_START_TEXT_VERSION);
            TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetText(hItem, "SWRB Fixture Ver:1.1");
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
                            SweepRobot_StartDlgManulBtnClickProc();
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
                case ID_START_BUTTON_SLAM: // Notifications sent by 'SLAM'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgSLAMBtnClickProc();
                            break;
                    }
                    break;
                case ID_START_BUTTON_STEP_MOTOR: // Notifications sent by 'STEPMOTOR'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgStepMotorBtnClickProc();
                            break;
                    }
                    break;
                case ID_START_BUTTON_DECRYPTO: // Notifications sent by 'Decrypto'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            SweepRobot_StartDlgDecryptoBtnClickProc();
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

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

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

/*************************** End of file ****************************/
