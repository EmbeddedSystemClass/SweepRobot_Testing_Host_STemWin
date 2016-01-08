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

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char *aSwrbDecryptoDlgButtonInitText[][1] = {
    {"OPEN"},
    {"DECRYPT"},
    {"SAVE"},
    {"EXIT"},
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "winDecrypto", ID_DECRYPTO_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
  { TREEVIEW_CreateIndirect, "tviewFilesystem", ID_DECRYPTO_TREEVIEW_MAIN, 0, 0, 260, 480, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "mlteditDisplay", ID_DECRYPTO_MULTIEDIT_MAIN, 260, 0, 440, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnOpen", ID_DECRYPTO_BUTTON_OPEN, 700, 0, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnDecrypt", ID_DECRYPTO_BUTTON_DECRYPT, 700, 120, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnSave", ID_DECRYPTO_BUTTON_SAVE, 700, 240, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnExit", ID_DECRYPTO_BUTTON_EXIT, 700, 360, 100, 120, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void Button_Init(WM_HWIN hItem)
{
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetSkinClassic(hItem);
    BUTTON_SetFocussable(hItem, DISABLE);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
}

static void Button_ExitProc(void)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;

    WM_HideWin(hWin_SWRB_DECRYPTO);
    WM_ShowWin(hWin_SWRB_START);
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  TREEVIEW_ITEM_Handle   hTreeItemCur;
  TREEVIEW_ITEM_Handle   hTreeItemNew;
  WM_HWIN                hItem;
  int                    NCode;
  int                    Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_DECRYPTO_TREEVIEW_MAIN);
    TREEVIEW_SetFont(hItem, GUI_FONT_32_ASCII);
    TREEVIEW_SetAutoScrollH(hItem, ENABLE);
    TREEVIEW_SetAutoScrollV(hItem, ENABLE);
    hTreeItemCur = TREEVIEW_GetItem(hItem, 0, TREEVIEW_GET_LAST);
    hTreeItemNew = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_NODE, "0:\\", 0);
    TREEVIEW_AttachItem(hItem, hTreeItemNew, hTreeItemCur, TREEVIEW_INSERT_BELOW);
    hTreeItemCur = TREEVIEW_GetItem(hItem, 0, TREEVIEW_GET_LAST);
    hTreeItemNew = TREEVIEW_ITEM_Create(TREEVIEW_ITEM_TYPE_NODE, "1:\\", 0);
    TREEVIEW_AttachItem(hItem, hTreeItemNew, hTreeItemCur, TREEVIEW_INSERT_BELOW);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_DECRYPTO_MULTIEDIT_MAIN);
    MULTIEDIT_SetText(hItem, "Multiedit");
    MULTIEDIT_SetAutoScrollH(hItem, ENABLE);
    MULTIEDIT_SetAutoScrollV(hItem, ENABLE);

    for(Id=ID_DECRYPTO_BUTTON_OPEN;Id<=ID_DECRYPTO_BUTTON_BOUND;Id++){
        hItem = WM_GetDialogItem(pMsg->hWin, Id);
        Button_Init(hItem);
        BUTTON_SetText(hItem, aSwrbDecryptoDlgButtonInitText[Id-ID_DECRYPTO_BUTTON_OPEN][0]);
    }

    WM_HideWin(pMsg->hWin);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_DECRYPTO_TREEVIEW_MAIN: // Notifications sent by 'tviewFilesystem'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    case ID_DECRYPTO_MULTIEDIT_MAIN: // Notifications sent by 'mlteditDisplay'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_DECRYPTO_BUTTON_OPEN: // Notifications sent by 'btnOpen'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_DECRYPTO_BUTTON_DECRYPT: // Notifications sent by 'btnDecrypt'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_DECRYPTO_BUTTON_SAVE: // Notifications sent by 'btnSave'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_DECRYPTO_BUTTON_EXIT: // Notifications sent by 'btnExit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
          Button_ExitProc();
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

WM_HWIN hWin_SWRB_DECRYPTO;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreatewinDecryptoDLG
*/

WM_HWIN CreatewinDecryptoDLG(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*************************** End of file ****************************/
