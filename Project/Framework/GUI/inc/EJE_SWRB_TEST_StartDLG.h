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

#ifndef __EJE_SWRB_TEST_STARTDLG_H
#define __EJE_SWRB_TEST_STARTDLG_H

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

#define ID_START_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_START_IMAGE_0 (GUI_ID_USER + 0x01)
#define ID_START_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_START_BUTTON_1 (GUI_ID_USER + 0x03)
#define ID_START_TEXT_0 (GUI_ID_USER + 0x04)
#define ID_START_BOUND (GUI_ID_USER + 0x10)

#define ID_START_IMAGE_0_IMAGE_0 0x00


// USER START (Optionally insert additional defines)
enum StartDLG_BUTTON{
    ID_START_BUTTON_PCB_TEST = ID_START_BUTTON_0,
    ID_START_BUTTON_POWER_STATION,
};

#define ID_START_WINDOW_START   ID_START_WINDOW_0
#define ID_START_TEXT_VERSION   ID_START_TEXT_0
#define ID_START_IMAGE_LOGO     ID_START_IMAGE_0

// USER END

extern WM_HWIN hWin_SWRB_START;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateEJE_SWRB_TEST_StartDLG
*/
WM_HWIN CreateEJE_SWRB_TEST_StartDLG(void);

// USER START (Optionally insert additional public code)
// USER END


#endif
/*************************** End of file ****************************/
