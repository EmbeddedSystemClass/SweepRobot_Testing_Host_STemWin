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

#include "EJE_SweepRobot_test_SystemDLG.h"
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
  { FRAMEWIN_CreateIndirect,  "EJE_SweepRobot_test_System", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "START", ID_BUTTON_0, 505, 345, 120, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "STOP", ID_BUTTON_1, 655, 345, 120, 100, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect,   "Progbar", ID_PROGBAR_0, 10, 425, 480, 20, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Msg Multiedit", ID_MULTIEDIT_0, 11, 29, 480, 380, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxWheel Test", ID_CHECKBOX_0, 505, 30, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxBrush Test", ID_CHECKBOX_1, 505, 55, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxFan Test", ID_CHECKBOX_3, 505, 80, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxIFRD Test", ID_CHECKBOX_4, 505, 105, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxCollision Test", ID_CHECKBOX_5, 505, 130, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxWheelFloat Test", ID_CHECKBOX_6, 505, 155, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxAshTray Test", ID_CHECKBOX_7, 505, 180, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxUniWheel Test", ID_CHECKBOX_8, 505, 205, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxKey Test", ID_CHECKBOX_9, 505, 230, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxIRDA Test", ID_CHECKBOX_10, 505, 255, 200, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect,  "cbxBuzzer Test", ID_CHECKBOX_11, 505, 280, 200, 20, 0, 0x0, 0 },
  { IMAGE_CreateIndirect,     "ImageLOGO", ID_IMAGE_0, 620, 10, 159, 53, 0, IMAGE_CF_MEMDEV, 0 },
  // USER START (Optionally insert additional widgets)
  { TEXT_CreateIndirect,      "CPU Load", ID_TEXT_0, 680, 10, 100, 30, 0, 0x0, 0 },
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
*       _GetImageById
*/
static const void * _GetImageById(U32 Id, U32 * pSize) {
  switch (Id) {
  case ID_IMAGE_0_IMAGE_0:
    *pSize = sizeof(bmeje_logo.pData);
    return (const void *)bmeje_logo.pData;
  }
  return NULL;
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  const void * pData;
  WM_HWIN hItem;
  U32     FileSize;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'START'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'STOP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //
    // Initialization of 'Msg Multiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "Waiting for Start");
    MULTIEDIT_SetFont(hItem, GUI_FONT_32B_ASCII);
    MULTIEDIT_SetAutoScrollV(hItem,1);
    MULTIEDIT_SetWrapWord(hItem);
    MULTIEDIT_SetBufferSize(hItem, 1024);
    //
    // Initialization of 'cbxWheel Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "Wheel");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //
    // Initialization of 'cbxSBrush Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "Brush");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
//    //
//    // Initialization of 'cbxMBrush Test'
//    //
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
//    CHECKBOX_SetText(hItem, "MBrush");
//    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxFan Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
    CHECKBOX_SetText(hItem, "Fan");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxIFRD Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
    CHECKBOX_SetText(hItem, "IFRD");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxCollision Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
    CHECKBOX_SetText(hItem, "Collision");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxWheelFloat Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
    CHECKBOX_SetText(hItem, "WheelFloat");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxAshTray Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
    CHECKBOX_SetText(hItem, "AshTray");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxUniWheel Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
    CHECKBOX_SetText(hItem, "UniWheel");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxKey Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9);
    CHECKBOX_SetText(hItem, "Key");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxIRDA Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10);
    CHECKBOX_SetText(hItem, "IRDA");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    //
    // Initialization of 'cbxBuzzer Test'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11);
    CHECKBOX_SetText(hItem, "Buzzer");
    CHECKBOX_SetFont(hItem, GUI_FONT_20B_ASCII);
    // USER START (Optionally insert additional code for further widget initialization)
		//
		// Initialization of 'PROGBAR'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //
    //Initialization of 'TEXT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_Font16_ASCII);
    TEXT_SetText(hItem, "CPU Load");
    //
    // Initialization of 'Image'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
    IMAGE_SetBMP(hItem, pData, FileSize);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'START'
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
    case ID_BUTTON_1: // Notifications sent by 'STOP'
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
void Progbar_Set_Value(u8 progbarValue)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, ID_PROGBAR_0);
	PROGBAR_SetValue(hItem, progbarValue);
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

void Checkbox_Set_Color(int checkboxId, GUI_COLOR checkboxcolor)
{
  WM_HWIN hItem;
	hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetTextColor(hItem, checkboxcolor);
}

void Checkbox_Set_TextAlign(int checkboxId, int align)
{
  WM_HWIN hItem;
  hItem = WM_GetDialogItem(hWinEJE_SweepRobot_test_System, checkboxId);
  CHECKBOX_SetTextAlign(hItem, align);
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

// USER END

/*************************** End of file ****************************/
