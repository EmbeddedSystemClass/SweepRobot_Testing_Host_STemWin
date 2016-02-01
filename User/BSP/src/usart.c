#include "sys.h"
#include "usart.h"
#include "malloc.h"
#include "stm32f4xx_it.h"

#include "sweeprobot_testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif

#if 1
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

FILE __stdout;

int _sys_exit(int x)
{
    x = x;
    return -1;
}

int fputc(int ch, FILE *f)
{
    while((STD_USART->SR&0X40)==0);
    STD_USART->DR = (u8) ch;
    return ch;
}
#endif

static void USART1_ISR(void);
static void USART3_ISR(void);

char USART_RX_BUF[USART_RX_LEN];
u16 USART_RX_STA=0;

void UART_Init(u32 bound){

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

//  USART_InitStructure.USART_BaudRate = bound;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  USART_Init(USART1, &USART_InitStructure);

//  USART_Cmd(USART1, ENABLE);

//  USART_ClearFlag(USART1, USART_FLAG_TC);

  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

  USART_Cmd(USART3, ENABLE);

  USART_ClearFlag(USART3, USART_FLAG_TC);

#if EN_USART_RX
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

  plat_int_reg_cb(STM32F4xx_INT_USART1, USART1_ISR);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  plat_int_reg_cb(STM32F4xx_INT_USART3, USART3_ISR);
#endif
}

void USART1_ISR(void)
{
    u8 rxValue;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    if(USART_GetITStatus(STD_USART, USART_IT_RXNE) != RESET){
        rxValue =USART_ReceiveData(STD_USART);

        if((USART_RX_STA&0x8000)==0){
            if(USART_RX_STA&0x4000){
                if(rxValue!='\n')
                    USART_RX_STA=0;
                else{
                    USART_RX_STA|=0x8000;
                    if( (gSwrbTestRuningTaskPrio != SWRB_MANUL_TEST_TASK_PRIO) && (gSwrbTestRuningTaskPrio != SWRB_IRDA_TEST_TASK_PRIO) ){
                        USART_RxArrayToNumber(USART_RX_BUF, &usartRxNum, USART_RX_STA&USART_CNT_MASK);
//                        USART_RX_STA = 0;
                    }
                    usartRxFlag = 1;
                    if(gSwrbTestRuningTaskPrio != NULL){
                        OSTimeDlyResume(gSwrbTestRuningTaskPrio);
                    }
                }
            }else{
                if(rxValue=='\r')
                    USART_RX_STA|=0x4000;
                else{
                    USART_RX_BUF[USART_RX_STA&USART_CNT_MASK]=rxValue;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_RX_LEN-1))
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
    u8 rxValue;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    if(USART_GetITStatus(STD_USART, USART_IT_RXNE) != RESET){
        rxValue =USART_ReceiveData(STD_USART);

        if((USART_RX_STA&0x8000)==0){
            if(USART_RX_STA&0x4000){
                if(rxValue!='\n')
                    USART_RX_STA=0;
                else{
                    USART_RX_STA|=0x8000;
                    if( (gSwrbTestRuningTaskPrio != SWRB_MANUL_TEST_TASK_PRIO) && (gSwrbTestRuningTaskPrio != SWRB_IRDA_TEST_TASK_PRIO) ){
                        USART_RxArrayToNumber(USART_RX_BUF, &usartRxNum, USART_RX_STA&USART_CNT_MASK);
//                        USART_RX_STA = 0;
                    }
                    usartRxFlag = 1;
                    if(gSwrbTestRuningTaskPrio != NULL){
                        OSTimeDlyResume(gSwrbTestRuningTaskPrio);
                    }
                }
            }else{
                if(rxValue=='\r')
                    USART_RX_STA|=0x4000;
                else{
                    USART_RX_BUF[USART_RX_STA&USART_CNT_MASK]=rxValue;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_RX_LEN-1))
                        USART_RX_STA=0;
                }
            }
        }
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}

s8 USART_RxArrayToString(char *src_array, char *dest_str)
{
  dest_str = (char *)mymalloc(SRAMIN ,(sizeof(char))*((USART_RX_STA&USART_CNT_MASK)+1));
  if(NULL==(*dest_str) )
    return -1;

  mymemset(dest_str, 0, (sizeof(char))*((USART_RX_STA&USART_CNT_MASK)+1));
  strncpy(dest_str, src_array, (USART_RX_STA&USART_CNT_MASK) );

  return 0;
}

s8 USART_RxArrayToNumber(char *src_array, int *dest_num, int arrayLenth)
{
  u8 i;

  *dest_num = 0;

  for(i=0;i<(arrayLenth);i++){
    if( ('0' <= (src_array[i])) && ('9' >= (src_array[i]) ) ){
      if(i){
        *dest_num *= 10;
      }
      *dest_num += (src_array[i] - '0');
    }else if( 0 == src_array[i]){
      continue;
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

