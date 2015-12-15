#ifndef __RED_H
#define __RED_H 

#include "sys.h"   
  
#define RDATA PAin(8)

#define REMOTE_ID 0      		   

extern u8 RmtCnt;

void Remote_Init(void);
u8 Remote_Scan(void);	    
#endif

