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

// USER START (Optionally insert additional defines)
// USER END

extern WM_HWIN hWinEJE_SweepRobot_test_System;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

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

// USER START (Optionally insert additional public code)
void Progbar_Set_Value(u8 progbarValue);
void Checkbox_Set_State(u8 checkboxState);
void Edit_Set_Text(char *s);
// USER END

/*************************** End of file ****************************/
