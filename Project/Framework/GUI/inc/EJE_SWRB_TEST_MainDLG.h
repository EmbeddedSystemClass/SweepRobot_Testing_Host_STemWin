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

#ifndef __EJE_SWRB_TEST_MAINDLG_H
#define __EJE_SWRB_TEST_MAINDLG_H

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


#define ID_CHECKBOX_0       (GUI_ID_USER + 0x01)
#define ID_CHECKBOX_1       (GUI_ID_USER + 0x02)
#define ID_CHECKBOX_2		(GUI_ID_USER + 0x03)
#define ID_CHECKBOX_3		(GUI_ID_USER + 0x04)
#define ID_CHECKBOX_4		(GUI_ID_USER + 0x05)
#define ID_CHECKBOX_5		(GUI_ID_USER + 0x06)
#define ID_CHECKBOX_6		(GUI_ID_USER + 0x07)
#define ID_CHECKBOX_7		(GUI_ID_USER + 0x08)
#define ID_CHECKBOX_8		(GUI_ID_USER + 0x09)
#define ID_CHECKBOX_9		(GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_10		(GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_11		(GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_12		(GUI_ID_USER + 0x0D)
#define ID_EDIT_0		(GUI_ID_USER + 0x0F)
#define ID_EDIT_1		(GUI_ID_USER + 0x10)
#define ID_EDIT_2		(GUI_ID_USER + 0x11)
#define ID_EDIT_3		(GUI_ID_USER + 0x12)
#define ID_EDIT_4		(GUI_ID_USER + 0x13)
#define ID_EDIT_5		(GUI_ID_USER + 0x14)
#define ID_EDIT_6		(GUI_ID_USER + 0x15)
#define ID_EDIT_7		(GUI_ID_USER + 0x16)
#define ID_EDIT_8		(GUI_ID_USER + 0x17)
#define ID_EDIT_9		(GUI_ID_USER + 0x18)
#define ID_EDIT_10		(GUI_ID_USER + 0x19)
#define ID_EDIT_11		(GUI_ID_USER + 0x1A)
#define ID_EDIT_12		(GUI_ID_USER + 0x1B)
#define ID_EDIT_13		(GUI_ID_USER + 0x1C)
#define ID_EDIT_14		(GUI_ID_USER + 0x1D)
#define ID_EDIT_15		(GUI_ID_USER + 0x1E)
#define ID_EDIT_16		(GUI_ID_USER + 0x1F)
#define ID_EDIT_17      (GUI_ID_USER + 0x20)
#define ID_BUTTON_1		(GUI_ID_USER + 0x31)
#define ID_BUTTON_2		(GUI_ID_USER + 0x32)
#define ID_BUTTON_3		(GUI_ID_USER + 0x33)
#define ID_BUTTON_4		(GUI_ID_USER + 0x34)
#define ID_BUTTON_5     (GUI_ID_USER + 0x35)
#define ID_BUTTON_6     (GUI_ID_USER + 0x36)
#define ID_BUTTON_7     (GUI_ID_USER + 0x37)
#define ID_BUTTON_8     (GUI_ID_USER + 0x38)
#define ID_FRAMEWIN_0		(GUI_ID_USER + 0x3A)
#define ID_FRAMEWIN_1       (GUI_ID_USER + 0x3B)
#define ID_FRAMEWIN_2       (GUI_ID_USER + 0x3C)
#define ID_MULTIPAGE_0      (GUI_ID_USER + 0x40)
#define ID_PROGBAR_0		(GUI_ID_USER + 0x41)
#define ID_MULTIEDIT_0		(GUI_ID_USER + 0x42)
#define ID_GRAPH_0		(GUI_ID_USER + 0x43)
#define ID_WINDOW_0     (GUI_ID_USER + 0x44)
#define ID_TEXT_0       (GUI_ID_USER + 0x45)
#define ID_TEXT_1       (GUI_ID_USER + 0x46)
#define ID_MAIN_BOUND   (GUI_ID_USER + 0x50)


// USER START (Optionally insert additional defines)
enum MainDLG_BUTTON{
    ID_BUTTON_START = ID_BUTTON_1,
    ID_BUTTON_SET,
    ID_BUTTON_STOP,
    ID_BUTTON_EXIT,
    ID_BUTTON_RGB_LED_OK,
    ID_BUTTON_RGB_LED_ERR,
    ID_BUTTON_BUZZER_OK,
    ID_BUTTON_BUZZER_ERR,
};

#define ID_GRAPH_CURVE              ID_GRAPH_0
#define ID_MULTIEDIT_MAIN           ID_MULTIEDIT_0
#define ID_PROGBAR_MAIN             ID_PROGBAR_0
#define ID_FRAMEWIN_MAIN            ID_FRAMEWIN_0
#define ID_FRAMEWIN_RGB_LED         ID_FRAMEWIN_1
#define ID_FRAMEWIN_BUZZER          ID_FRAMEWIN_2
#define ID_WINDOW_MAIN              ID_WINDOW_0
#define ID_MULTIPAGE_MAIN           ID_MULTIPAGE_0
#define ID_TEXT_RGB_LED             ID_TEXT_0
#define ID_TEXT_BUZZER              ID_TEXT_1

enum GUI_ID_CHECKBOX{
  ID_CHECKBOX_WHEEL = ID_CHECKBOX_0,
  ID_CHECKBOX_BRUSH,
  ID_CHECKBOX_FAN,
  ID_CHECKBOX_IFRD,
  ID_CHECKBOX_COLLISION,
  ID_CHECKBOX_WHEEL_FLOAT,
  ID_CHECKBOX_ASH_TRAY,
  ID_CHECKBOX_UNIWHEEL,
  ID_CHECKBOX_KEY,
  ID_CHECKBOX_IRDA,
  ID_CHECKBOX_BUZZER,
  ID_CHECKBOX_RGB_LED,
  ID_CHECKBOX_CHARGE,
  ID_CHECKBOX_BOUND,
};

enum GUI_ID_EDIT{
    ID_EDIT_U1 = ID_EDIT_0,
    ID_EDIT_U2,
    ID_EDIT_U3,
    ID_EDIT_U4,
    ID_EDIT_U5,
    ID_EDIT_U6,
    ID_EDIT_U7,
    ID_EDIT_U8,
    ID_EDIT_D1,
    ID_EDIT_D2,
    ID_EDIT_D3,
    ID_EDIT_D4,
    ID_EDIT_D5,
    ID_EDIT_D6,
    ID_EDIT_D7,
    ID_EDIT_D8,
    ID_EDIT_SN,
    ID_EDIT_DATE,
    ID_EDIT_BOUND,
};

// USER END

extern WM_HWIN hWin_SWRB_MAIN;
extern WM_HWIN hWin_SWRB_RGB_LED;
extern WM_HWIN hWin_SWRB_BUZZER;

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
WM_HWIN CreateRGB_LED_TestDLG(void);
WM_HWIN CreateBUZZER_TestDLG(void);

// USER START (Optionally insert additional public code)
void Button_Set_Text(int buttonId, char *str);
void Button_Set_unPressedBkColor(WM_HWIN hWin, int buttonId, GUI_COLOR color);
void Progbar_Set_Value(int progbarValue);
void Progbar_Set_Percent(u8 teststate);
void Edit_Set_Text(WM_HWIN hWin, int editId, char *str);
void Edit_Set_Value(int editId, long editValue);
void Edit_Clear(void);
void Text_Set_Text(WM_HWIN hWin, int textId, char *str);
void Text_Set_Color(WM_HWIN hWin, int textId, GUI_COLOR color);
void Checkbox_Set_Text(int checkboxId, char *string);
int Checkbox_Get_State(int checkboxId);
void SWRB_TestCheckboxStateGet(WM_HWIN hWin, int id, int taskPrio);
void Checkbox_Set_State(int checkboxId , unsigned int checkboxState);
void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxcolor);
void Checkbox_Set_TextAlign(int checkboxId, int align);
void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor);
void Checkbox_Set_Box_Back_Color(int checkboxId, GUI_COLOR boxBkColor, int Index);
void MULTIEDIT_Set_Buffer_Size(int size);
void MultiEdit_Set_Text(char *s);
void MultiEdit_Add_Text(char *s);
void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor);
//void MultiEdit_Set_CursorCharPos(int x, int y);
// USER END


#endif
/*************************** End of file ****************************/
