#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_RX_LEN                200
#define EN_USART_RX                 1

#define STD_USART                   USART3

#define USART_CNT_MASK              0x3FFF
          
extern char USART_RX_BUF[USART_RX_LEN];
extern u16 USART_RX_STA;

void STD_UART_Init(u32 bound);
void STD_UART_DISABLE(void);
void STD_UART_ENABLE(void);
s8 USART_RxArrayToString(char *src_array, char *dest_str);
s8 USART_RxArrayToNumber(char *src_array, int *dest_num, int arrayLenth);
char* USART_NumberToString(int *src_num, char *dest_str);

#endif


