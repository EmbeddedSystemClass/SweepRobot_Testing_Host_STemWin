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

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_LISTWHEEL_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_1 (GUI_ID_USER + 0x03)
#define ID_LISTWHEEL_1 (GUI_ID_USER + 0x04)
#define ID_LISTWHEEL_2 (GUI_ID_USER + 0x05)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "SettingDLG", ID_FRAMEWIN_0, 0, 0, 320, 240, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 50, 50, 50, 68, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "confirm", ID_BUTTON_0, 40, 160, 100, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 178, 161, 100, 40, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_1, 110, 50, 50, 68, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_2, 170, 50, 50, 68, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'SettingDLG'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem, 0x00000000);
    //
    // Initialization of 'Listwheel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
    LISTWHEEL_AddString(hItem, "2014");
    LISTWHEEL_AddString(hItem, "2015");
    LISTWHEEL_AddString(hItem, "2016");
    LISTWHEEL_AddString(hItem, "2017");
    LISTWHEEL_AddString(hItem, "2018");
    //
    // Initialization of 'confirm'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    BUTTON_SetText(hItem, "Confirm");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    BUTTON_SetText(hItem, "Cancel");
    //
    // Initialization of 'Listwheel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
    LISTWHEEL_AddString(hItem, "1");
    LISTWHEEL_AddString(hItem, "2");
    LISTWHEEL_AddString(hItem, "3");
    LISTWHEEL_AddString(hItem, "4");
    LISTWHEEL_AddString(hItem, "5");
    LISTWHEEL_AddString(hItem, "6");
    LISTWHEEL_AddString(hItem, "7");
    LISTWHEEL_AddString(hItem, "8");
    LISTWHEEL_AddString(hItem, "9");
    LISTWHEEL_AddString(hItem, "10");
    LISTWHEEL_AddString(hItem, "11");
    LISTWHEEL_AddString(hItem, "12");
    //
    // Initialization of 'Listwheel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
    LISTWHEEL_AddString(hItem, "1");
    LISTWHEEL_AddString(hItem, "2");
    LISTWHEEL_AddString(hItem, "3");
    LISTWHEEL_AddString(hItem, "4");
    LISTWHEEL_AddString(hItem, "5");
    LISTWHEEL_AddString(hItem, "6");
    LISTWHEEL_AddString(hItem, "7");
    LISTWHEEL_AddString(hItem, "8");
    LISTWHEEL_AddString(hItem, "9");
    LISTWHEEL_AddString(hItem, "10");
    LISTWHEEL_AddString(hItem, "11");
    LISTWHEEL_AddString(hItem, "12");
    LISTWHEEL_AddString(hItem, "13");
    LISTWHEEL_AddString(hItem, "14");
    LISTWHEEL_AddString(hItem, "15");
    LISTWHEEL_AddString(hItem, "16");
    LISTWHEEL_AddString(hItem, "17");
    LISTWHEEL_AddString(hItem, "18");
    LISTWHEEL_AddString(hItem, "19");
    LISTWHEEL_AddString(hItem, "20");
    LISTWHEEL_AddString(hItem, "21");
    LISTWHEEL_AddString(hItem, "22");
    LISTWHEEL_AddString(hItem, "23");
    LISTWHEEL_AddString(hItem, "24");
    LISTWHEEL_AddString(hItem, "25");
    LISTWHEEL_AddString(hItem, "26");
    LISTWHEEL_AddString(hItem, "27");
    LISTWHEEL_AddString(hItem, "28");
    LISTWHEEL_AddString(hItem, "29");
    LISTWHEEL_AddString(hItem, "30");
    LISTWHEEL_AddString(hItem, "31");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTWHEEL_0: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'confirm'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_1: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_2: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

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
WM_HWIN CreateSettingDLG(void);
WM_HWIN CreateSettingDLG(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/