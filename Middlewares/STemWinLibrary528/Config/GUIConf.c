#include "GUI.h"
#include "sram.h"
#include "malloc.h"

#define USE_EXRAM  1

#define GUI_NUMBYTES  (1024*1024*2)
#define GUI_BLOCKSIZE 0X80

void GUI_X_Config(void) {
	if(USE_EXRAM)
	{
		U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES);
		GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES);
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
		GUI_SetDefaultFont(GUI_FONT_6X8);
	}else
	{
		U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES);
		GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES);
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
		GUI_SetDefaultFont(GUI_FONT_6X8);
	}
}
