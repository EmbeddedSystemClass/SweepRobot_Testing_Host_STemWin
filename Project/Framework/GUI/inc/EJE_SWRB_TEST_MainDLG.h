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

#include "DIALOG.h"
#include "stm32f4xx.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

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

#endif
/*************************** End of file ****************************/
