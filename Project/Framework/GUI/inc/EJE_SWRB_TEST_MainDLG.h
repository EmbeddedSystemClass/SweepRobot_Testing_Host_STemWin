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

#include "EJE_SWRB_TEST_StartDLG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_MAIN_CHECKBOX_0       (ID_START_BOUND + 0x01)
#define ID_MAIN_CHECKBOX_1       (ID_START_BOUND + 0x02)
#define ID_MAIN_CHECKBOX_2		(ID_START_BOUND + 0x03)
#define ID_MAIN_CHECKBOX_3		(ID_START_BOUND + 0x04)
#define ID_MAIN_CHECKBOX_4		(ID_START_BOUND + 0x05)
#define ID_MAIN_CHECKBOX_5		(ID_START_BOUND + 0x06)
#define ID_MAIN_CHECKBOX_6		(ID_START_BOUND + 0x07)
#define ID_MAIN_CHECKBOX_7		(ID_START_BOUND + 0x08)
#define ID_MAIN_CHECKBOX_8		(ID_START_BOUND + 0x09)
#define ID_MAIN_CHECKBOX_9		(ID_START_BOUND + 0x0A)
#define ID_MAIN_CHECKBOX_10		(ID_START_BOUND + 0x0B)
#define ID_MAIN_CHECKBOX_11		(ID_START_BOUND + 0x0C)
#define ID_MAIN_CHECKBOX_12		(ID_START_BOUND + 0x0D)
#define ID_MAIN_EDIT_0		(ID_START_BOUND + 0x0F)
#define ID_MAIN_EDIT_1		(ID_START_BOUND + 0x10)
#define ID_MAIN_EDIT_2		(ID_START_BOUND + 0x11)
#define ID_MAIN_EDIT_3		(ID_START_BOUND + 0x12)
#define ID_MAIN_EDIT_4		(ID_START_BOUND + 0x13)
#define ID_MAIN_EDIT_5		(ID_START_BOUND + 0x14)
#define ID_MAIN_EDIT_6		(ID_START_BOUND + 0x15)
#define ID_MAIN_EDIT_7		(ID_START_BOUND + 0x16)
#define ID_MAIN_EDIT_8		(ID_START_BOUND + 0x17)
#define ID_MAIN_EDIT_9		(ID_START_BOUND + 0x18)
#define ID_MAIN_EDIT_10		(ID_START_BOUND + 0x19)
#define ID_MAIN_EDIT_11		(ID_START_BOUND + 0x1A)
#define ID_MAIN_EDIT_12		(ID_START_BOUND + 0x1B)
#define ID_MAIN_EDIT_13		(ID_START_BOUND + 0x1C)
#define ID_MAIN_EDIT_14		(ID_START_BOUND + 0x1D)
#define ID_MAIN_EDIT_15		(ID_START_BOUND + 0x1E)
#define ID_MAIN_EDIT_16		(ID_START_BOUND + 0x1F)
#define ID_MAIN_EDIT_17      (ID_START_BOUND + 0x20)
#define ID_MAIN_BUTTON_1		(ID_START_BOUND + 0x31)
#define ID_MAIN_BUTTON_2		(ID_START_BOUND + 0x32)
#define ID_MAIN_BUTTON_3		(ID_START_BOUND + 0x33)
#define ID_MAIN_BUTTON_4		(ID_START_BOUND + 0x34)
#define ID_MAIN_BUTTON_5     (ID_START_BOUND + 0x35)
#define ID_MAIN_BUTTON_6     (ID_START_BOUND + 0x36)
#define ID_MAIN_BUTTON_7     (ID_START_BOUND + 0x37)
#define ID_MAIN_BUTTON_8     (ID_START_BOUND + 0x38)
#define ID_MAIN_BUTTON_9     (ID_START_BOUND + 0x39)
#define ID_MAIN_BUTTON_10    (ID_START_BOUND + 0x3A)
#define ID_MAIN_BUTTON_11    (ID_START_BOUND + 0x3B)
#define ID_MAIN_FRAMEWIN_0		(ID_START_BOUND + 0x40)
#define ID_MAIN_FRAMEWIN_1       (ID_START_BOUND + 0x41)
#define ID_MAIN_FRAMEWIN_2       (ID_START_BOUND + 0x42)
#define ID_MAIN_MULTIPAGE_0      (ID_START_BOUND + 0x43)
#define ID_MAIN_PROGBAR_0		(ID_START_BOUND + 0x44)
#define ID_MAIN_MULTIEDIT_0		(ID_START_BOUND + 0x45)
#define ID_MAIN_GRAPH_0		(ID_START_BOUND + 0x46)
#define ID_MAIN_WINDOW_0     (ID_START_BOUND + 0x47)
#define ID_MAIN_TEXT_0       (ID_START_BOUND + 0x48)
#define ID_MAIN_TEXT_1       (ID_START_BOUND + 0x49)
#define ID_MAIN_BOUND   (ID_START_BOUND + 0x50)


// USER START (Optionally insert additional defines)
enum MainDLG_BUTTON{
    ID_MAIN_BUTTON_START = ID_MAIN_BUTTON_1,
    ID_MAIN_BUTTON_SET,
    ID_MAIN_BUTTON_STOP,
    ID_MAIN_BUTTON_EXIT,
    ID_MAIN_BUTTON_SET_SN,
    ID_MAIN_BUTTON_SET_TIME,
    ID_MAIN_BUTTON_RGB_LED_OK,
    ID_MAIN_BUTTON_RGB_LED_ERR,
    ID_MAIN_BUTTON_BUZZER_OK,
    ID_MAIN_BUTTON_BUZZER_ERR,
    ID_MAIN_BUTTON_INDICATE,
};

#define ID_MAIN_GRAPH_CURVE              ID_MAIN_GRAPH_0
#define ID_MAIN_MULTIEDIT_MAIN           ID_MAIN_MULTIEDIT_0
#define ID_MAIN_PROGBAR_MAIN             ID_MAIN_PROGBAR_0
#define ID_MAIN_FRAMEWIN_MAIN            ID_MAIN_FRAMEWIN_0
#define ID_MAIN_FRAMEWIN_RGB_LED         ID_MAIN_FRAMEWIN_1
#define ID_MAIN_FRAMEWIN_BUZZER          ID_MAIN_FRAMEWIN_2
#define ID_MAIN_WINDOW_MAIN              ID_MAIN_WINDOW_0
#define ID_MAIN_MULTIPAGE_MAIN           ID_MAIN_MULTIPAGE_0
#define ID_MAIN_TEXT_RGB_LED             ID_MAIN_TEXT_0
#define ID_MAIN_TEXT_BUZZER              ID_MAIN_TEXT_1

enum GUI_ID_MAIN_CHECKBOX{
  ID_MAIN_CHECKBOX_WHEEL = ID_MAIN_CHECKBOX_0,
  ID_MAIN_CHECKBOX_BRUSH,
  ID_MAIN_CHECKBOX_FAN,
  ID_MAIN_CHECKBOX_IFRD,
  ID_MAIN_CHECKBOX_COLLISION,
  ID_MAIN_CHECKBOX_WHEEL_FLOAT,
  ID_MAIN_CHECKBOX_ASH_TRAY,
  ID_MAIN_CHECKBOX_UNIWHEEL,
  ID_MAIN_CHECKBOX_KEY,
  ID_MAIN_CHECKBOX_IRDA,
  ID_MAIN_CHECKBOX_BUZZER,
  ID_MAIN_CHECKBOX_RGB_LED,
  ID_MAIN_CHECKBOX_CHARGE,
  ID_MAIN_CHECKBOX_BOUND,
};

enum GUI_ID_MAIN_EDIT{
    ID_MAIN_EDIT_U1 = ID_MAIN_EDIT_0,
    ID_MAIN_EDIT_U2,
    ID_MAIN_EDIT_U3,
    ID_MAIN_EDIT_U4,
    ID_MAIN_EDIT_U5,
    ID_MAIN_EDIT_U6,
    ID_MAIN_EDIT_U7,
    ID_MAIN_EDIT_U8,
    ID_MAIN_EDIT_D1,
    ID_MAIN_EDIT_D2,
    ID_MAIN_EDIT_D3,
    ID_MAIN_EDIT_D4,
    ID_MAIN_EDIT_D5,
    ID_MAIN_EDIT_D6,
    ID_MAIN_EDIT_D7,
    ID_MAIN_EDIT_D8,
    ID_MAIN_EDIT_SN,
    ID_MAIN_EDIT_DATE,
    ID_MAIN_EDIT_BOUND,
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
*       CreateEJE_SWRB_TEST_MainDLG
*/
WM_HWIN CreateEJE_SWRB_TEST_MainDLG(void);
WM_HWIN CreateRGB_LED_TestDLG(void);
WM_HWIN CreateBUZZER_TestDLG(void);

// USER START (Optionally insert additional public code)
void Button_Set_Text(WM_HWIN hWin, int buttonId, char *str);
void Button_Set_unPressedBkColor(WM_HWIN hWin, int buttonId, GUI_COLOR color);
void Button_SetEnable(WM_HWIN hWin, int buttonId);
void Button_SetDisable(WM_HWIN hWin, int buttonId);
void Progbar_Set_Value(int progbarValue);
void Progbar_Set_Percent(void);
void Edit_Set_Text(WM_HWIN hWin, int editId, char *str);
void Edit_Set_Value(int editId, long editValue);
void Edit_Clear(void);
void Text_Set_Text(WM_HWIN hWin, int textId, char *str);
void Text_Set_Color(WM_HWIN hWin, int textId, GUI_COLOR color);
void Checkbox_Set_Text(WM_HWIN hWin, int checkboxId, char *string);
int Checkbox_Get_State(int checkboxId);
void Checkbox_Set_State(WM_HWIN hWin, int checkboxId, unsigned int checkboxState);
void SWRB_TestCheckboxStateGet(WM_HWIN hWin, int id, int taskPrio);
void SWRB_TestCheckboxStateSet(u8 stateNum);
void SWRB_TestCheckboxEnable(void);
void SWRB_TestCheckboxDisable(void);
void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxcolor);
void Checkbox_Set_TextAlign(int checkboxId, int align);
void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor);
void Checkbox_Set_Box_Back_Color(int checkboxId, GUI_COLOR boxBkColor, int Index);
void MULTIEDIT_Set_Buffer_Size(int size);
void MultiEdit_Set_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Add_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor);

void SweepRobot_MainTestIndicateBtnToggle(void);

// USER END


#endif
/*************************** End of file ****************************/
