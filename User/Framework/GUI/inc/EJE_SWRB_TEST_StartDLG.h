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

#include "DIALOG.h"
#include "stm32f4xx.h"

#include "EJE_SWRB_TEST_DLG_Conf.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/



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

#endif
/*************************** End of file ****************************/
