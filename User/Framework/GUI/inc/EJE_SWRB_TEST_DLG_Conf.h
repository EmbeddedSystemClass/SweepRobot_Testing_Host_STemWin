/******************** (C) COPYRIGHT 2007 EJE ***********************************************************
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
#define __EJE_SWRB_TEST_TESTSELSETTING_DLG  1
#define __EJE_SWRB_TEST_LOGIN_DLG           1
#define __EJE_SWRB_TEST_NUMPAD_DLG          1
#define __EJE_SWRB_TEST_SLAM_DLG            1
#define __EJE_SWRB_TEST_MANUL_DLG           1
#define __EJE_SWRB_TEST_STEPMOTOR_DLG       1
#define __EJE_SWRB_TEST_STEERMOTOR_DLG      1
#define __EJE_SWRB_TEST_RELAYCTRL_DLG       1
#define __EJE_SWRB_TEST_DECRYPTO_DLG        1

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

#if __EJE_SWRB_TEST_TESTSELSETTING_DLG
    #include "EJE_SWRB_TEST_TestSelSettingDLG.h"
#endif

#if __EJE_SWRB_TEST_LOGIN_DLG
    #include "EJE_SWRB_TEST_LoginDLG.h"
#endif

#if __EJE_SWRB_TEST_NUMPAD_DLG
    #include "EJE_SWRB_TEST_NumPadDLG.h"
#endif

#if __EJE_SWRB_TEST_SLAM_DLG
    #include "EJE_SWRB_TEST_SLAMDLG.h"
#endif

#if __EJE_SWRB_TEST_MANUL_DLG
    #include "EJE_SWRB_TEST_ManulDLG.h"
#endif

#if __EJE_SWRB_TEST_STEPMOTOR_DLG
    #include "EJE_SWRB_TEST_StepMotorTestDLG.h"
#endif

#if __EJE_SWRB_TEST_STEERMOTOR_DLG
    #include "EJE_SWRB_TEST_SteerMotorTestDLG.h"
#endif

#if __EJE_SWRB_TEST_RELAYCTRL_DLG
    #include "EJE_SWRB_TEST_RelayCtrlDLG.h"
#endif

#if __EJE_SWRB_TEST_DECRYPTO_DLG
    #include "EJE_SWRB_TEST_DecryptoDLG.h"
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define SWRB_MANUL_TEST_OK_BK_COLOR     GUI_LIGHTGREEN
#define SWRB_MANUL_TEST_FAULT_BK_COLOR  GUI_RED

/* EJE_SWRB_TEST_StartDLG */
#if __EJE_SWRB_TEST_START_DLG

    enum ID_START_WIDGET{
        ID_START_WINDOW_0 = GUI_ID_USER,
        ID_START_WINDOW_1,
        ID_START_IMAGE_0,
        ID_START_BUTTON_0,
        ID_START_BUTTON_1,
        ID_START_BUTTON_2,
        ID_START_BUTTON_3,
        ID_START_BUTTON_4,
        ID_START_BUTTON_5,
        ID_START_BUTTON_6,
        ID_START_BUTTON_7,
        ID_START_BUTTON_8,
        ID_START_BUTTON_9,
        ID_START_EDIT_0,
        ID_START_TEXT_0,
        ID_START_TEXT_1,
        ID_START_BOUND,
    };

    enum ID_START_WINDOW{
        ID_START_WINDOW_MAIN = ID_START_WINDOW_0,
        ID_START_WINDOW_WARNING,
        ID_START_WINDOW_BOUND,
    };
    
    enum ID_START_EDIT{
        ID_START_EDIT_DATE = ID_START_EDIT_0,
        ID_START_EDIT_BOUND,
    };

    enum ID_START_TEXT{
        ID_START_TEXT_VERSION = ID_START_TEXT_0,
        ID_START_TEXT_STORAGE_WARNING,
        ID_START_TEXT_BOUND,
    };
    
    enum ID_START_IMAGE{
        ID_START_IMAGE_LOGO = ID_START_IMAGE_0,
        ID_START_IMAGE_BOUND,
    };

    enum ID_START_BUTTON{
        ID_START_BUTTON_PCB_TEST = ID_START_BUTTON_0,
        ID_START_BUTTON_POWER_STATION,
        ID_START_BUTTON_MANUL,
        ID_START_BUTTON_SET,
        ID_START_BUTTON_SLAM,
        ID_START_BUTTON_STEP_MOTOR,
        ID_START_BUTTON_DECRYPTO,
        ID_START_BUTTON_TITLE,
        ID_START_BUTTON_WARNING_INDICATE,
        ID_START_BUTTON_WARNING_OK,
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
    
    enum ID_PCBTEST_WIDGET{
        ID_PCBTEST_CHECKBOX_0 = ID_PS_BOUND,
        ID_PCBTEST_CHECKBOX_1,
        ID_PCBTEST_CHECKBOX_2,
        ID_PCBTEST_CHECKBOX_3,
        ID_PCBTEST_CHECKBOX_4,
        ID_PCBTEST_CHECKBOX_5,
        ID_PCBTEST_CHECKBOX_6,
        ID_PCBTEST_CHECKBOX_7,
        ID_PCBTEST_CHECKBOX_8,
        ID_PCBTEST_CHECKBOX_9,
        ID_PCBTEST_CHECKBOX_10,
        ID_PCBTEST_CHECKBOX_11,
        ID_PCBTEST_CHECKBOX_12,
        ID_PCBTEST_EDIT_0,
        ID_PCBTEST_EDIT_1,
        ID_PCBTEST_EDIT_2,
        ID_PCBTEST_EDIT_3,
        ID_PCBTEST_EDIT_4,
        ID_PCBTEST_EDIT_5,
        ID_PCBTEST_EDIT_6,
        ID_PCBTEST_EDIT_7,
        ID_PCBTEST_EDIT_8,
        ID_PCBTEST_EDIT_9,
        ID_PCBTEST_EDIT_10,
        ID_PCBTEST_EDIT_11,
        ID_PCBTEST_EDIT_12,
        ID_PCBTEST_EDIT_13,
        ID_PCBTEST_EDIT_14,
        ID_PCBTEST_EDIT_15,
        ID_PCBTEST_EDIT_16,
        ID_PCBTEST_EDIT_17,
        ID_PCBTEST_EDIT_18,
        ID_PCBTEST_BUTTON_0,
        ID_PCBTEST_BUTTON_1,
        ID_PCBTEST_BUTTON_2,
        ID_PCBTEST_BUTTON_3,
        ID_PCBTEST_BUTTON_4,
        ID_PCBTEST_BUTTON_5,
        ID_PCBTEST_BUTTON_6,
        ID_PCBTEST_BUTTON_7,
        ID_PCBTEST_BUTTON_8,
        ID_PCBTEST_BUTTON_9,
        ID_PCBTEST_BUTTON_10,
        ID_PCBTEST_BUTTON_11,
        ID_PCBTEST_BUTTON_12,
        ID_PCBTEST_BUTTON_13,
        ID_PCBTEST_BUTTON_14,
        ID_PCBTEST_BUTTON_15,
        ID_PCBTEST_BUTTON_16,
        ID_PCBTEST_FRAMEWIN_0,
        ID_PCBTEST_FRAMEWIN_1,
        ID_PCBTEST_FRAMEWIN_2,
        ID_PCBTEST_PROGBAR_0,
        ID_PCBTEST_PROGBAR_1,
        ID_PCBTEST_MULTIEDIT_0,
        ID_PCBTEST_WINDOW_0,
        ID_PCBTEST_WINDOW_1,
        ID_PCBTEST_TEXT_0,
        ID_PCBTEST_TEXT_1,
        ID_PCBTEST_TEXT_2,
        ID_PCBTEST_BOUND,
    };
    
    enum ID_PCBTEST_BUTTON{
        ID_PCBTEST_BUTTON_START = ID_PCBTEST_BUTTON_0,
        ID_PCBTEST_BUTTON_SET,
        ID_PCBTEST_BUTTON_STOP,
        ID_PCBTEST_BUTTON_EXIT,
        ID_PCBTEST_BUTTON_SET_SN,
        ID_PCBTEST_BUTTON_SET_TIME,
        ID_PCBTEST_BUTTON_WARNING_RETEST,
        ID_PCBTEST_BUTTON_WARNING_SKIP,
        ID_PCBTEST_BUTTON_KEY_TITLE,
        ID_PCBTEST_BUTTON_RGB_LED_OK,
        ID_PCBTEST_BUTTON_RGB_LED_ERR,
        ID_PCBTEST_BUTTON_RGB_LED_TEXT,
        ID_PCBTEST_BUTTON_BUZZER_OK,
        ID_PCBTEST_BUTTON_BUZZER_ERR,
        ID_PCBTEST_BUTTON_LOGIN_OK,
        ID_PCBTEST_BUTTON_LOGIN_CANCEL,
        ID_PCBTEST_BUTTON_INDICATE,
        ID_PCBTEST_BUTTON_BOUND,
    };
    
    enum ID_PCBTEST_FRAMEWIN{
        ID_PCBTEST_FRAMEWIN_KEY = ID_PCBTEST_FRAMEWIN_0,
        ID_PCBTEST_FRAMEWIN_RGB_LED,
        ID_PCBTEST_FRAMEWIN_BUZZER,
        ID_PCBTEST_FRAMEWIN_BOUND,
    };
    
    enum ID_PCBTEST_MULTIEDIT{
        ID_PCBTEST_MULTIEDIT_MAIN = ID_PCBTEST_MULTIEDIT_0,
        ID_PCBTEST_MULTIEDIT_BOUND,
    };
    
    enum ID_PCBTEST_PROGBAR{
        ID_PCBTEST_PROGBAR_MAIN = ID_PCBTEST_PROGBAR_0,
        ID_PCBTEST_PROGBAR_KEY,
        ID_PCBTEST_PROGBAR_BOUND,
    };

    enum ID_PCBTEST_WINDOW{
        ID_PCBTEST_WINDOW_MAIN = ID_PCBTEST_WINDOW_0,
        ID_PCBTEST_WINDOW_WARNING,
        ID_PCBTEST_WINDOW_BOUND,
    };
    
    enum ID_PCBTEST_TEXT{
        ID_PCBTEST_TEXT_KEY = ID_PCBTEST_TEXT_0,
        ID_PCBTEST_TEXT_RGB_LED,
        ID_PCBTEST_TEXT_BUZZER,
        ID_PCBTEST_TEXT_BOUND,
    };

    enum ID_PCBTEST_CHECKBOX{
      ID_PCBTEST_CHECKBOX_WHEEL = ID_PCBTEST_CHECKBOX_0,
      ID_PCBTEST_CHECKBOX_BRUSH,
      ID_PCBTEST_CHECKBOX_FAN,
      ID_PCBTEST_CHECKBOX_IFRD,
      ID_PCBTEST_CHECKBOX_COLLISION,
      ID_PCBTEST_CHECKBOX_WHEEL_FLOAT,
      ID_PCBTEST_CHECKBOX_ASH_TRAY,
      ID_PCBTEST_CHECKBOX_UNIWHEEL,
      ID_PCBTEST_CHECKBOX_IRDA,
      ID_PCBTEST_CHECKBOX_CHARGE,
      ID_PCBTEST_CHECKBOX_BUZZER,
      ID_PCBTEST_CHECKBOX_RGB_LED,
      ID_PCBTEST_CHECKBOX_KEY,
      ID_PCBTEST_CHECKBOX_BOUND,
    };

    enum ID_PCBTEST_EDIT{
        ID_PCBTEST_EDIT_U1 = ID_PCBTEST_EDIT_0,
        ID_PCBTEST_EDIT_U2,
        ID_PCBTEST_EDIT_U3,
        ID_PCBTEST_EDIT_U4,
        ID_PCBTEST_EDIT_U5,
        ID_PCBTEST_EDIT_U6,
        ID_PCBTEST_EDIT_U7,
        ID_PCBTEST_EDIT_U8,
        ID_PCBTEST_EDIT_D1,
        ID_PCBTEST_EDIT_D2,
        ID_PCBTEST_EDIT_D3,
        ID_PCBTEST_EDIT_D4,
        ID_PCBTEST_EDIT_D5,
        ID_PCBTEST_EDIT_D6,
        ID_PCBTEST_EDIT_D7,
        ID_PCBTEST_EDIT_D8,
        ID_PCBTEST_EDIT_SN,
        ID_PCBTEST_EDIT_DATE,
        ID_PCBTEST_EDIT_LOGIN,
        ID_PCBTEST_EDIT_BOUND,
    };
#endif

/* EJE_SWRB_TEST_SNSettingDLG */    
#ifdef __EJE_SWRB_TEST_SNSETTING_DLG
    
    enum ID_SNSET_WIDGET{
        ID_SNSET_LISTWHEEL_0 = ID_PCBTEST_BOUND,
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
        ID_SNSET_BUTTON_9,
        ID_SNSET_BUTTON_10,
        ID_SNSET_BUTTON_11,
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
        ID_SNSET_BUTTON_TITLE  = ID_SNSET_BUTTON_0,
        ID_SNSET_BUTTON_CONFIRM,
        ID_SNSET_BUTTON_CHECK,
        ID_SNSET_BUTTON_RESET,
        ID_SNSET_BUTTON_CANCEL,
        ID_SNSET_BUTTON_SNSET,
        ID_SNSET_BUTTON_TIMESET,
        ID_SNSET_BUTTON_TESTSELECT,
        ID_SNSET_BUTTON_RESERVE2,
        ID_SNSET_BUTTON_RESERVE3,
        ID_SNSET_BUTTON_RESERVE4,
        ID_SNSET_BUTTON_RESERVE5,
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
        ID_TIMESET_BUTTON_9,
        ID_TIMESET_BUTTON_10,
        ID_TIMESET_BUTTON_11,
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
        ID_TIMESET_BUTTON_TITLE = ID_TIMESET_BUTTON_0,
        ID_TIMESET_BUTTON_CONFIRM,
        ID_TIMESET_BUTTON_CHECK,
        ID_TIMESET_BUTTON_RESET,
        ID_TIMESET_BUTTON_CANCEL,
        ID_TIMESET_BUTTON_SNSET,
        ID_TIMESET_BUTTON_TIMESET,
        ID_TIMESET_BUTTON_TESTSELECT,
        ID_TIMESET_BUTTON_RESERVE2,
        ID_TIMESET_BUTTON_RESERVE3,
        ID_TIMESET_BUTTON_RESERVE4,
        ID_TIMESET_BUTTON_RESERVE5,
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
        ID_LOGIN_BUTTON_2,
        ID_LOGIN_BUTTON_3,
        ID_LOGIN_BUTTON_4,
        ID_LOGIN_BUTTON_5,
        ID_LOGIN_BUTTON_6,
        ID_LOGIN_BUTTON_7,
        ID_LOGIN_BUTTON_8,
        ID_LOGIN_BUTTON_9,
        ID_LOGIN_BUTTON_10,
        ID_LOGIN_BUTTON_11,
        ID_LOGIN_BUTTON_12,
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
        ID_LOGIN_BUTTON_DELETE,
        ID_LOGIN_BUTTON_NUM0,
        ID_LOGIN_BUTTON_NUM1,
        ID_LOGIN_BUTTON_NUM2,
        ID_LOGIN_BUTTON_NUM3,
        ID_LOGIN_BUTTON_NUM4,
        ID_LOGIN_BUTTON_NUM5,
        ID_LOGIN_BUTTON_NUM6,
        ID_LOGIN_BUTTON_NUM7,
        ID_LOGIN_BUTTON_NUM8,
        ID_LOGIN_BUTTON_NUM9,
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
        ID_NUMPAD_BUTTON_NUM0 = ID_NUMPAD_BUTTON_0,
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
    
#if __EJE_SWRB_TEST_SLAM_DLG
    
    enum ID_SLAM_WIDGET{
        ID_SLAM_WINDOW_0 = ID_NUMPAD_BOUND,
        ID_SLAM_BUTTON_0,
        ID_SLAM_BUTTON_1,
        ID_SLAM_BUTTON_2,
        ID_SLAM_BUTTON_3,
        ID_SLAM_PROGBAR_0,
        ID_SLAM_EDIT_0,
        ID_SLAM_EDIT_1,
        ID_SLAM_EDIT_2,
        ID_SLAM_TEXT_0,
        ID_SLAM_TEXT_1,
        ID_SLAM_TEXT_2,
        ID_SLAM_TEXT_3,
        ID_SLAM_BOUND,
    };
    
    enum ID_SLAM_WINDOW{
        ID_SLAM_WINDOW_MAIN = ID_SLAM_WINDOW_0,
        ID_SLAM_WINDOW_BOUND,
    };
    
    enum ID_SLAM_BUTTON{
        ID_SLAM_BUTTON_START = ID_SLAM_BUTTON_0,
        ID_SLAM_BUTTON_RESET,
        ID_SLAM_BUTTON_STOP,
        ID_SLAM_BUTTON_EXIT,
        ID_SLAM_BUTTON_BOUND,
    };
    
    enum ID_SLAM_PROGBAR{
        ID_SLAM_PROGBAR_MAIN = ID_SLAM_PROGBAR_0,
        ID_SLAM_PROGBAR_BOUND,
    };
    
    enum ID_SLAM_EDIT{
        ID_SLAM_EDIT_XPOS = ID_SLAM_EDIT_0,
        ID_SLAM_EDIT_YPOS,
        ID_SLAM_EDIT_RVALUE,
        ID_SLAM_EDIT_BOUND,
    };
    
    enum ID_SLAM_TEXT{
        ID_SLAM_TEXT_TITLE = ID_SLAM_TEXT_0,
        ID_SLAM_TEXT_XPOS, 
        ID_SLAM_TEXT_YPOS,
        ID_SLAM_TEXT_RVALUE,
        ID_SLAM_TEXT_BOUND,
    };
#endif
    
#if __EJE_SWRB_TEST_TESTSELSETTING_DLG
    
    enum ID_TESTSEL_WIDGET{
        ID_TESTSEL_WINDOW_0 = ID_SLAM_BOUND,
        ID_TESTSEL_BUTTON_0,
        ID_TESTSEL_BUTTON_1,
        ID_TESTSEL_BUTTON_2,
        ID_TESTSEL_BUTTON_3,
        ID_TESTSEL_BUTTON_4,
        ID_TESTSEL_BUTTON_5,
        ID_TESTSEL_BUTTON_6,
        ID_TESTSEL_BUTTON_7,
        ID_TESTSEL_BUTTON_8,
        ID_TESTSEL_BUTTON_9,
        ID_TESTSEL_BUTTON_10,
        ID_TESTSEL_BUTTON_11,
        ID_TESTSEL_BUTTON_12,
        ID_TESTSEL_BUTTON_13,
        ID_TESTSEL_BUTTON_14,
        ID_TESTSEL_BUTTON_15,
        ID_TESTSEL_BUTTON_16,
        ID_TESTSEL_BUTTON_17,
        ID_TESTSEL_BUTTON_18,
        ID_TESTSEL_BUTTON_19,
        ID_TESTSEL_BUTTON_20,
        ID_TESTSEL_BUTTON_21,
        ID_TESTSEL_BUTTON_22,
        ID_TESTSEL_BUTTON_23,
        ID_TESTSEL_BUTTON_24,
        ID_TESTSEL_BUTTON_25,
        ID_TESTSEL_BUTTON_26,
        ID_TESTSEL_BUTTON_27,
        ID_TESTSEL_BUTTON_28,
        ID_TESTSEL_BUTTON_29,
        ID_TESTSEL_BUTTON_30,
        ID_TESTSEL_BOUND,
    };
    
    enum ID_TESTSEL_WINDOW{
        ID_TESTSEL_WINDOW_MAIN = ID_TESTSEL_WINDOW_0,
        ID_TESTSEL_WINDOW_BOUND,
    };
    
    enum ID_TESTSEL_BUTTON{
        ID_TESTSEL_BUTTON_CONFIRM = ID_TESTSEL_BUTTON_0,
        ID_TESTSEL_BUTTON_SELALL,
        ID_TESTSEL_BUTTON_SELNONE,
        ID_TESTSEL_BUTTON_CANCEL,
        ID_TESTSEL_BUTTON_SNSET,
        ID_TESTSEL_BUTTON_TIMESET,
        ID_TESTSEL_BUTTON_TESTSEL,
        ID_TESTSEL_BUTTON_RESERVE1,
        ID_TESTSEL_BUTTON_RESERVE2,
        ID_TESTSEL_BUTTON_RESERVE3,
        ID_TESTSEL_BUTTON_RESERVE4,
        ID_TESTSEL_BUTTON_WHEEL,
        ID_TESTSEL_BUTTON_BRUSH,
        ID_TESTSEL_BUTTON_FAN,
        ID_TESTSEL_BUTTON_IFRD,
        ID_TESTSEL_BUTTON_COLLISION,
        ID_TESTSEL_BUTTON_WHEEL_FLOAT,
        ID_TESTSEL_BUTTON_ASH_TRAY,
        ID_TESTSEL_BUTTON_UNIWHEEL,
        ID_TESTSEL_BUTTON_IRDA,
        ID_TESTSEL_BUTTON_CHARGE,
        ID_TESTSEL_BUTTON_BUZZER,
        ID_TESTSEL_BUTTON_RGB_LED,
        ID_TESTSEL_BUTTON_KEY,
        ID_TESTSEL_BUTTON_TEST1,
        ID_TESTSEL_BUTTON_TEST2,
        ID_TESTSEL_BUTTON_FRONT_IFRD,
        ID_TESTSEL_BUTTON_TEST4,
        ID_TESTSEL_BUTTON_TEST5,
        ID_TESTSEL_BUTTON_TEST6,
        ID_TESTSEL_BUTTON_TEST7,
        ID_TESTSEL_BUTTON_BOUND,
    };
#endif

#if __EJE_SWRB_TEST_MANUL_DLG
    
    enum ID_MANUL_WIDGET{
        ID_MANUL_WINDOW_0 = ID_TESTSEL_BOUND,
        ID_MANUL_WINDOW_1,
        ID_MANUL_BUTTON_0,
        ID_MANUL_BUTTON_1,
        ID_MANUL_BUTTON_2,
        ID_MANUL_BUTTON_3,
        ID_MANUL_BUTTON_4,
        ID_MANUL_BUTTON_5,
        ID_MANUL_BUTTON_6,
        ID_MANUL_BUTTON_7,
        ID_MANUL_BUTTON_8,
        ID_MANUL_BUTTON_9,
        ID_MANUL_BUTTON_10,
        ID_MANUL_BUTTON_11,
        ID_MANUL_BUTTON_12,
        ID_MANUL_LISTVIEW_0,
        ID_MANUL_TEXT_0,
        ID_MANUL_TEXT_1,
        ID_MANUL_EDIT_0,
        ID_MANUL_EDIT_1,
        ID_MANUL_PROGBAR_0,
        ID_MANUL_BOUND,
    };
    
    enum ID_MANUL_WINDOW{
        ID_MANUL_WINDOW_MAIN = ID_MANUL_WINDOW_0,
        ID_MANUL_WINDOW_TEST_STATE_INDICATE,
        ID_MANUL_WINDOW_BOUND,
    };
    
    enum ID_MANUL_BUTTON{
        ID_MANUL_BUTTON_START = ID_MANUL_BUTTON_0,
        ID_MANUL_BUTTON_SET,
        ID_MANUL_BUTTON_RESET,
        ID_MANUL_BUTTON_EXIT,
        ID_MANUL_BUTTON_INDICATE,
        ID_MANUL_BUTTON_WHEEL,
        ID_MANUL_BUTTON_BRUSH,
        ID_MANUL_BUTTON_FAN,
        ID_MANUL_BUTTON_IFRD,
        ID_MANUL_BUTTON_BUZZER,
        ID_MANUL_BUTTON_RGB_LED,
        ID_MANUL_BUTTON_RELAY,
        ID_MANUL_BUTTON_TEST_STATE_INDICATE,
        ID_MANUL_BUTTON_BOUND,
    };
    
    enum ID_MANUL_LISTVIEW{
        ID_MANUL_LISTVIEW_MAIN = ID_MANUL_LISTVIEW_0,
        ID_MANUL_LISTVIEW_BOUND,
    };
    
    enum ID_MANUL_TEXT{
        ID_MANUL_TEXT_TITLE = ID_MANUL_TEXT_0,
        ID_MANUL_TEXT_VOLT,
        ID_MANUL_TEXT_BOUND,
    };
    
    enum ID_MANUL_EDIT{
        ID_MANUL_EDIT_DATE = ID_MANUL_EDIT_0,
        ID_MANUL_EDIT_VOLT,
        ID_MANUL_EDIT_BOUND,
    };
    
    enum ID_MANUL_PROGBAR{
        ID_MANUL_PROGBAR_VOLT = ID_MANUL_PROGBAR_0,
        ID_MANUL_PROGBAR_BOUND,
    };
#endif

#if __EJE_SWRB_TEST_STEPMOTOR_DLG
    
    enum ID_STEPMOTOR_WIDGET{
        ID_STEPMOTOR_WINDOW_0 = ID_MANUL_BOUND,
        ID_STEPMOTOR_BUTTON_0,
        ID_STEPMOTOR_BUTTON_1,
        ID_STEPMOTOR_BUTTON_2,
        ID_STEPMOTOR_BUTTON_3,
        ID_STEPMOTOR_BUTTON_4,
        ID_STEPMOTOR_BUTTON_5,
        ID_STEPMOTOR_BUTTON_6,
        ID_STEPMOTOR_SLIDER_0,
        ID_STEPMOTOR_SLIDER_1,
        ID_STEPMOTOR_EDIT_0,
        ID_STEPMOTOR_EDIT_1,
        ID_STEPMOTOR_EDIT_2,
        ID_STEPMOTOR_TEXT_0,
        ID_STEPMOTOR_TEXT_1,
        ID_STEPMOTOR_TEXT_2,
        ID_STEPMOTOR_BOUND,
    };
    
    enum ID_STEPMOTOR_WINDOW{
        ID_STEPMOTOR_WINDOW_MAIN = ID_STEPMOTOR_WINDOW_0,
        ID_STEPMOTOR_WINDOW_BOUND,
    };
    
    enum ID_STEPMOTOR_BUTTON{
        ID_STEPMOTOR_BUTTON_RUN = ID_STEPMOTOR_BUTTON_0,
        ID_STEPMOTOR_BUTTON_STOP,
        ID_STEPMOTOR_BUTTON_FORWARD,
        ID_STEPMOTOR_BUTTON_BACKWARD,
        ID_STEPMOTOR_BUTTON_ENABLE,
        ID_STEPMOTOR_BUTTON_DISABLE,
        ID_STEPMOTOR_BUTTON_EXIT,
        ID_STEPMOTOR_BUTTON_BOUND,
    };
    
    enum ID_STEPMOTOR_TEXT{
        ID_STEPMOTOR_TEXT_SPEED = ID_STEPMOTOR_TEXT_0,
        ID_STEPMOTOR_TEXT_STEPS,
        ID_STEPMOTOR_TEXT_POS,
        ID_STEPMOTOR_TEXT_BOUND,
    };
    
    enum ID_STEPMOTOR_SLIDER{
        ID_STEPMOTOR_SLIDER_SPEED = ID_STEPMOTOR_SLIDER_0,
        ID_STEPMOTOR_SLIDER_STEPS,
        ID_STEPMOTOR_SLIDER_BOUND,
    };
    
    enum ID_STEPMOTOR_EDIT{
        ID_STEPMOTOR_EDIT_SPEED = ID_STEPMOTOR_EDIT_0,
        ID_STEPMOTOR_EDIT_STEPS,
        ID_STEPMOTOR_EDIT_POS,
        ID_STEPMOTOR_EDIT_BOUND,
    };
#endif
    
#if __EJE_SWRB_TEST_STEERMOTOR_DLG
    
    enum ID_STEERMOTOR_WIDGET{
        ID_STEERMOTOR_WINDOW_0 = ID_STEPMOTOR_BOUND,
        ID_STEERMOTOR_BUTTON_0,
        ID_STEERMOTOR_BUTTON_1,
        ID_STEERMOTOR_BUTTON_2,
        ID_STEERMOTOR_BUTTON_3,
        ID_STEERMOTOR_BUTTON_4,
        ID_STEERMOTOR_BUTTON_5,
        ID_STEERMOTOR_BUTTON_6,
        ID_STEERMOTOR_BUTTON_7,
        ID_STEERMOTOR_BUTTON_8,
        ID_STEERMOTOR_BUTTON_9,
        ID_STEERMOTOR_SLIDER_0,
        ID_STEERMOTOR_SLIDER_1,
        ID_STEERMOTOR_SLIDER_2,
        ID_STEERMOTOR_EDIT_0,
        ID_STEERMOTOR_EDIT_1,
        ID_STEERMOTOR_EDIT_2,
        ID_STEERMOTOR_TEXT_0,
        ID_STEERMOTOR_TEXT_1,
        ID_STEERMOTOR_TEXT_2,
        ID_STEERMOTOR_BOUND,
    };
    
    enum ID_STEERMOTOR_WINDOW{
        ID_STEERMOTOR_WINDOW_MAIN = ID_STEERMOTOR_WINDOW_0,
        ID_STEERMOTOR_WINDOW_BOUND,
    };
    
    enum ID_STEERMOTOR_BUTTON{
        ID_STEERMOTOR_BUTTON_RETURN = ID_STEERMOTOR_BUTTON_0,
        ID_STEERMOTOR_BUTTON_LIDLE,
        ID_STEERMOTOR_BUTTON_LFRONT,
        ID_STEERMOTOR_BUTTON_LSIDE,
        ID_STEERMOTOR_BUTTON_MIDLE,
        ID_STEERMOTOR_BUTTON_MUP,
        ID_STEERMOTOR_BUTTON_MDOWN,
        ID_STEERMOTOR_BUTTON_RIDLE,
        ID_STEERMOTOR_BUTTON_RFRONT,
        ID_STEERMOTOR_BUTTON_RSIDE,
        ID_STEERMOTOR_BUTTON_BOUND,
    };
    
    enum ID_STEERMOTOR_SLIDER{
        ID_STEERMOTOR_SLIDER_LCOLLISION = ID_STEERMOTOR_SLIDER_0,
        ID_STEERMOTOR_SLIDER_MWHEELFLOAT,
        ID_STEERMOTOR_SLIDER_RCOLLISION,
        ID_STEERMOTOR_SLIDER_BOUND,
    };
    
    enum ID_STEERMOTOR_EDIT{
        ID_STEERMOTOR_EDIT_LCOLLISION = ID_STEERMOTOR_EDIT_0,
        ID_STEERMOTOR_EDIT_MWHEELFLOAT,
        ID_STEERMOTOR_EDIT_RCOLLISION,
        ID_STEERMOTOR_EDIT_BOUND,
    };
    
    enum ID_STEERMOTOR_TEXT{
        ID_STEERMOTOR_TEXT_LCOLLISION = ID_STEERMOTOR_TEXT_0,
        ID_STEERMOTOR_TEXT_MWHEELFLOAT,
        ID_STEERMOTOR_TEXT_RCOLLISION,
        ID_STEERMOTOR_TEXT_BOUND,
    };
    
#endif
    
#if __EJE_SWRB_TEST_RELAYCTRL_DLG
    
    enum ID_RELAYCTRL_WIDGET{
        ID_RELAYCTRL_WINDOW_0 = ID_STEERMOTOR_BOUND,
        ID_RELAYCTRL_BUTTON_0,
        ID_RELAYCTRL_BUTTON_1,
        ID_RELAYCTRL_BUTTON_2,
        ID_RELAYCTRL_BUTTON_3,
        ID_RELAYCTRL_BUTTON_4,
        ID_RELAYCTRL_BUTTON_5,
        ID_RELAYCTRL_BUTTON_6,
        ID_RELAYCTRL_BUTTON_7,
        ID_RELAYCTRL_BUTTON_8,
        ID_RELAYCTRL_BUTTON_9,
        ID_RELAYCTRL_BUTTON_10,
        ID_RELAYCTRL_BUTTON_11,
        ID_RELAYCTRL_BOUND,
    };
    
    enum ID_RELAYCTRL_WINDOW{
        ID_RELAYCTRL_WINDOW_MAIN = ID_RELAYCTRL_WINDOW_0,
        ID_RELAYCTRL_WINDOW_BOUND,
    };
    
    enum ID_RELAYCTRL_BUTTON{
        ID_RELAYCTRL_BUTTON_R1 = ID_RELAYCTRL_BUTTON_0,
        ID_RELAYCTRL_BUTTON_R2,
        ID_RELAYCTRL_BUTTON_R3,
        ID_RELAYCTRL_BUTTON_R4,
        ID_RELAYCTRL_BUTTON_R5,
        ID_RELAYCTRL_BUTTON_R6,
        ID_RELAYCTRL_BUTTON_R7,
        ID_RELAYCTRL_BUTTON_R8,
        ID_RELAYCTRL_BUTTON_START,
        ID_RELAYCTRL_BUTTON_SET,
        ID_RELAYCTRL_BUTTON_STOP,
        ID_RELAYCTRL_BUTTON_EXIT,
        ID_RELAYCTRL_BUTTON_BOUND,
    };
    
#endif
    
#if __EJE_SWRB_TEST_DECRYPTO_DLG
    
    enum ID_DECRYPTO_WIDGET{
        ID_DECRYPTO_WINDOW_0 = ID_RELAYCTRL_BOUND,
        ID_DECRYPTO_TREEVIEW_0,
        ID_DECRYPTO_MULTIEDIT_0,
        ID_DECRYPTO_BUTTON_0,
        ID_DECRYPTO_BUTTON_1,
        ID_DECRYPTO_BUTTON_2,
        ID_DECRYPTO_BUTTON_3,
        ID_DECRYPTO_BOUND,
    };
    
    enum ID_DECRYPTO_WINDOW{
        ID_DECRYPTO_WINDOW_MAIN = ID_DECRYPTO_WINDOW_0,
        ID_DECRYPTO_WINDOW_BOUND,
    };
    
    enum ID_DECRYPTO_TREEVIEW{
        ID_DECRYPTO_TREEVIEW_MAIN = ID_DECRYPTO_TREEVIEW_0,
        ID_DECRYPTO_TREEVIEW_BOUND,
    };
    
    enum ID_DECRYPTO_MULTIEDIT{
        ID_DECRYPTO_MULTIEDIT_MAIN = ID_DECRYPTO_MULTIEDIT_0,
        ID_DECRYPTO_MULTIEDIT_BOUND,
    };
    
    enum ID_DECRYPTO_BUTTON{
        ID_DECRYPTO_BUTTON_OPEN = ID_DECRYPTO_BUTTON_0,
        ID_DECRYPTO_BUTTON_DECRYPT,
        ID_DECRYPTO_BUTTON_SAVE,
        ID_DECRYPTO_BUTTON_EXIT,
        ID_DECRYPTO_BUTTON_BOUND,
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
    extern WM_HWIN hWin_SWRB_PCBTEST;
    extern WM_HWIN hWin_SWRB_WARNING;
    extern WM_HWIN hWin_SWRB_KEY;
    extern WM_HWIN hWin_SWRB_RGB_LED;
    extern WM_HWIN hWin_SWRB_BUZZER;
#endif

#if __EJE_SWRB_TEST_SNSETTING_DLG
    extern WM_HWIN hWin_SWRB_SNSET;
#endif

#if __EJE_SWRB_TEST_TIMESETTING_DLG
    extern  WM_HWIN hWin_SWRB_TIMESET;
#endif

#if __EJE_SWRB_TEST_LOGIN_DLG
    extern WM_HWIN hWin_SWRB_LOGIN;
#endif

#if (__EJE_SWRB_TEST_NUMPAD_DLG && defined USE_NUMPAD)
    extern WM_HWIN hWin_SWRB_NUMPAD;
#endif

#if (__EJE_SWRB_TEST_SLAM_DLG && defined USE_SLAM)
    extern WM_HWIN hWin_SWRB_SLAM;
#endif

#if __EJE_SWRB_TEST_TESTSELSETTING_DLG
    extern WM_HWIN hWin_SWRB_TESTSEL;
#endif

#if __EJE_SWRB_TEST_MANUL_DLG
    extern WM_HWIN hWin_SWRB_MANUL;
#endif

#if (__EJE_SWRB_TEST_STEPMOTOR_DLG && defined USE_STEPMOTOR_CTRL)
    extern WM_HWIN hWin_SWRB_STEPMOTOR;
#endif

#if (__EJE_SWRB_TEST_STEERMOTOR_DLG && defined USE_STEERMOTOR_CTRL)
    extern WM_HWIN hWin_SWRB_STEERMOTOR;
#endif

#if (__EJE_SWRB_TEST_RELAYCTRL_DLG && defined USE_RELAY_CTRL)
    extern WM_HWIN hWin_SWRB_RELAY;
#endif

#if (__EJE_SWRB_TEST_DECRYPTO_DLG && defined USE_DECRYPTO)
    extern WM_HWIN hWin_SWRB_DECRYPTO;
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void Button_Set_Text(WM_HWIN hWin, int buttonId, char *str);
void Button_Set_TextColor(WM_HWIN hWin, int buttonId, GUI_COLOR color);
void Button_Set_BkColor(WM_HWIN hWin, int buttonId, GUI_COLOR color);
void Button_SetEnable(WM_HWIN hWin, int buttonId);
void Button_SetDisable(WM_HWIN hWin, int buttonId);
void BUTTON_Set_Bitmap_Ex(WM_HWIN hWin, int buttonId, const GUI_BITMAP *pBitmap, int x, int y);
void BUTTON_DispSWRBTestTitleCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispRobotTestTitleCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
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
void BUTTON_DispTestSelCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispOKCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispErrorCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispReTestCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispSkipCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispDeleteCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispAutoModeCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispManulModeCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void BUTTON_DispPressKeyCHNStr(WM_HWIN hWin, int buttonId, int x, int y);
void Progbar_Set_Value(WM_HWIN hWin, int id, int progbarValue);
void Progbar_Set_Percent(void);
void Edit_Set_Text(WM_HWIN hWin, int editId, char *str);
void Edit_Set_Value(WM_HWIN hWin, int editId, long editValue);
void Edit_Set_FloatValue(WM_HWIN hWin, int editId, float editValue);
void Edit_Get_Text(WM_HWIN hWin, int editId, char *str);
int Edit_Get_UserData(WM_HWIN hWin, int editId, char *str, int numBytes);
void Edit_Set_HexMode(WM_HWIN hWin, int editId, long value, long min, long max);
void Edit_Set_DecMode(WM_HWIN hWin, int editId, long value, long min, long max, int shift, unsigned char flags);
void Edit_Clear(void);
void Text_Set_Text(WM_HWIN hWin, int textId, char *str);
void Text_Set_Color(WM_HWIN hWin, int textId, GUI_COLOR color);
void Checkbox_Set_State(WM_HWIN hWin, int checkboxId, unsigned int checkboxState);
void Checkbox_Set_Text(WM_HWIN hWin, int checkboxId, char *string);
int Checkbox_Get_State(WM_HWIN hWin, int checkboxId);
void Checkbox_Set_Text_Color(int checkboxId, GUI_COLOR checkboxcolor);
void Checkbox_Set_TextAlign(int checkboxId, int align);
void Checkbox_Set_Back_Color(int checkboxId, GUI_COLOR checkboxbkcolor);
void Checkbox_Set_Box_Back_Color(WM_HWIN hWin, int checkboxId, GUI_COLOR boxBkColor, int Index);
void Listview_Set_Item_Text(WM_HWIN hWin, int id, uint16_t column, uint16_t row, char *str);
void Listview_Set_Item_TextColor(WM_HWIN hWin, int id, uint16_t colunm, uint16_t row, GUI_COLOR color);
void Listview_Set_Text_Color(WM_HWIN hWin, int id, GUI_COLOR color);
void Listview_Set_Item_BkColor(WM_HWIN hWin, int id, int column, int row, GUI_COLOR color);
void Listview_Set_Item_BkColor_withCoord(WM_HWIN hWin, int id, u8 aCoord[][2],enum SWRB_MANUL_TEST_DATA_POS pos, GUI_COLOR color);
void ListWheel_AddNumString(WM_HWIN hItem, int startStringNum, int endStringNum);
void Multiedit_Set_Buffer_Size(int size);
void MultiEdit_Set_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Add_Text(WM_HWIN hWin, int multiEditId, char *s);
void MultiEdit_Set_Text_Color(GUI_COLOR multieditTextColor);
void WM_Set_Y_Size(WM_HWIN hWin, int id, int ySize);
void SWRB_TestDataFilePathDisp(WM_HWIN hWin, int id);
void SWRB_TestCurSNDisp(WM_HWIN hWin, int idEdit);
void SWRB_WM_EnableWindow(WM_HWIN hWin, int id);
void SWRB_WM_DisableWindow(WM_HWIN hWin, int id);
void SWRB_TestCheckboxStateGet(WM_HWIN hWin, int id);
void SWRB_TestCheckboxStateSet(u8 stateNum);
void SWRB_PCBTestCheckboxEnable(void);
void SWRB_PCBTestCheckboxDisable(void);
void SWRB_RTC_TIME_Disp(WM_HWIN hWin, int id, RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
void SWRB_IndicateButtonToggle(WM_HWIN hWin, int buttonId);
void SWRB_LastCallNumpadEditSave(WM_MESSAGE *pMsg);
WM_HWIN SWRB_LastCallNumpadEditWinGet(void);
int SWRB_LastCallNumpadEditIdGet(void);
void SWRB_SET_EditTextUpdate(void);
void SWRB_SET_ListwheelSnapPosUpdate(void);

#endif
/*************************** End of file ****************************/
