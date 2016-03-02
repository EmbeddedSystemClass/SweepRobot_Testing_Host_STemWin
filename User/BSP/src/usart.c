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

static FunctionalState gStdUartState = DISABLE;

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
    if(gStdUartState){
        while((STD_USART->SR&0X40)==0);
        STD_USART->DR = (u8) ch;
        return ch;
    }else{
        return 0;
    }
}
#endif

static void STD_USART_ISR(void);

char USART_RX_BUF[USART_RX_LEN];
u16 USART_RX_STA=0;

void STD_UART_Init(u32 bound){

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  STD_USART_GPIO_RCC_ENABLE();
  STD_USART_RCC_ENABLE();

  GPIO_PinAFConfig(STD_USART_GPIO, STD_USART_RX_PIN_SOURCE, STD_USART_AF_PPP);
  GPIO_PinAFConfig(STD_USART_GPIO, STD_USART_TX_PIN_SOURCE, STD_USART_AF_PPP);

  GPIO_InitStructure.GPIO_Pin = STD_USART_RX_PIN | STD_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(STD_USART_GPIO,&GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(STD_USART, &USART_InitStructure);

  USART_Cmd(STD_USART, ENABLE);

  USART_ClearFlag(STD_USART, USART_FLAG_TC);

#if EN_USART_RX
  USART_ITConfig(STD_USART, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = STD_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  plat_int_reg_cb(STD_USART_INT, STD_USART_ISR);
#endif
}

void STD_USART_ISR(void)
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

void STD_UART_DISABLE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(gStdUartState){
        USART_Cmd(STD_USART, DISABLE);

        GPIO_InitStructure.GPIO_Pin = STD_USART_RX_PIN | STD_USART_TX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(STD_USART_GPIO,&GPIO_InitStructure);

        STD_USART_RCC_DISABLE();

        gStdUartState = DISABLE;
    }
}

void STD_UART_ENABLE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(!gStdUartState){
        STD_USART_RCC_ENABLE();

        GPIO_InitStructure.GPIO_Pin = STD_USART_RX_PIN | STD_USART_TX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(STD_USART_GPIO,&GPIO_InitStructure);

        USART_Cmd(STD_USART, ENABLE);

        gStdUartState = ENABLE;
    }
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

