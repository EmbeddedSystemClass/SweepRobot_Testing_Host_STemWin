/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
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
#include "stm32f4xx.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_PROGBAR_0   (GUI_ID_USER + 0x02)
#define ID_MULTIEDIT_0 (GUI_ID_USER + 0x03)
#define ID_CHECKBOX_0  (GUI_ID_USER + 0x04)
#define ID_CHECKBOX_1  (GUI_ID_USER + 0x05)
//#define ID_CHECKBOX_2  (GUI_ID_USER + 0x06)
#define ID_CHECKBOX_3  (GUI_ID_USER + 0x07)
#define ID_CHECKBOX_4  (GUI_ID_USER + 0x08)
#define ID_CHECKBOX_5  (GUI_ID_USER + 0x09)
#define ID_CHECKBOX_6  (GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_7  (GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_8  (GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_9  (GUI_ID_USER + 0x0D)
#define ID_CHECKBOX_10 (GUI_ID_USER + 0x0E)
#define ID_CHECKBOX_11 (GUI_ID_USER + 0x0F)
#define ID_BUTTON_1    (GUI_ID_USER + 0x10)
#define ID_TEXT_0      (GUI_ID_USER + 0x11)
#define ID_IMAGE_0      (GUI_ID_USER + 0x12)

// USER START (Optionally insert additional defines)
#define ID_CHECKBOX_WHEEL         ID_CHECKBOX_0
#define ID_CHECKBOX_BRUSH         ID_CHECKBOX_1
//#define ID_CHECKBOX_MBRUSH        ID_CHECKBOX_2
#define ID_CHECKBOX_FAN           ID_CHECKBOX_3
#define ID_CHECKBOX_IFRD          ID_CHECKBOX_4
#define ID_CHECKBOX_COLLISION     ID_CHECKBOX_5
#define ID_CHECKBOX_WHEEL_FLOAT   ID_CHECKBOX_6
#define ID_CHECKBOX_ASH_TRAY      ID_CHECKBOX_7
#define ID_CHECKBOX_UNIWHEEL      ID_CHECKBOX_8
#define ID_CHECKBOX_KEY           ID_CHECKBOX_9
#define ID_CHECKBOX_IRDA          ID_CHECKBOX_10
#define ID_CHECKBOX_BUZZER        ID_CHECKBOX_11
#define ID_TEXT_CPU               ID_TEXT_0
#define ID_IMAGE_LOGO             ID_IMAGE_0
// USER END

extern WM_HWIN hWinEJE_SweepRobot_test_System;

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
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

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

// USER START (Optionally insert additional public code)
void Progbar_Set_Value(u8 progbarValue);
void Checkbox_Set_Text(int checkboxId, char *string);
void Checkbox_Set_State(int checkboxId , unsigned int checkboxState);
void Checkbox_Set_Color(int checkboxId, GUI_COLOR checkboxcolor);
void Checkbox_Set_TextAlign(int checkboxId, int align);
void MultiEdit_Set_Text(char *s);
void MultiEdit_Add_Text(char *s);
void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor);
//void MultiEdit_Set_CursorCharPos(int x, int y);
// USER END

/*************************** End of file ****************************/
