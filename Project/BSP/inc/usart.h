#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  		200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		  //ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

s8 USART_RxArrayToString(char *src_array, char* *dest_str);
s8 USART_RxArrayToNumber(char *src_array, int *dest_num);
char* USART_NumberToString(int *src_num, char *dest_str);
#endif


