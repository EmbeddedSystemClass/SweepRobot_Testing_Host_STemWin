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

#ifndef __EJE_SWRB_TEST_PCBTESTDLG_H
#define __EJE_SWRB_TEST_PCBTESTDLG_H

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
*       CreateEJE_SWRB_TEST_PCBTestDLG
*/
extern WM_HWIN CreateEJE_SWRB_TEST_PCBTestDLG(void);
extern WM_HWIN CreateKEY_TestDLG(void);
extern WM_HWIN CreateRGB_LED_TestDLG(void);
extern WM_HWIN CreateBUZZER_TestDLG(void);
extern WM_HWIN CreateWarningDLG(void);

#endif
/*************************** End of file ****************************/
