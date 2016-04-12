#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#define KEY0            PEin(4)     //PE4
#define KEY1            PEin(3)     //PE3
#define KEY2            PEin(2)     //PE2
#define WK_UP           PAin(0)     //PA0
#define KEY_TEST_START  PBin(3)     //PB3
#define KEY_TEST_STOP   PBin(4)     //PB4

#define KEY0_PRES               1
#define KEY1_PRES               2
#define KEY2_PRES               3
#define WKUP_PRES               4
#define KEY_TEST_START_PRES     5
#define KEY_TEST_STOP_PRES      6

void KEY_Init(void);  //IO初始化
u8 KEY_Scan(u8);    //按键扫描函数
#endif 
