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

#ifndef __EJE_SWRB_TEST_POWERSTATIONDLG_H
#define __EJE_SWRB_TEST_POWERSTATIONDLG_H




#include "DIALOG.h"
#include "stm32f4xx.h"

#include "EJE_SWRB_TEST_TimeSettingDLG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_PS_WINDOW_0     (ID_TIMESET_BOUND + 0x00)
#define ID_PS_BUTTON_0     (ID_TIMESET_BOUND + 0x01)
#define ID_PS_BUTTON_1     (ID_TIMESET_BOUND + 0x02)
#define ID_PS_BUTTON_2     (ID_TIMESET_BOUND + 0x03)
#define ID_PS_BUTTON_3     (ID_TIMESET_BOUND + 0x04)
#define ID_PS_BUTTON_4     (ID_TIMESET_BOUND + 0x05)
#define ID_PS_CHECKBOX_0     (ID_TIMESET_BOUND + 0x10)
#define ID_PS_CHECKBOX_1     (ID_TIMESET_BOUND + 0x11)
#define ID_PS_CHECKBOX_2     (ID_TIMESET_BOUND + 0x12)
#define ID_PS_CHECKBOX_3     (ID_TIMESET_BOUND + 0x13)
#define ID_PS_CHECKBOX_4     (ID_TIMESET_BOUND + 0x14)
#define ID_PS_CHECKBOX_5     (ID_TIMESET_BOUND + 0x15)
#define ID_PS_CHECKBOX_6     (ID_TIMESET_BOUND + 0x16)
#define ID_PS_CHECKBOX_7     (ID_TIMESET_BOUND + 0x17)
#define ID_PS_CHECKBOX_8     (ID_TIMESET_BOUND + 0x18)
#define ID_PS_IMAGE_0     (ID_TIMESET_BOUND + 0x20)
#define ID_PS_MULTIPAGE_0     (ID_TIMESET_BOUND + 0x21)
#define ID_PS_MULTIEDIT_0   (ID_TIMESET_BOUND + 0x22)
#define ID_PS_GRAPH_0   (ID_TIMESET_BOUND + 0x23)
#define ID_PS_PROGBAR_0 (ID_TIMESET_BOUND + 0x24)
#define ID_PS_TEXT_0    (ID_TIMESET_BOUND + 0x25)
#define ID_PS_BOUND     (ID_TIMESET_BOUND + 0x40)

#define ID_PS_IMAGE_0_IMAGE_0     0x00



enum PSDLG_BUTTON{
    ID_PS_BUTTON_START = ID_PS_BUTTON_0,
    ID_PS_BUTTON_SET,
    ID_PS_BUTTON_STOP,
    ID_PS_BUTTON_EXIT,
    ID_PS_BUTTON_INDICATE,
};

enum PSDLG_CHECKBOX{
    ID_PS_CHECKBOX_LL = ID_PS_CHECKBOX_0,
    ID_PS_CHECKBOX_LS,
    ID_PS_CHECKBOX_L,
    ID_PS_CHECKBOX_M,
    ID_PS_CHECKBOX_R,
    ID_PS_CHECKBOX_RS,
    ID_PS_CHECKBOX_RL,
    ID_PS_CHECKBOX_24V,
    ID_PS_CHECKBOX_INDICATE,
};

#define ID_PS_WINDOW_MAIN       ID_PS_WINDOW_0
#define ID_PS_MULTIPAGE_MAIN      ID_PS_MULTIPAGE_0
#define ID_PS_MULTIEDIT_MAIN        ID_PS_MULTIEDIT_0
#define ID_PS_IMAGE_PS     ID_PS_IMAGE_0
#define ID_PS_GRAPH_MAIN    ID_PS_GRAPH_0
#define ID_PS_PROGBAR_MAIN  ID_PS_PROGBAR_0
#define ID_PS_TEXT_TITLE    ID_PS_TEXT_0



extern WM_HWIN hWin_SWRB_POWER_STATION;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateEJE_SWRB_TEST_PowerStationDLG
*/
WM_HWIN CreateEJE_SWRB_TEST_PowerStationDLG(void);





#endif
/*************************** End of file ****************************/
