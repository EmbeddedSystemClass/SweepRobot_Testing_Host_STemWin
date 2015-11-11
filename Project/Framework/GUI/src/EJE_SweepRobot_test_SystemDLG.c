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
#include "eje_logo.h"

#include "usart.h"
#include "led.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

// USER START (Optionally insert additional defines)
WM_HWIN hWinEJE_SweepRobot_test_System;

#define ID_IMAGE_0_IMAGE_0 0x00
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
  { FRAMEWIN_CreateIndirect, "EJE_SweepRobot_test_System", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "START", ID_BUTTON_1, 675, 15, 100, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PAUSE", ID_BUTTON_2, 675, 120, 100, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "STOP", ID_BUTTON_3, 675, 225, 100, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "EXIT", ID_BUTTON_4, 675, 330, 100, 90, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect,   "Progbar",        ID_PROGBAR_0, 10, 425, 440, 20, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Msg Multiedit",  ID_MULTIEDIT_0, 10, 60, 440, 250, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxWheel",       ID_CHECKBOX_0, 460, 60, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxBrush",       ID_CHECKBOX_1, 460, 85, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxFan",         ID_CHECKBOX_2, 460, 110, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxIFRD",        ID_CHECKBOX_3, 460, 135, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxCollision",   ID_CHECKBOX_4, 460, 160, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxWheelFloat",  ID_CHECKBOX_5, 460, 185, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxAshTray",     ID_CHECKBOX_6, 460, 210, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxUniWheel",    ID_CHECKBOX_7, 460, 235, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxKey",         ID_CHECKBOX_8, 460, 260, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxIRDA",        ID_CHECKBOX_9, 460, 285, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxBuzzer",      ID_CHECKBOX_10, 460, 310, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxRgbLed",      ID_CHECKBOX_11, 460, 335, 210, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxCharge",      ID_CHECKBOX_12, 460, 360, 210, 20, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 10, 310, 440, 100, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EditU1", ID_EDIT_1, 10, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU2", ID_EDIT_2, 65, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU3", ID_EDIT_3, 120, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU4", ID_EDIT_4, 175, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU5", ID_EDIT_5, 230, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU6", ID_EDIT_6, 285, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU7", ID_EDIT_7, 340, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditU8", ID_EDIT_8, 395, 0, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD1", ID_EDIT_9, 10, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD2", ID_EDIT_10, 65, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD3", ID_EDIT_11, 120, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD4", ID_EDIT_12, 175, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD5", ID_EDIT_13, 230, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD6", ID_EDIT_14, 285, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD7", ID_EDIT_15, 340, 30, 55, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD8", ID_EDIT_16, 395, 30, 55, 30, 0, 0x64, 0 },   
  { EDIT_CreateIndirect, "EditHEX", ID_EDIT_17, 460, 415, 80, 30, 0, 0x64, 0 },
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
*       _onValueChangedDialog
*/
//static void _onValueChangedDialog(WM_HWIN hDlg, int Id)
//{
//	if( Id == ID_CHECKBOX_0){
//		
//	}else if ( Id == ID_CHECKBOX_1 ){
//		
//	}else{
//		
//	}
//}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    u32     i;
    // USER START (Optionally insert additional variables)
    // USER END

  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
    //
    // Initialization of 'START'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'PAUSE'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'STOP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'EXIT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'Msg Multiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "Waiting for Start");
    MULTIEDIT_SetFont(hItem, GUI_FONT_16B_ASCII);
    MULTIEDIT_SetAutoScrollV(hItem,1);
    MULTIEDIT_SetWrapWord(hItem);
    MULTIEDIT_SetBufferSize(hItem, 1024);
    //
    // Initialization of 'cbxWheel Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "WHEEL");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //
    // Initialization of 'cbxSBrush Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "BRUSH");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxFan Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
    CHECKBOX_SetText(hItem, "FAN");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxIFRD Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
    CHECKBOX_SetText(hItem, "IFRD");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxCollision Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
    CHECKBOX_SetText(hItem, "COLLISION");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxWheelFloat Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
    CHECKBOX_SetText(hItem, "WHEEL FLOAT");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxAshTray Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
    CHECKBOX_SetText(hItem, "ASH TRAY");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxUniWheel Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
    CHECKBOX_SetText(hItem, "UNIWHEEL");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxKey Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
    CHECKBOX_SetText(hItem, "KEY");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxIRDA Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9);
    CHECKBOX_SetText(hItem, "IRDA");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxBuzzer Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10);
    CHECKBOX_SetText(hItem, "BUZZER");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxRgbLed Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11);
    CHECKBOX_SetText(hItem, "RGB LED");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxCharge'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_12);
    CHECKBOX_SetText(hItem, "CHARGE");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 20, 5, 5, 5);
    //
    // Initialization of 'Edit'
    //
    for(i=ID_EDIT_U1;i<ID_EDIT_BOUND-1;i++){
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        EDIT_SetFont(hItem, &GUI_Font20_ASCII);
        EDIT_SetDecMode(hItem, 0, 0, 65536, 0, GUI_EDIT_NORMAL);
        EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    }
    //
    // Initialization of 'EditHEX'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_17);
    EDIT_SetFont(hItem, &GUI_Font20_ASCII);
    EDIT_SetDecMode(hItem, 0, 0, 65536, 0, GUI_EDIT_NORMAL);
    EDIT_SetHexMode(hItem, 0, 0, 0xFFFFFFFF);
    // USER START (Optionally insert additional code for further widget initialization)
    //
    // Initialization of 'PROGBAR'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_1: // Notifications sent by 'START'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_TestStartProc();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'PAUSE'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_TestPauseProc();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'STOP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_TestStopProc();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'EXIT'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        SweepRobot_TestExitProc();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_MULTIEDIT_0: // Notifications sent by 'Msg Multiedit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_0: // Notifications sent by 'cbxWheel Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_1: // Notifications sent by 'cbxSBrush Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
//    case ID_CHECKBOX_2: // Notifications sent by 'cbxMBrush Test'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_VALUE_CHANGED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      // USER START (Optionally insert additional code for further notification handling)
//      // USER END
//      }
//      break;
    case ID_CHECKBOX_3: // Notifications sent by 'cbxFan Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_4: // Notifications sent by 'cbxIFRD Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_5: // Notifications sent by 'cbxCollision Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_6: // Notifications sent by 'cbxWheelFloat Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_7: // Notifications sent by 'cbxAshTray Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_8: // Notifications sent by 'cbxUniWheel Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_9: // Notifications sent by 'cbxKey Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_10: // Notifications sent by 'cbxIRDA Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_11: // Notifications sent by 'cbxBuzzer Test'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
		case ID_PROGBAR_0:
			switch(NCode) {
				case WM_NOTIFICATION_VALUE_CHANGED:
					
					break;
			}
			break;
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
*       CreateEJE_SweepRobot_test_System
*/
WM_HWIN CreateEJE_SweepRobot_test_System(void);
WM_HWIN CreateEJE_SweepRobot_test_System(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
void Progbar_Set_Value(int progbarValue)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, ID_PROGBAR_0);
	PROGBAR_SetValue(hItem, progbarValue);
}

void Edit_Set_Value(int editId, long editValue)
{
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, editId);
  EDIT_SetValue(hItem, editValue);
}

void Checkbox_Set_Text(int checkboxId, char *string)
{
  WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetText(hItem, string);
}

void Checkbox_Set_State(int checkboxId , unsigned int checkboxState)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetState(hItem, checkboxState);
}

void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxtextcolor)
{
  WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetTextColor(hItem, checkboxtextcolor);
}

void Checkbox_Set_TextAlign(int checkboxId, int align)
{
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetTextAlign(hItem, align);
}

void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor)
{
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetBkColor(hItem, checkboxbkcolor);
}

void MultiEdit_Set_Text(char *s)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, ID_MULTIEDIT_0);
	MULTIEDIT_SetText(hItem, s);
}

void MultiEdit_Add_Text(char *s)
{
  WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, ID_MULTIEDIT_0);
	MULTIEDIT_AddText(hItem, s);
}

void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor)
{
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, ID_MULTIEDIT_0);
  MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_EDIT, multieditTextColor);
}

GRAPH_DATA_Handle Graph_Data_YT_Create(GUI_COLOR color, u32 maxNumItems, int16_t *pData, u32 numItems)
{
  GRAPH_DATA_Handle hGraphData;
  hGraphData = GRAPH_DATA_YT_Create(color, maxNumItems, pData, numItems);
  return hGraphData;
}


 

// USER END

/*************************** End of file ****************************/