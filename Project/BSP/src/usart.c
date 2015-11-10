#include "sys.h"
#include "usart.h"	
#include "malloc.h"
#include "stm32f4xx_it.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x;
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

static void USART1_ISR(void);
static void USART3_ISR(void);
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 usartRxLen = 0;
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

#define USART_CNT_MASK    0x3F

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //串口1对应引脚复用映射
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  //USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

   //USART1 初始化设置
  USART_InitStructure.USART_BaudRate = bound;//波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1

  USART_Cmd(USART1, ENABLE);  //使能串口1 

  USART_ClearFlag(USART1, USART_FLAG_TC);

  USART_InitStructure.USART_BaudRate = bound;//波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);

  USART_Cmd(USART3, ENABLE);

  USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART1_RX	
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

  plat_int_reg_cb(STM32F4xx_INT_USART1, USART1_ISR);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

  plat_int_reg_cb(STM32F4xx_INT_USART3, USART3_ISR);
#endif
}

void USART1_ISR(void)
{
	u8 Res;
#if SYSTEM_SUPPORT_UCOS
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		Res =USART_ReceiveData(USART1);
		
		if((USART_RX_STA&0x8000)==0){
			if(USART_RX_STA&0x4000){
				if(Res!=0x0a)
					USART_RX_STA=0;
				else{
					USART_RX_STA|=0x8000;
          usartRxLen = USART_RX_STA&USART_CNT_MASK;
				}
			}
			else{
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;
				}		 
			}
		}   		 
  } 
	
#if SYSTEM_SUPPORT_UCOS  
	OSIntExit();
#endif
}

void USART3_ISR(void)
{
  
}

s8 USART_RxArrayToString(char *src_array, char* *dest_str)
{
  *dest_str = (char *)mymalloc(SRAMEX ,(sizeof(char))*((USART_RX_STA&USART_CNT_MASK)+1));
//    *dest_str = (char *)mymalloc(SRAMEX ,(sizeof(char))*(usartRxLen+1) );
//    *dest_str = (char *)mymalloc(SRAMEX ,(sizeof(char))*3 );
  if(NULL==(*dest_str) )
    return -1;
    
  mymemset(*dest_str, 0, (sizeof(char))*((USART_RX_STA&USART_CNT_MASK)+1));
  strncpy(*dest_str, src_array, (USART_RX_STA&USART_CNT_MASK) );
  
  return 0;
}

s8 USART_RxArrayToNumber(char *src_array, int *dest_num)
{
  u8 i;
  
  *dest_num = 0;
  
  for(i=0;i<(USART_RX_STA&USART_CNT_MASK);i++){
    if( ('0' <= (src_array[i])) && ('9' >= (src_array[i]) ) ){
      if(i){
        *dest_num *= 10;
      }
      *dest_num += (src_array[i] - '0');
    }else{
      return -1;
    }
  }
  
  return 0;
}

char* USART_NumberToString(int *src_num, char *dest_str)
{
  sprintf(dest_str, "%d", *src_num); 
  return dest_str;
}


#endif	

