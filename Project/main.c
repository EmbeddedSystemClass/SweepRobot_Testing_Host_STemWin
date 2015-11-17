#include "includes.h"

#include "sweeprobot_testing.h"

int main(void)
{
#ifdef _DEBUG_LOG
    u32 pmf;
    u8 *str1;
    s8 putsErr = 0;
    s8 putErr = 0;
    s8 makeDir = 0;
    u8 writebuf[10]= {1,2,3,4,5,6,7,8,9,0};
    u8 writeErr = 0;
    u32 rtrnvalue,rtrnvalue1, totaldsk=0,freedsk=0;
    u8 filetype1;
#endif
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
    My_RTC_Init();
	TFTLCD_Init();
	W25QXX_Init();
	LED_Init();
	KEY_Init();
    SD_Init();
    exfans_init();
    TIM3_Int_Init(9999,168-1);
	FSMC_SRAM_Init();

	mem_init(SRAMIN);
	mem_init(SRAMEX);
	mem_init(SRAMCCM);
    
	f_mount(fs[0],"0:",1);
	f_mount(fs[1],"1:",1);

#ifdef _DEBUG_LOG

    rtrnvalue = exf_getfree("0", &totaldsk, &freedsk);
    
    printf("err_code:%d\r\n",rtrnvalue);
    printf("0:total:%d,free:%d\r\n",totaldsk, freedsk);
    
    exf_getfree("1", &totaldsk, &freedsk);
    printf("1:total:%d,free:%d\r\n",totaldsk, freedsk);
    
    mf_scan_files("0:");
    mf_scan_files("1:");
    mf_scan_files("1:SYSTEM");
    mf_scan_files("1:/SYSTEM/APP");
    mf_scan_files("1:/SYSTEM/SPB");

    rtrnvalue1 = mf_open("0:/testroot.txt",FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
    
    printf("open_err_code:%d\r\n",rtrnvalue1);
    
    mf_read(512);
    putErr = mf_putc('5');
    printf("putErr:%d\r\n",putErr);
    str1 = "inserted data";
    putsErr = mf_puts(str1);
    printf("putsErr:%d\r\n",putsErr);
    makeDir = mf_mkdir("0:/test");
    printf("mkDirErr:%d\r\n",makeDir);
    
    mf_close();
    
#endif
    
	OSInit();
	OS_Task_Create();
	OSStart();

  /* XXX: should never get here */
	while(1){

	}

	return -1;
}
