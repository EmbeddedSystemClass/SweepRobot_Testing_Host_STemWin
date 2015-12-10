/******************** (C) COPYRIGHT 2008 EJE ***********************************************************
* File Name          : EJE_SWRB_TEST_DLG_Conf.h
* Author             : MeredithRowe@163.com
* Version            : V1.0
* Date               : 12/10/2015
* Description        : STemWin Widget ID definition and User GUI function declaration
*********************************************************************************************************/

#ifndef __EJE_SWRB_TEST_DLG_CONF_H
#define __EJE_SWRB_TEST_DLG_CONF_H

#define __EJE_SWRB_TEST_START_DLG           1
#define __EJE_SWRB_TEST_POWERSTATION_DLG    1
#define __EJE_SWRB_TEST_MAIN_DLG            1
#define __EJE_SWRB_TEST_SNSETTING_DLG       1
#define __EJE_SWRB_TEST_TIMESETTING_DLG     1
#define __EJE_SWRB_TEST_LOGIN_DLG           1
#define __EJE_SWRB_TEST_NUMPAD_DLG          1
#define __EJE_SWRB_TEST_DECRYPTO_DLG        0

#include "stm32f4xx.h"
#include "DIALOG.h"

#if __EJE_SWRB_TEST_START_DLG
    #include "EJE_SWRB_TEST_StartDLG.h"
#endif

#if __EJE_SWRB_TEST_POWERSTATION_DLG
    #include "EJE_SWRB_TEST_PowerStationDLG.h"
#endif

#if __EJE_SWRB_TEST_MAIN_DLG
    #include "EJE_SWRB_TEST_MainDLG.h"
#endif

#if __EJE_SWRB_TEST_SNSETTING_DLG
    #include "EJE_SWRB_TEST_SNSettingDLG.h"
#endif

#if __EJE_SWRB_TEST_TIMESETTING_DLG
    #include "EJE_SWRB_TEST_TimeSettingDLG.h"
#endif

#if __EJE_SWRB_TEST_LOGIN_DLG
    #include "EJE_SWRB_TEST_LoginDLG.h"
#endif

#if __EJE_SWRB_TEST_NUMPAD_DLG
    #include "EJE_SWRB_TEST_NumPadDLG.h"
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/* EJE_SWRB_TEST_StartDLG */
#if __EJE_SWRB_TEST_START_DLG

    #define ID_START_IMAGE_EJE_LOGO 0x00

    enum ID_START_WIDGET{
        ID_START_WINDOW_0 = GUI_ID_USER,
        ID_START_IMAGE_0,
        ID_START_BUTTON_0,
        ID_START_BUTTON_1,
        ID_START_BUTTON_2,
        ID_START_TEXT_0,
        ID_START_TEXT_1,
        ID_START_BOUND,
    };

    enum ID_START_WINDOW{
        ID_START_WINDOW_MAIN = ID_START_WINDOW_0,
        ID_START_WINDOW_BOUND,
    };

    enum ID_START_TEXT{
        ID_START_TEXT_VERSION = ID_START_TEXT_0,
        ID_START_TEXT_TITLE,
        ID_START_TEXT_BOUND,
    };
    
    enum ID_START_IMAGE{
        ID_START_IMAGE_LOGO = ID_START_IMAGE_0,
        ID_START_IMAGE_BOUND,
    };

    enum ID_START_BUTTON{
        ID_START_BUTTON_PCB_TEST = ID_START_BUTTON_0,
        ID_START_BUTTON_POWER_STATION,
        ID_START_BUTTON_DECRYPTO,
        ID_START_BUTTON_BOUND,
    };
#endif

/* EJE_SWRB_TEST_PowerStationDLG */
#ifdef __EJE_SWRB_TEST_POWERSTATION_DLG
    
    enum ID_PS_WIDGET{
        ID_PS_WINDOW_0 = ID_START_BOUND,
        ID_PS_BUTTON_0,
        ID_PS_BUTTON_1,
        ID_PS_BUTTON_2,
        ID_PS_BUTTON_3,
        ID_PS_BUTTON_4,
        ID_PS_CHECKBOX_0,
        ID_PS_CHECKBOX_1,
        ID_PS_CHECKBOX_2,
        ID_PS_CHECKBOX_3,
        ID_PS_CHECKBOX_4,
        ID_PS_CHECKBOX_5,
        ID_PS_CHECKBOX_6,
        ID_PS_CHECKBOX_7,
        ID_PS_CHECKBOX_8,
        ID_PS_MULTIEDIT_0,
        ID_PS_GRAPH_0,
        ID_PS_PROGBAR_0,
        ID_PS_TEXT_0,
        ID_PS_TEXT_1,
        ID_PS_TEXT_2,
        ID_PS_EDIT_0,
        ID_PS_EDIT_1,
        ID_PS_BOUND,
    };

    enum ID_PS_WINDOW{
        ID_PS_WINDOW_MAIN = ID_PS_WINDOW_0,
        ID_PS_WINDOW_BOUND,
    };

    enum ID_PS_BUTTON{
        ID_PS_BUTTON_START = ID_PS_BUTTON_0,
        ID_PS_BUTTON_SET,
        ID_PS_BUTTON_STOP,
        ID_PS_BUTTON_EXIT,
        ID_PS_BUTTON_INDICATE,
        ID_PS_BUTTON_BOUND,
    };

    enum ID_PS_CHECKBOX{
        ID_PS_CHECKBOX_LL = ID_PS_CHECKBOX_0,
        ID_PS_CHECKBOX_LS,
        ID_PS_CHECKBOX_L,
        ID_PS_CHECKBOX_M,
        ID_PS_CHECKBOX_R,
        ID_PS_CHECKBOX_RS,
        ID_PS_CHECKBOX_RL,
        ID_PS_CHECKBOX_24V,
        ID_PS_CHECKBOX_INDICATE,
        ID_PS_CHECKBOX_BOUND,
    };
    
    enum ID_PS_MULTIEDIT{
        ID_PS_MULTIEDIT_MAIN = ID_PS_MULTIEDIT_0,
        ID_PS_MULTIEDIT_BOUND,
    };
    
    enum ID_PS_GRAPH{
        ID_PS_GRAPH_MAIN = ID_PS_GRAPH_0,
        ID_PS_GRAPH_BOUND,
    };

    enum ID_PS_PROGBAR{
        ID_PS_PROGBAR_MAIN = ID_PS_PROGBAR_0,
        ID_PS_PROGBAR_BOUND,
    };
    
    enum ID_PS_TEXT{
        ID_PS_TEXT_TITLE = ID_PS_TEXT_0,
        ID_PS_TEXT_VOLTAGE,
        ID_PS_TEXT_CURRENT,
        ID_PS_TEXT_BOUND,
    };
    
    enum ID_PS_EDIT{
        ID_PS_EDIT_VOLTAGE = ID_PS_EDIT_0,
        ID_PS_EDIT_CURRENT,
        ID_PS_EDIT_BOUND,
    };
#endif
    
/* EJE_SWRB_TEST_MainDLG */
#ifdef __EJE_SWRB_TEST_MAIN_DLG
    
    enum ID_MAIN_WIDGET{
        ID_MAIN_CHECKBOX_0 = ID_PS_BOUND,
        ID_MAIN_CHECKBOX_1,
        ID_MAIN_CHECKBOX_2,
        ID_MAIN_CHECKBOX_3,
        ID_MAIN_CHECKBOX_4,
        ID_MAIN_CHECKBOX_5,
        ID_MAIN_CHECKBOX_6,
        ID_MAIN_CHECKBOX_7,
        ID_MAIN_CHECKBOX_8,
        ID_MAIN_CHECKBOX_9,
        ID_MAIN_CHECKBOX_10,
        ID_MAIN_CHECKBOX_11,
        ID_MAIN_CHECKBOX_12,
        ID_MAIN_EDIT_0,
        ID_MAIN_EDIT_1,
        ID_MAIN_EDIT_2,
        ID_MAIN_EDIT_3,
        ID_MAIN_EDIT_4,
        ID_MAIN_EDIT_5,
        ID_MAIN_EDIT_6,
        ID_MAIN_EDIT_7,
        ID_MAIN_EDIT_8,
        ID_MAIN_EDIT_9,
        ID_MAIN_EDIT_10,
        ID_MAIN_EDIT_11,
        ID_MAIN_EDIT_12,
        ID_MAIN_EDIT_13,
        ID_MAIN_EDIT_14,
        ID_MAIN_EDIT_15,
        ID_MAIN_EDIT_16,
        ID_MAIN_EDIT_17,
        ID_MAIN_EDIT_18,
        ID_MAIN_BUTTON_0,
        ID_MAIN_BUTTON_1,
        ID_MAIN_BUTTON_2,
        ID_MAIN_BUTTON_3,
        ID_MAIN_BUTTON_4,
        ID_MAIN_BUTTON_5,
        ID_MAIN_BUTTON_6,
        ID_MAIN_BUTTON_7,
        ID_MAIN_BUTTON_8,
        ID_MAIN_BUTTON_9,
        ID_MAIN_BUTTON_10,
        ID_MAIN_BUTTON_11,
        ID_MAIN_BUTTON_12,
        ID_MAIN_FRAMEWIN_0,
        ID_MAIN_FRAMEWIN_1,
        ID_MAIN_PROGBAR_0,
        ID_MAIN_MULTIEDIT_0,
        ID_MAIN_WINDOW_0,
        ID_MAIN_TEXT_0,
        ID_MAIN_TEXT_1,
        ID_MAIN_BOUND,
    };
    
    enum ID_MAIN_BUTTON{
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
        ID_MAIN_BUTTON_LOGIN_OK,
        ID_MAIN_BUTTON_LOGIN_CANCEL,
        ID_MAIN_BUTTON_INDICATE,
        ID_MAIN_BUTTON_BOUND,
    };
    
    enum ID_MAIN_FRAMEWIN{
        ID_MAIN_FRAMEWIN_RGB_LED = ID_MAIN_FRAMEWIN_0,
        ID_MAIN_FRAMEWIN_BUZZER,
        ID_MAIN_FRAMEWIN_BOUND,
    };
    
    enum ID_MAIN_MULTIEDIT{
        ID_MAIN_MULTIEDIT_MAIN = ID_MAIN_MULTIEDIT_0,
        ID_MAIN_MULTIEDIT_BOUND,
    };
    
    enum ID_MAIN_PROGBAR{
        ID_MAIN_PROGBAR_MAIN = ID_MAIN_PROGBAR_0,
        ID_MAIN_PROGBAR_BOUND,
    };

    enum ID_MAIN_WINDOW{
        ID_MAIN_WINDOW_MAIN = ID_MAIN_WINDOW_0,
        ID_MAIN_WINDOW_BOUND,
    };
    
    enum ID_MAIN_TEXT{
        ID_MAIN_TEXT_RGB_LED = ID_MAIN_TEXT_0,
        ID_MAIN_TEXT_BUZZER,
        ID_MAIN_TEXT_BOUND,
    };

    enum ID_MAIN_CHECKBOX{
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

    enum ID_MAIN_EDIT{
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
        ID_MAIN_EDIT_LOGIN,
        ID_MAIN_EDIT_BOUND,
    };
#endif

/* EJE_SWRB_TEST_SNSettingDLG */    
#ifdef __EJE_SWRB_TEST_SNSETTING_DLG
    
    enum ID_SNSET_WIDGET{
        ID_SNSET_LISTWHEEL_0 = ID_MAIN_BOUND,
        ID_SNSET_LISTWHEEL_1,
        ID_SNSET_LISTWHEEL_2,
        ID_SNSET_LISTWHEEL_3,
        ID_SNSET_LISTWHEEL_4,
        ID_SNSET_LISTWHEEL_5,
        ID_SNSET_EDIT_0,
        ID_SNSET_EDIT_1,
        ID_SNSET_EDIT_2,
        ID_SNSET_EDIT_3,
        ID_SNSET_EDIT_4,
        ID_SNSET_EDIT_5,
        ID_SNSET_EDIT_6,
        ID_SNSET_EDIT_7,
        ID_SNSET_BUTTON_0,
        ID_SNSET_BUTTON_1,
        ID_SNSET_BUTTON_2,
        ID_SNSET_BUTTON_3,
        ID_SNSET_BUTTON_4,
        ID_SNSET_BUTTON_5,
        ID_SNSET_BUTTON_6,
        ID_SNSET_BUTTON_7,
        ID_SNSET_BUTTON_8,
        ID_SNSET_WINDOW_0,
        ID_SNSET_TEXT_0,
        ID_SNSET_BOUND,
    };
    
    enum GUI_ID_SNSET_LISTWHEEL{
        ID_SNSET_LISTWHEEL_YEAR = ID_SNSET_LISTWHEEL_0,
        ID_SNSET_LISTWHEEL_MONTH,
        ID_SNSET_LISTWHEEL_DATE,
        ID_SNSET_LISTWHEEL_SN1,
        ID_SNSET_LISTWHEEL_SN2,
        ID_SNSET_LISTWHEEL_SN3,
        ID_SNSET_LISTWHEEL_BOUND,
    };
    
    enum ID_SNSET_EDIT{
        ID_SNSET_EDIT_COMB_PREVIOUS = ID_SNSET_EDIT_0,
        ID_SNSET_EDIT_COMB_SET,
        ID_SNSET_EDIT_YEAR,
        ID_SNSET_EDIT_MONTH,
        ID_SNSET_EDIT_DAY,
        ID_SNSET_EDIT_SN1,
        ID_SNSET_EDIT_SN2,
        ID_SNSET_EDIT_SN3,
        ID_SNSET_EDIT_BOUND,
    };
    
    enum ID_SNSET_BUTTON{
        ID_SNSET_BUTTON_CONFIRM = ID_SNSET_BUTTON_0,
        ID_SNSET_BUTTON_CHECK,
        ID_SNSET_BUTTON_RESET,
        ID_SNSET_BUTTON_CANCEL,
        ID_SNSET_BUTTON_SNSET,
        ID_SNSET_BUTTON_TIMESET,
        ID_SNSET_BUTTON_RESERVE1,
        ID_SNSET_BUTTON_RESERVE2,
        ID_SNSET_BUTTON_RESERVE3,
        ID_SNSET_BUTTON_BOUND,
    };
    
    enum ID_SNSET_WINDOW{
        ID_SNSET_WINDOW_MAIN = ID_SNSET_WINDOW_0,
        ID_SNSET_WINDOW_BOUND,
    };
    
    enum ID_SNSET_TEXT{
        ID_SNSET_TEXT_TITLE = ID_SNSET_TEXT_0,
        ID_SNSET_TEXT_BOUND,
    };
#endif

/* EJE_SWRB_TEST_TimeSettingDLG */    
#ifdef __EJE_SWRB_TEST_TIMESETTING_DLG
    
    enum ID_TIMESET_WIDGET{
        ID_TIMESET_LISTWHEEL_0 = ID_SNSET_BOUND,
        ID_TIMESET_LISTWHEEL_1,
        ID_TIMESET_LISTWHEEL_2,
        ID_TIMESET_LISTWHEEL_3,
        ID_TIMESET_LISTWHEEL_4,
        ID_TIMESET_LISTWHEEL_5,
        ID_TIMESET_EDIT_0,
        ID_TIMESET_EDIT_1,
        ID_TIMESET_EDIT_2,
        ID_TIMESET_EDIT_3,
        ID_TIMESET_EDIT_4,
        ID_TIMESET_EDIT_5,
        ID_TIMESET_EDIT_6,
        ID_TIMESET_EDIT_7,
        ID_TIMESET_BUTTON_0,
        ID_TIMESET_BUTTON_1,
        ID_TIMESET_BUTTON_2,
        ID_TIMESET_BUTTON_3,
        ID_TIMESET_BUTTON_4,
        ID_TIMESET_BUTTON_5,
        ID_TIMESET_BUTTON_6,
        ID_TIMESET_BUTTON_7,
        ID_TIMESET_BUTTON_8,
        ID_TIMESET_WINDOW_0,
        ID_TIMESET_TEXT_0,
        ID_TIMESET_BOUND,
    };
    
    enum ID_TIMESET_LISTWHEEL{
        ID_TIMESET_LISTWHEEL_YEAR = ID_TIMESET_LISTWHEEL_0,
        ID_TIMESET_LISTWHEEL_MONTH,
        ID_TIMESET_LISTWHEEL_DAY,
        ID_TIMESET_LISTWHEEL_HOUR,
        ID_TIMESET_LISTWHEEL_MIN,
        ID_TIMESET_LISTWHEEL_SEC,
        ID_TIMESET_LISTWHEEL_BOUND,
    };

    enum ID_TIMESET_EDIT{
        ID_TIMESET_EDIT_SETVALUE = ID_TIMESET_EDIT_0,
        ID_TIMESET_EDIT_ACTVALUE,
        ID_TIMESET_EDIT_YEAR,
        ID_TIMESET_EDIT_MONTH,
        ID_TIMESET_EDIT_DAY,
        ID_TIMESET_EDIT_HOUR,
        ID_TIMESET_EDIT_MIN,
        ID_TIMESET_EDIT_SEC,
        ID_TIMESET_EDIT_BOUND,
    };
    
    enum ID_TIMESET_BUTTON{
        ID_TIMESET_BUTTON_CONFIRM = ID_TIMESET_BUTTON_0,
        ID_TIMESET_BUTTON_CHECK,
        ID_TIMESET_BUTTON_RESET,
        ID_TIMESET_BUTTON_CANCEL,
        ID_TIMESET_BUTTON_SNSET,
        ID_TIMESET_BUTTON_TIMESET,
        ID_TIMESET_BUTTON_RESERVE1,
        ID_TIMESET_BUTTON_RESERVE2,
        ID_TIMESET_BUTTON_RESERVE3,
        ID_TIMESET_BUTTON_BOUND,
    };
    
    enum ID_TIMESET_TEXT{
        ID_TIMESET_TEXT_TITLE = ID_TIMESET_TEXT_0,
        ID_TIMESET_TEXT_BOUND,
    };
    
    enum ID_TIMESET_WINDOW{
        ID_TIMESET_WINDOW_MAIN = ID_TIMESET_WINDOW_0,
        ID_TIMESET_WINDOW_BOUND,
    };
#endif

/* EJE_SWRB_TEST_LoginDLG */    
#ifdef __EJE_SWRB_TEST_LOGIN_DLG
    
    enum ID_LOGIN_WIDGET{
        ID_LOGIN_FRAMEWIN_0 = ID_TIMESET_BOUND,
        ID_LOGIN_BUTTON_0,
        ID_LOGIN_BUTTON_1,
        ID_LOGIN_EDIT_0,
        ID_LOGIN_TEXT_0,
        ID_LOGIN_BOUND,
    };
    
    enum ID_LOGIN_FRAMEWIN{
        ID_LOGIN_FRAMEWIN_MAIN = ID_LOGIN_FRAMEWIN_0,
        ID_LOGIN_FRAMEWIN_BOUND,
    };
    
    enum ID_LOGIN_BUTTON{
        ID_LOGIN_BUTTON_OK = ID_LOGIN_BUTTON_0,
        ID_LOGIN_BUTTON_CANCEL,
        ID_LOGIN_BUTTON_BOUND,
    };
    
    enum ID_LOGIN_EDIT{
        ID_LOGIN_EDIT_PASSWORD = ID_LOGIN_EDIT_0,
        ID_LOGIN_EDIT_BOUND,
    };
    
    enum ID_LOGIN_TEXT{
        ID_LOGIN_TEXT_PASSWORD = ID_LOGIN_TEXT_0,
        ID_LOGIN_TEXT_BOUND,
    };
#endif
    
#ifdef __EJE_SWRB_TEST_NUMPAD_DLG

    enum ID_NUMPAD_WIDGET{
        ID_NUMPAD_FRAMEWIN_0 = ID_LOGIN_BOUND,
        ID_NUMPAD_EDIT_0,
        ID_NUMPAD_BUTTON_0,
        ID_NUMPAD_BUTTON_1,
        ID_NUMPAD_BUTTON_2,
        ID_NUMPAD_BUTTON_3,
        ID_NUMPAD_BUTTON_4,
        ID_NUMPAD_BUTTON_5,
        ID_NUMPAD_BUTTON_6,
        ID_NUMPAD_BUTTON_7,
        ID_NUMPAD_BUTTON_8,
        ID_NUMPAD_BUTTON_9,
        ID_NUMPAD_BUTTON_10,
        ID_NUMPAD_BUTTON_11,
        ID_NUMPAD_BOUND,
    };
    
    enum ID_NUMPAD_FRAMEWIN{
        ID_NUMPAD_FRAMEWIN_MAIN = ID_NUMPAD_FRAMEWIN_0,
        ID_NUMPAD_FRAMEWIN_BOUND,
    };
    
    enum ID_NUMPAD_EDIT{
        ID_NUMPAD_EDIT_MAIN = ID_NUMPAD_EDIT_0,
        ID_NUMPAD_EDIT_BOUND,
    };
    
    enum ID_NUMPAD_BUTTON{
        ID_NUMPAD_BUTTON_NUM0= ID_NUMPAD_BUTTON_0,
        ID_NUMPAD_BUTTON_NUM1,
        ID_NUMPAD_BUTTON_NUM2,
        ID_NUMPAD_BUTTON_NUM3,
        ID_NUMPAD_BUTTON_NUM4,
        ID_NUMPAD_BUTTON_NUM5,
        ID_NUMPAD_BUTTON_NUM6,
        ID_NUMPAD_BUTTON_NUM7,
        ID_NUMPAD_BUTTON_NUM8,
        ID_NUMPAD_BUTTON_NUM9,
        ID_NUMPAD_BUTTON_OK,
        ID_NUMPAD_BUTTON_DEL,
        ID_NUMPAD_BUTTON_BOUND,
    };
#endif    

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
    
#if __EJE_SWRB_TEST_START_DLG
    extern WM_HWIN hWin_SWRB_START;
#endif

#if __EJE_SWRB_TEST_POWERSTATION_DLG
    extern WM_HWIN hWin_SWRB_POWER_STATION;
#endif

#if __EJE_SWRB_TEST_MAIN_DLG
    extern WM_HWIN hWin_SWRB_MAIN;
    extern WM_HWIN hWin_SWRB_RGB_LED;
    extern WM_HWIN hWin_SWRB_BUZZER;
#endif

#if __EJE_SWRB_TEST_SNSETTING_DLG
    extern WM_HWIN hWin_SWRB_SNSETTING;
#endif

#if __EJE_SWRB_TEST_TIMESETTING_DLG
    extern  WM_HWIN hWin_SWRB_TIMESETTING;
#endif

#if __EJE_SWRB_TEST_LOGIN_DLG
    extern WM_HWIN hWin_SWRB_LOGIN;
#endif

#if __EJE_SWRB_TEST_NUMPAD_DLG
    extern WM_HWIN hWin_SWRB_NUMPAD;
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void Button_Set_Text(WM_HWIN hWin, int buttonId, char *str);
void Button_Set_BkColor(WM_HWIN hWin, int buttonId, GUI_COLOR color);
void Button_SetEnable(WM_HWIN hWin, int buttonId);
void Button_SetDisable(WM_HWIN hWin, int buttonId);
void BUTTON_Set_Bitmap_Ex(WM_HWIN hWin, int buttonId, const GUI_BITMAP *pBitmap, int x, int y);
void BUTTON_DispStartCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispPauseCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispResumeCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispSetCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispStopCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispExitCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispConfirmCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispCheckCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispResetCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispCancelCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispSerialNumCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispTimeCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void Progbar_Set_Value(int progbarValue);
void Progbar_Set_Percent(void);
void Edit_Set_Text(WM_HWIN hWin, int editId, char *str);
void Edit_Set_Value(WM_HWIN hWin, int editId, long editValue);
void Edit_Clear(void);
void Text_Set_Text(WM_HWIN hWin, int textId, char *str);
void Text_Set_Color(WM_HWIN hWin, int textId, GUI_COLOR color);
void Checkbox_Set_State(WM_HWIN hWin, int checkboxId, unsigned int checkboxState);
void Checkbox_Set_Text(WM_HWIN hWin, int checkboxId, char *string);
int Checkbox_Get_State(int checkboxId);
void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxcolor);
void Checkbox_Set_TextAlign(int checkboxId, int align);
void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor);
void Checkbox_Set_Box_Back_Color(WM_HWIN hWin, int checkboxId, GUI_COLOR boxBkColor, int Index);
void ListWheel_AddNumString(WM_HWIN hItem, int startStringNum, int endStringNum);
void Multiedit_Set_Buffer_Size(int size);
void MultiEdit_Set_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Add_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor);
void SWRB_WM_EnableWindow(WM_HWIN hWin, int id);
void SWRB_WM_DisableWindow(WM_HWIN hWin, int id);
void SWRB_TestCheckboxStateGet(WM_HWIN hWin, int id, int taskPrio);
void SWRB_TestCheckboxStateSet(u8 stateNum);
void SWRB_TestCheckboxEnable(void);
void SWRB_TestCheckboxDisable(void);
void SWRB_RTC_TIME_Disp(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
void SweepRobot_MainTestIndicateBtnToggle(void);

#endif
/*************************** End of file ****************************/
