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

#include "sweeprobot_testing.h"

#include "eje_logo_char.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

// USER START (Optionally insert additional defines)
WM_HWIN hWin_SWRB_START;
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
*       _aDialogStart
*/
static const GUI_WIDGET_CREATE_INFO _aDialogStart[] = {
  { WINDOW_CreateIndirect, "Window", ID_START_WINDOW_START, 0, 0, 800, 480, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_START_IMAGE_LOGO, 247, 5, 306, 153, 0, 0, 0 },
  { BUTTON_CreateIndirect, "PCB TEST", ID_START_BUTTON_PCB_TEST, 100, 205, 200, 180, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "POWER STATION", ID_START_BUTTON_POWER_STATION, 500, 203, 200, 180, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_START_TEXT_VERSION, 600, 430, 200, 50, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetImageById
*/
static const void * _GetImageById(U32 Id, U32 * pSize) {
  switch (Id) {
  case ID_START_IMAGE_0_IMAGE_0:
    *pSize = sizeof(_acImage_eje_logo);
    return (const void *)_acImage_eje_logo;
  }
  return NULL;
}

// USER START (Optionally insert additional static code)
static void Button_Init(WM_HWIN hItem)
{
    BUTTON_SetFont(hItem, GUI_FONT_32_ASCII);
    BUTTON_SetSkinClassic(hItem);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_Simple);
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
  int          NCode;
  int          Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_START_IMAGE_LOGO);
    pData = _GetImageById(ID_START_IMAGE_0_IMAGE_0, &FileSize);
    IMAGE_SetBMP(hItem, pData, FileSize);
    //
    // Initialization of 'PCB TEST'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_PCB_TEST);
    BUTTON_SetText(hItem, "PCB TEST");
    Button_Init(hItem);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_START_BUTTON_POWER_STATION);
    BUTTON_SetText(hItem, "Power Station");
    Button_Init(hItem);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_START_TEXT_VERSION);
    TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "SWRB Fixture Ver:1.0");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_START_BUTTON_PCB_TEST: // Notifications sent by 'PCB TEST'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_StartDlgPCBBtnClickProc();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_START_BUTTON_POWER_STATION: // Notifications sent by 'POWER STATION'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_StartDlgPowerStationBtnClickPorc();
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
*       CreateWindow
*/
WM_HWIN CreateEJE_SWRB_TEST_StartDLG(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogStart, GUI_COUNTOF(_aDialogStart), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
