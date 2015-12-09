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

#include "sweeprobot_testing.h"

#include "eje_logo_char.h"
#include "sweeprobot_testing_power_station.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

WM_HWIN hWin_SWRB_POWER_STATION;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

//static WM_CALLBACK * _pcbCallbackCharge;

//static int _VolMin = 0, _VolMax = 4095;
//static int _CurMin = 0, _CurMax = 4095;

//GUI_COLOR _ColorBackGround = GUI_RED;
//GUI_COLOR _ColorGrid       = GUI_DARKGRAY;
//GUI_COLOR _ColorLabel      = GUI_WHITE;
//GUI_COLOR _ColorVoltage      = GUI_DARKGREEN;
//GUI_COLOR _ColorCurrent      = GUI_BLUE;

/*********************************************************************
*
*       _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_PS_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
//  { IMAGE_CreateIndirect, "Image", ID_PS_IMAGE_PS, 305, 44, 150, 200, 0, 0, 0 },
  { BUTTON_CreateIndirect, "btnStart", ID_PS_BUTTON_START, 700, 0, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnSet", ID_PS_BUTTON_SET, 700, 120, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnStop", ID_PS_BUTTON_STOP, 700, 240, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnExit", ID_PS_BUTTON_EXIT, 700, 360, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnIndicate", ID_PS_BUTTON_INDICATE, 470, 50, 30, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_LL(0x41)", ID_PS_CHECKBOX_LL, 530, 135, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_LS(0x44)", ID_PS_CHECKBOX_LS, 530, 180, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_L(0x46)", ID_PS_CHECKBOX_L, 530, 225, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_M(0x40)", ID_PS_CHECKBOX_M, 530, 270, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_R(0x45)", ID_PS_CHECKBOX_R, 530, 315, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_RS(0x42)", ID_PS_CHECKBOX_RS, 530, 360, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Sig_RL(0x43)", ID_PS_CHECKBOX_RL, 530, 405, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "24V Out", ID_PS_CHECKBOX_24V, 530, 90, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "PowerStation", ID_PS_CHECKBOX_INDICATE, 530, 40, 120, 30, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "MultiEdit", ID_PS_MULTIEDIT_MAIN, 20, 40, 130, 395, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_PS_GRAPH_MAIN, 170, 40, 340, 360, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PS_PROGBAR_MAIN, 20, 450, 630, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "PowerStation Test", ID_PS_TEXT_TITLE, 20, 8, 200, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "eVoltage", ID_PS_EDIT_VOLTAGE, 250, 410, 80, 25, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "eCurrent", ID_PS_EDIT_CURRENT, 430, 410, 80, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "tVoltage", ID_PS_TEXT_VOLTAGE, 170, 410, 80, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "tCurrent", ID_PS_TEXT_CURRENT, 350, 410, 80, 25, 0, 0x64, 0 },
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
//static const void * _GetImageById(U32 Id, U32 * pSize) {
//  switch (Id) {
//  case ID_PS_IMAGE_0_IMAGE_0:
//    *pSize = sizeof(_acImage_power_station);
//    return (const void *)_acImage_power_station;
//  }
//  return NULL;
//}

static void Button_Init(WM_HWIN hItem)
{
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetSkinClassic(hItem);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
}

///*******************************************************************
//*
//*       _DrawGraph
//*/
//static void _DrawGraph(void) {
//  GUI_RECT Rect;
//  int      xSize;
//  int      ySize;
//  int      x;
//  int      y;

//  WM_GetClientRect(&Rect);
//  xSize = Rect.x1;
//  ySize = Rect.y1;
//  GUI_SetBkColor(GUI_BLACK);
//  GUI_ClearRect(26, 1, xSize- 21, ySize - 21);
//  GUI_SetColor(_ColorGrid);
//  for (y = 20; y < (ySize - 21); y += 20) {
//    int yPos = ySize - 20 - y;
//    GUI_DrawHLine(yPos, 26, 302);
//  }
//  for (x = 40; x < (xSize - 25); x += 40) {
//    int xPos = x + 25;
//    GUI_DrawVLine(xPos, 1, ySize - 21);
//  }
//  GUI_SetColor(_ColorVoltage);
//  GUI_DrawGraph(aVoltageValue, GUI_COUNTOF(aVoltageValue), 26, ySize - 121);
//  GUI_SetColor(_ColorCurrent);
//  GUI_DrawGraph(aCurrentValue, GUI_COUNTOF(aCurrentValue), 26, ySize - 121);
//}

///*********************************************************************
//*
//*       _LabelGraph
//*/
//static void _LabelGraph(void) {
//  GUI_RECT Rect;
//  int      x;
//  int      y;
//  int      xSize;
//  int      ySize;

//  WM_GetClientRect(&Rect);
//  xSize = Rect.x1;
//  ySize = Rect.y1;
//  GUI_SetBkColor(_ColorBackGround);
//  GUI_Clear();
//  GUI_SetColor(_ColorLabel);
//  GUI_SetPenSize(1);
//  GUI_ClearRect(0, (ySize - 21) - ySize, (xSize - 1), (ySize - 1));
//  GUI_DrawRect(25, 0, xSize, ySize - 20);
//  GUI_SetFont(&GUI_Font6x8);
//  for (x = 0; x < (xSize - 20); x += 40) {
//    int xPos = x + 25;
//    GUI_DrawVLine(xPos, (ySize - 20), (ySize - 14));
//    GUI_DispDecAt(x / 40, xPos - 2, (ySize - 9), 1);
//  }
//  for (y = 0; y < ySize - 20; y += 20) {
//    int yPos = ySize - 20 - y;
//    GUI_DrawHLine(yPos, 20, 25);
//    GUI_GotoXY(1, yPos - 4);
//    GUI_DispDecSpace(_VolMin + y, 3);
//  }
//}

///*********************************************************************
//*
//*       _cbCharge
//*
//* Function description
//*   Callback routine of Charge Graph window
//*/
//static void _cbCharge(WM_MESSAGE * pMsg) {
//  switch (pMsg->MsgId) {
//  case WM_PAINT:
//    _LabelGraph();
//    _DrawGraph();
//    return;
//  }
//  if (_pcbCallbackCharge) {
//    _pcbCallbackCharge(pMsg);
//  }
//}


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
//  const void * pData;
  WM_HWIN      hItem;
//  MULTIPAGE_Handle hMultiPage;
//  U32          FileSize;
  int          NCode;
  int          Id;
  int          i;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
//    //
//    // Initialization of 'Image'
//    //
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_IMAGE_PS);
//    pData = _GetImageById(ID_PS_IMAGE_0_IMAGE_0, &FileSize);
//    IMAGE_SetBMP(hItem, pData, FileSize);
    //
    // Initialization of 'btnStart'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_BUTTON_START);
//    BUTTON_SetText(hItem, "START");
    BUTTON_SetText(hItem, " ");
    BUTTON_Disp_Start_CHNStr(pMsg->hWin, ID_PS_BUTTON_START, 18, 43);
    Button_Init(hItem);
    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTBLUE);
    BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_LIGHTBLUE);
    //
    // Initialization of 'btnSet'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_BUTTON_SET);
//    BUTTON_SetText(hItem, "SET");
    BUTTON_SetText(hItem, " ");
    BUTTON_Disp_Set_CHNStr(pMsg->hWin, ID_PS_BUTTON_SET, 18, 43);
    Button_Init(hItem);
    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTCYAN);
    BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_LIGHTCYAN);
    //
    // Initialization of 'btnStop'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_BUTTON_STOP);
//    BUTTON_SetText(hItem, "STOP");
    BUTTON_SetText(hItem, " ");
    BUTTON_Disp_Stop_CHNStr(pMsg->hWin, ID_PS_BUTTON_STOP, 18, 43);
    Button_Init(hItem);
    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTRED);
    BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_LIGHTRED);
    //
    // Initialization of 'btnExit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_BUTTON_EXIT);
//    BUTTON_SetText(hItem, "EXIT");
    BUTTON_SetText(hItem, " ");
    BUTTON_Disp_Exit_CHNStr(pMsg->hWin, ID_PS_BUTTON_EXIT, 18, 43);
    Button_Init(hItem);
    BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTGREEN);
    BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_LIGHTGREEN);
    //
    // Initialization of 'btnIndicate'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_BUTTON_INDICATE);
    WM_BringToTop(hItem);
    BUTTON_SetText(hItem, " ");
    Button_Init(hItem);
    BUTTON_SetFocussable(hItem, 0);
    //
    // Initialization of 'tTitle'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_TEXT_TITLE);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "PowerStation Test");
    //
    // Initialization of 'eVoltage'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_EDIT_VOLTAGE);
    EDIT_SetDecMode(hItem, 0, 0, 65535, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
    EDIT_SetTextAlign(hItem, GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    //
    // Initialization of 'eCurrent'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_EDIT_CURRENT);
    EDIT_SetDecMode(hItem, 0, 0, 65535, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
    EDIT_SetTextAlign(hItem, GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
    //
    // Initialization of 'tVoltage'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_TEXT_VOLTAGE);
    TEXT_SetText(hItem, "Voltage:");
    TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    //
    // Initialization of 'tCurrent'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_TEXT_CURRENT);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
    TEXT_SetText(hItem, "Current:");
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_GRAPH_MAIN);
    GRAPH_SetBorder(hItem, 40, 10, 40, 40);
    GRAPH_SetAutoScrollbar(hItem, GUI_COORD_X, ENABLE);
    GRAPH_SetAutoScrollbar(hItem, GUI_COORD_Y, ENABLE);
    GRAPH_SetGridVis(hItem, ENABLE);
    GRAPH_SetGridDistY(hItem, 20);
//    WM_DisableWindow(hItem);
    //
    // Initialization of 'Checkbox'
    //
    for(i=ID_PS_CHECKBOX_LL;i<=ID_PS_CHECKBOX_INDICATE;i++){
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
        WM_DisableWindow(hItem);
        switch(i){
            case ID_PS_CHECKBOX_LL:
                CHECKBOX_SetText(hItem, "Sig_LL(0x41)");
                break;
            case ID_PS_CHECKBOX_LS:
                CHECKBOX_SetText(hItem, "Sig_LS(0x44)");
                break;
            case ID_PS_CHECKBOX_L:
                CHECKBOX_SetText(hItem, "Sig_L(0x46)");
                break;
            case ID_PS_CHECKBOX_M:
                CHECKBOX_SetText(hItem, "Sig_M(0x40)");
                break;
            case ID_PS_CHECKBOX_R:
                CHECKBOX_SetText(hItem, "Sig_R(0x45)");
                break;
            case ID_PS_CHECKBOX_RS:
                CHECKBOX_SetText(hItem, "Sig_RS(0x42)");
                break;
            case ID_PS_CHECKBOX_RL:
                CHECKBOX_SetText(hItem, "Sig_RL(0x43)");
                break;
            case ID_PS_CHECKBOX_24V:
                CHECKBOX_SetText(hItem, "24V OUT");
                break;
            case ID_PS_CHECKBOX_INDICATE:
                CHECKBOX_SetText(hItem, "PowerStation");
                break;
        }
    }
    //
    // Initialization of 'PROGBAR'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_PROGBAR_MAIN);
    PROGBAR_SetSkinClassic(hItem);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
    //
    // Initialization of 'MULTIEDIT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PS_MULTIEDIT_MAIN);
    MULTIEDIT_SetAutoScrollV(hItem,1);
    MULTIEDIT_SetWrapWord(hItem);
    MULTIEDIT_SetBufferSize(hItem, 2048);
    WM_DisableWindow(hItem);
    //
    //Hide Dialog after initial
    //
    WM_HideWin(pMsg->hWin);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
        case ID_PS_MULTIPAGE_MAIN: // Notifications sent by 'Multipage'
            switch(NCode) {
                case WM_NOTIFICATION_CLICKED:
                break;
                case WM_NOTIFICATION_RELEASED:
                break;
                case WM_NOTIFICATION_MOVED_OUT:
                break;
                case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
    case ID_PS_BUTTON_START: // Notifications sent by 'btnStart'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        SweepRobot_PowerStationTestStartProc();
        break;
      }
      break;
    case ID_PS_BUTTON_SET: // Notifications sent by 'btnSet'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_PS_BUTTON_STOP: // Notifications sent by 'btnStop'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        SweepRobot_PowerStationTestStopProc();
        break;
      }
      break;
    case ID_PS_BUTTON_EXIT: // Notifications sent by 'btnExit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        SweepRobot_PowerStationTestExitProc();
        break;
      }
      break;
    case ID_PS_CHECKBOX_LL: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_LS: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_L: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_M: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_R: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_RS: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_PS_CHECKBOX_RL: // Notifications sent by 'Checkbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
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

int16_t pDataGraph[500] = {0x0100, 0x0010, 0x1100, 0x0011, 0x0101, 0x01101, 0x0000, 0x0010, 0x0011, 0x0110};

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

WM_HWIN CreateEJE_SWRB_TEST_PowerStationDLG(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*************************** End of file ****************************/
