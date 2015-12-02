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
#ifndef __EJE_SWRB_TEST_TIMESETTINGDLG_H
#define __EJE_SWRB_TEST_TIMESETTINGDLG_H

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

#include "EJE_SWRB_TEST_SNSettingDLG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_TIMESET_LISTWHEEL_0 (ID_SET_BOUND + 0x00)
#define ID_TIMESET_LISTWHEEL_1 (ID_SET_BOUND + 0x01)
#define ID_TIMESET_LISTWHEEL_2 (ID_SET_BOUND + 0x02)
#define ID_TIMESET_LISTWHEEL_3 (ID_SET_BOUND + 0x03)
#define ID_TIMESET_LISTWHEEL_4 (ID_SET_BOUND + 0x04)
#define ID_TIMESET_LISTWHEEL_5 (ID_SET_BOUND + 0x05)
#define ID_TIMESET_EDIT_0 (ID_SET_BOUND + 0x10)
#define ID_TIMESET_EDIT_1 (ID_SET_BOUND + 0x11)
#define ID_TIMESET_EDIT_2 (ID_SET_BOUND + 0x12)
#define ID_TIMESET_EDIT_3 (ID_SET_BOUND + 0x13)
#define ID_TIMESET_EDIT_4 (ID_SET_BOUND + 0x14)
#define ID_TIMESET_EDIT_5 (ID_SET_BOUND + 0x15)
#define ID_TIMESET_EDIT_6 (ID_SET_BOUND + 0x16)
#define ID_TIMESET_EDIT_7 (ID_SET_BOUND + 0x17)
#define ID_TIMESET_FRAMEWIN_0 (ID_SET_BOUND + 0x20)
#define ID_TIMESET_WINDOW_0 (ID_SET_BOUND + 0x21)
#define ID_TIMESET_BUTTON_0 (ID_SET_BOUND + 0x30)
#define ID_TIMESET_BUTTON_1 (ID_SET_BOUND + 0x31)
#define ID_TIMESET_BUTTON_2 (ID_SET_BOUND + 0x32)
#define ID_TIMESET_BUTTON_3 (ID_SET_BOUND + 0x33)
#define ID_TIMESET_BUTTON_4 (ID_SET_BOUND + 0x34)
#define ID_TIMESET_BUTTON_5 (ID_SET_BOUND + 0x35)
#define ID_TIMESET_BUTTON_6 (ID_SET_BOUND + 0x36)
#define ID_TIMESET_BUTTON_7 (ID_SET_BOUND + 0x37)
#define ID_TIMESET_BUTTON_8 (ID_SET_BOUND + 0x38)
#define ID_TIMESET_TEXT_0 (ID_SET_BOUND + 0x40)
#define ID_TIMESET_BOUND (ID_SET_BOUND + 0x50)

#define ID_TIMESET_LISTWHEEL_YEAR   ID_TIMESET_LISTWHEEL_0
#define ID_TIMESET_LISTWHEEL_MONTH  ID_TIMESET_LISTWHEEL_1
#define ID_TIMESET_LISTWHEEL_DAY    ID_TIMESET_LISTWHEEL_2
#define ID_TIMESET_LISTWHEEL_HOUR   ID_TIMESET_LISTWHEEL_3
#define ID_TIMESET_LISTWHEEL_MIN    ID_TIMESET_LISTWHEEL_4
#define ID_TIMESET_LISTWHEEL_SEC    ID_TIMESET_LISTWHEEL_5
#define ID_TIMESET_EDIT_SETVALUE    ID_TIMESET_EDIT_0
#define ID_TIMESET_EDIT_ACTVALUE    ID_TIMESET_EDIT_1
#define ID_TIMESET_EDIT_YEAR        ID_TIMESET_EDIT_2
#define ID_TIMESET_EDIT_MONTH       ID_TIMESET_EDIT_3
#define ID_TIMESET_EDIT_DAY         ID_TIMESET_EDIT_4
#define ID_TIMESET_EDIT_HOUR        ID_TIMESET_EDIT_5
#define ID_TIMESET_EDIT_MIN         ID_TIMESET_EDIT_6
#define ID_TIMESET_EDIT_SEC         ID_TIMESET_EDIT_7
#define ID_TIMESET_BUTTON_CONFIRM   ID_TIMESET_BUTTON_0
#define ID_TIMESET_BUTTON_CHECK     ID_TIMESET_BUTTON_1
#define ID_TIMESET_BUTTON_RESET     ID_TIMESET_BUTTON_2
#define ID_TIMESET_BUTTON_CANCEL    ID_TIMESET_BUTTON_3
#define ID_TIMESET_BUTTON_SNSET     ID_TIMESET_BUTTON_4
#define ID_TIMESET_BUTTON_TIMESET   ID_TIMESET_BUTTON_5
#define ID_TIMESET_BUTTON_RESERVE1  ID_TIMESET_BUTTON_6
#define ID_TIMESET_BUTTON_RESERVE2  ID_TIMESET_BUTTON_7
#define ID_TIMESET_BUTTON_RESERVE3  ID_TIMESET_BUTTON_8
#define ID_TIMESET_FRAMEWIN         ID_TIMESET_FRAMEWIN_0
#define ID_TIMESET_WINDOW           ID_TIMESET_WINDOW_0

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

extern WM_HWIN hWin_SWRB_TIMESETTING;

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
WM_HWIN CreateTimeSettingDLG(void);

// USER START (Optionally insert additional public code)
// USER END


#endif
/*************************** End of file ****************************/
