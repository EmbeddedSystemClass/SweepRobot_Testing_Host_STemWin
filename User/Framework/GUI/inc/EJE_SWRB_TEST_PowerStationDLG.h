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
