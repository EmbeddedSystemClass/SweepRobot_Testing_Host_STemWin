#ifndef __SWEEPROBOT_TESTING_H
#define __SWEEPROBOT_TESTING_H

#include "includes.h"

#include "delay.h"
#include "usart.h"
#include "malloc.h"

#include "ff.h"
#include "exfans.h"
#include "w25qxx.h"
#include "fattester.h"

#include "sram.h"
#include "RTC.h"
#include "ILI93xx.h"
#include "timer.h"
#include "touch.h"
#include "led.h"
#include "key.h"
#include "sdio_SDcard.h"

#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"

#include "EJE_SweepRobot_test_SystemDLG.h"

enum SWRB_Test_State_Pos{

  SWRB_TEST_WHEEL_L_STATE_POS,
  SWRB_TEST_WHEEL_R_STATE_POS,
  SWRB_TEST_BRUSH_L_STATE_POS,
  SWRB_TEST_BRUSH_R_STATE_POS,

  SWRB_TEST_BRUSH_M_STATE_POS,
  SWRB_TEST_FAN_POS,
  SWRB_TEST_IFRD_F_L_POS,
  SWRB_TEST_IFRD_F_R_POS,

  SWRB_TEST_IFRD_S_L_POS,
  SWRB_TEST_IFRD_S_R_POS,
  SWRB_TEST_IFRD_B_F_L_POS,
  SWRB_TEST_IFRD_B_F_R_POS,

  SWRB_TEST_IFRD_B_S_L_POS,
  SWRB_TEST_IFRD_B_S_R_POS,
  SWRB_TEST_COLLISION_L_POS,
  SWRB_TEST_COLLISION_F_L_POS,

  SWRB_TEST_COLLISION_F_R_POS,
  SWRB_TEST_COLLISION_R_POS,
  SWRB_TEST_WHEEL_FLOAT_L_POS,
  SWRB_TEST_WHEEL_FLOAT_R_POS,

  SWRB_TEST_ASH_TRAY_INS_POS,
  SWRB_TEST_ASH_TRAY_LVL_POS,
  SWRB_TEST_UNIWHEEL_POS,
  SWRB_TEST_KEY_POS,

  SWRB_TEST_IRDA_B_POS,
  SWRB_TEST_IRDA_L_POS,
  SWRB_TEST_IRDA_FL_POS,
  SWRB_TEST_IRDA_FR_POS,
  
  SWRB_TEST_IRDA_R_POS,
  SWRB_TEST_BUZZER_POS,
  SWRB_TEST_RGB_LED_POS,

  SWRB_TEST_STATE_POS_BOUND,
};

enum SWRB_TEST_STATE{

  SWRB_TEST_STATE_WHEEL = 1,
  SWRB_TEST_STATE_BRUSH,
//  SWRB_TEST_STATE_SBRUSH,
//  SWRB_TEST_STATE_MBRUSH,
  SWRB_TEST_STATE_FAN,
  SWRB_TEST_STATE_IFRD,
  SWRB_TEST_STATE_COLLISION,
  SWRB_TEST_STATE_WHEEL_FLOAT,
  SWRB_TEST_STATE_ASH_TRAY,
  SWRB_TEST_STATE_UNIWHEEL,
  SWRB_TEST_STATE_KEY,
  SWRB_TEST_STATE_IRDA,
  SWRB_TEST_STATE_BUZZER,
  SWRB_TEST_STATE_RGB_LED,

  SWRB_TEST_STATE_BOUND,
};

enum SWRB_TEST_MODE{
    SWRB_TEST_MODE_IDLE,
    SWRB_TEST_MODE_PAUSE,
    SWRB_TEST_MODE_RUN,
};

#define SWRB_TEST_FAULT_WHEEL_MASK            0x00000003
#define SWRB_TEST_FAULT_WHEEL_L_MASK          0x00000001
#define SWRB_TEST_FAULT_WHEEL_R_MASK          0x00000002

#define SWRB_TEST_FAULT_BRUSH_MASK            0x0000001C
#define SWRB_TEST_FAULT_BRUSH_L_MASK          0x00000004
#define SWRB_TEST_FAULT_BRUSH_R_MASK          0x00000008
#define SWRB_TEST_FAULT_BRUSH_M_MASK          0x00000010

#define SWRB_TEST_FAULT_FAN_MASK              0x00000020

#define SWRB_TEST_FAULT_IFRD_MASK             0x00003FC0
#define SWRB_TEST_FAULT_IFRD_F_L_MASK         0x00000040
#define SWRB_TEST_FAULT_IFRD_F_R_MASK         0x00000080
#define SWRB_TEST_FAULT_IFRD_S_L_MASK         0x00000100
#define SWRB_TEST_FAULT_IFRD_S_R_MASK         0x00000200
#define SWRB_TEST_FAULT_IFRD_B_FL_MASK        0x00000400
#define SWRB_TEST_FAULT_IFRD_B_FR_MASK        0x00000800
#define SWRB_TEST_FAULT_IFRD_B_SL_MASK        0x00001000
#define SWRB_TEST_FAULT_IFRD_B_SR_MASK        0x00002000

#define SWRB_TEST_FAULT_COLLISION_MASK        0x0003C000
#define SWRB_TEST_FAULT_COLLISION_L_MASK      0x00004000
#define SWRB_TEST_FAULT_COLLISION_F_L_MASK    0x00008000
#define SWRB_TEST_FAULT_COLLISION_F_R_MASK    0x00010000
#define SWRB_TEST_FAULT_COLLISION_R_MASK      0x00020000

#define SWRB_TEST_FAULT_WHEEL_FLOAT_MASK      0x000C0000
#define SWRB_TEST_FAULT_WHEEL_FLOAT_L_MASK    0x00040000
#define SWRB_TEST_FAULT_WHEEL_FLOAT_R_MASK    0x00080000

#define SWRB_TEST_FAULT_ASH_TRAY_MASK         0x00300000
#define SWRB_TEST_FAULT_ASH_TRAY_INS_MASK     0x00100000
#define SWRB_TEST_FAULT_ASH_TRAY_LVL_MASK     0x00200000

#define SWRB_TEST_FAULT_UNIWHEEL_MASK         0x00400000

#define SWRB_TEST_FAULT_KEY_MASK              0x00800000

#define SWRB_TEST_FAULT_IRDA_MASK             0x1F000000
#define SWRB_TEST_FAULT_IRDA_B_MSAK           0x01000000
#define SWRB_TEST_FAULT_IRDA_L_MSAK           0x02000000
#define SWRB_TEST_FAULT_IRDA_FL_MSAK          0x04000000
#define SWRB_TEST_FAULT_IRDA_FR_MSAK          0x08000000
#define SWRB_TEST_FAULT_IRDA_R_MSAK           0x10000000

#define SWRB_TEST_FAULT_BUZZER_MASK           0x20000000

#define SWRB_TEST_FAULT_RGB_LED_MASK          0x40000000

enum SWRB_TEST_DATA_POS{
    SWRB_TEST_DATA_DUT_SN_POS,
    SWRB_TEST_DATA_WHEEL_L_SPEED_POS,
    SWRB_TEST_DATA_WHEEL_R_SPEED_POS,
    
};

enum SWRB_TEST_TASK_PRIO{

  SWRB_TEST_CTRL_TASK_PRIO=3,
  SWRB_TEST_START_TASK_BOUND,
  SWRB_WHEEL_TEST_TASK_PRIO,
  SWRB_BRUSH_TEST_TASK_PRIO,
  SWRB_FAN_TEST_TASK_PRIO,
  SWRB_IFRD_TEST_TASK_PRIO,
  SWRB_COLLISION_TEST_TASK_PRIO,
  SWRB_WHEEL_FLOAT_TEST_TASK_PRIO,
  SWRB_ASH_TRAY_TEST_TASK_PRIO,
  SWRB_UNIWHEEL_TEST_TASK_PRIO,
  SWRB_KEY_TEST_TASK_PRIO,
  SWRB_IRDA_TEST_TASK_PRIO,
  SWRB_BUZZER_TEST_TASK_PRIO,
  SWRB_RGB_LED_TEST_TASK_PRIO,
  SWRB_CHARGE_TEST_TASK_PRIO,
  SWRB_TEST_TASK_PRIO_BOUND,
  TOUCH_TASK_PRIO,
  KEY_TASK_PRIO,
  USART_TASK_PRIO,
  EMWINDEMO_TASK_PRIO,
  LED_TASK_PRIO,
  START_TASK_PRIO,
  SAVE_DATA_TASK_PRIO,
};

#define START_STK_SIZE                      128
#define TOUCH_STK_SIZE                      128
#define KEY_STK_SIZE                        256
#define USART_STK_SIZE                      256
#define EMWINDEMO_STK_SIZE		            2048
#define LED_STK_SIZE				        128
#define SAVE_DATA_STK_SIZE                  256
#define SWRB_TEST_CTRL_STK_SIZE             256
#define SWRB_WHEEL_TEST_STK_SIZE            256
#define SWRB_BRUSH_TEST_STK_SIZE            256
#define SWRB_FAN_TEST_STK_SIZE              256
#define SWRB_IFRD_TEST_STK_SIZE             256
#define SWRB_COLLISION_TEST_STK_SIZE        256
#define SWRB_WHEEL_FLOAT_TEST_STK_SIZE      256
#define SWRB_ASH_TRAY_TEST_STK_SIZE         256
#define SWRB_UNIWHEEL_TEST_STK_SIZE         256
#define SWRB_KEY_TEST_STK_SIZE              256
#define SWRB_IRDA_TEST_STK_SIZE             256
#define SWRB_BUZZER_TEST_STK_SIZE           256
#define SWRB_RGB_LED_TEST_STK_SIZE          256
#define SWRB_CHARGE_TEST_STK_SIZE           256

#define KEYMSG_Q_NUM	                    1
#define DATAMSG_Q_NUM	                    4

#define SWRB_TEST_USART_READ_TIMES          5

extern u8 usartRxFlag;
extern int usartRxNum;

extern u8 gSwrbTestMode;
extern u8 gSwrbTestRuningTaskPrio;
extern u32 gSwrbTestStateMap;
extern u32 lastSwrbTestStateMap;
extern u32 gSwrbTestDUTSerialNum;
extern u16 gSwrbTestAcquiredData[];
extern u16 gSwrbTestTaskCnt;

void OS_Task_Create(void);

void SweepRobot_TestStartProc(void);
void SweepRobot_TestPauseProc(void);
void SweepRobot_TestSetProc(void);
void SweepRobot_TestStopProc(void);
void SweepRobot_TestExitProc(void);

#endif
