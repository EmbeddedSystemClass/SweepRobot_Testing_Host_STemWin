#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "TestSelSettingDLG", ID_TESTSEL_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL", ID_TESTSEL_BUTTON_WHEEL, 0, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BRUSH", ID_TESTSEL_BUTTON_BRUSH, 140, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "FAN", ID_TESTSEL_BUTTON_FAN, 280, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IFRD", ID_TESTSEL_BUTTON_IFRD, 420, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "COLLISION", ID_TESTSEL_BUTTON_COLLISION, 560, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL FLOAT", ID_TESTSEL_BUTTON_WHEEL_FLOAT, 0, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ASH TRAY", ID_TESTSEL_BUTTON_ASH_TRAY, 140, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UNIWHEEL", ID_TESTSEL_BUTTON_UNIWHEEL, 280, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY", ID_TESTSEL_BUTTON_KEY, 420, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IRDA", ID_TESTSEL_BUTTON_IRDA, 560, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUZZER", ID_TESTSEL_BUTTON_BUZZER, 0, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RGB LED", ID_TESTSEL_BUTTON_RGB_LED, 140, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CHARGE", ID_TESTSEL_BUTTON_CHARGE, 280, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST1", ID_TESTSEL_BUTTON_TEST1, 420, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST2", ID_TESTSEL_BUTTON_TEST2, 560, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST3", ID_TESTSEL_BUTTON_TEST3, 0, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST4", ID_TESTSEL_BUTTON_TEST4, 140, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST5", ID_TESTSEL_BUTTON_TEST5, 280, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST6", ID_TESTSEL_BUTTON_TEST6, 420, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST7", ID_TESTSEL_BUTTON_TEST7, 560, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Confirm", ID_TESTSEL_BUTTON_CONFIRM, 700, 0, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelAll", ID_TESTSEL_BUTTON_SELALL, 700, 120, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelNone", ID_TESTSEL_BUTTON_SELNONE, 700, 240, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_TESTSEL_BUTTON_CANCEL, 700, 360, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SnSet", ID_TESTSEL_BUTTON_SNSET, 0, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimeSet", ID_TESTSEL_BUTTON_TIMESET, 100, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TestSel", ID_TESTSEL_BUTTON_TESTSELSET, 200, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv2", ID_TESTSEL_BUTTON_RESERVE1, 300, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv3", ID_TESTSEL_BUTTON_RESERVE2, 400, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv4", ID_TESTSEL_BUTTON_RESERVE3, 500, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv5", ID_TESTSEL_BUTTON_RESERVE4, 600, 420, 100, 60, 0, 0x0, 0 },
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
      
    for(i=ID_TESTSEL_BUTTON_0;i<ID_TESTSEL_BUTTON_BOUND;i++){
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        Button_Init(hItem);
        if(i<=ID_TESTSEL_BUTTON_RESERVE4){
            if(i%2){
                BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTGRAY);
            }else{

            }
        }
    }
    
    WM_HideWin(pMsg->hWin);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_TESTSEL_BUTTON_CONFIRM: // Notifications sent by 'btnConfirm'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_WHEEL: // Notifications sent by 'btnWheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_WHEEL_FLOAT: // Notifications sent by 'btnWheelFloat'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:       
          break;
      }
      break;
    case ID_TESTSEL_BUTTON_BUZZER: // Notifications sent by 'btnBuzzer'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST3: // Notifications sent by 'btnTest3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_BRUSH: // Notifications sent by 'btnBrush'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_ASH_TRAY: // Notifications sent by 'btnAshTray'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;

      }
      break;
    case ID_TESTSEL_BUTTON_FAN: // Notifications sent by 'btnFan'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_IFRD: // Notifications sent by 'btnIFRD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
          break;
      }
      break;
    case ID_TESTSEL_BUTTON_COLLISION: // Notifications sent by 'btnCollision'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_UNIWHEEL: // Notifications sent by 'btnUniwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_KEY: // Notifications sent by 'btnKey'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_IRDA: // Notifications sent by 'btnIrDA'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_RGB_LED: // Notifications sent by 'btnRgbLed'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_CHARGE: // Notifications sent by 'btnCharge'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST1: // Notifications sent by 'btnTest1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST2: // Notifications sent by 'btnTest2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST4: // Notifications sent by 'btnTest4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST5: // Notifications sent by 'btnTest5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST6: // Notifications sent by 'btnTest6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TEST7: // Notifications sent by 'btnTest7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_SELALL: // Notifications sent by 'btnSelAll'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_SELNONE: // Notifications sent by 'btnSelNone'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_CANCEL: // Notifications sent by 'btnCancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_SNSET: // Notifications sent by 'btnSnSet'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SNSET_BUTTON_SNSET);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
        BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
        gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;
        WM_HideWin(pMsg->hWin);
        WM_ShowWin(hWin_SWRB_SNSETTING);
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TIMESET: // Notifications sent by 'btnTimeSet'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        hItem = WM_GetDialogItem(hWin_SWRB_TIMESETTING, ID_TIMESET_BUTTON_TIMESET);
        BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
        BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
        gSwrbTestSetState = SWRB_TEST_SET_STATE_TIME;
        WM_HideWin(pMsg->hWin);
        WM_ShowWin(hWin_SWRB_TIMESETTING);
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_TESTSELSET: // Notifications sent by 'btnTestSelSet'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_RESERVE1: // Notifications sent by 'btnRvs1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_TESTSEL_BUTTON_RESERVE2: // Notifications sent by 'btnRvs2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
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

WM_HWIN hWin_SWRB_TESTSEL;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateTestSelSettingDLG
*/

WM_HWIN CreateTestSelSettingDLG(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*************************** End of file ****************************/

