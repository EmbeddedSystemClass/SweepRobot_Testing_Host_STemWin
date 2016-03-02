#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_RX_LEN                100
#define EN_USART_RX                 1

#ifdef USE_USART1
#define STD_USART                   USART1
#define STD_USART_IRQn              USART1_IRQn
#define STD_USART_GPIO              GPIOA
#define STD_USART_RX_PIN            GPIO_Pin_10
#define STD_USART_TX_PIN            GPIO_Pin_9
#define STD_USART_RX_PIN_SOURCE     GPIO_PinSource10
#define STD_USART_TX_PIN_SOURCE     GPIO_PinSource9
#define STD_USART_AF_PPP            GPIO_AF_USART1
#define STD_USART_INT               STM32F4xx_INT_USART1
#define STD_USART_GPIO_PERIPH_ID    RCC_AHB1Periph_GPIOA
#define STD_USART_PERIPH_ID         RCC_APB2Periph_USART1
#define STD_USART_GPIO_RCC_ENABLE() RCC_AHB1PeriphClockCmd(STD_USART_GPIO_PERIPH_ID, ENABLE)
#define STD_USART_RCC_ENABLE()      RCC_APB2PeriphClockCmd(STD_USART_PERIPH_ID, ENABLE)
#define STD_USART_RCC_DISABLE()     RCC_APB2PeriphClockCmd(STD_USART_PERIPH_ID, DISABLE)
#elif USE_USART2
#define STD_USART                   USART2
#define STD_USART_IRQn              USART2_IRQn
#define STD_USART_GPIO              GPIOA
#define STD_USART_RX_PIN            GPIO_Pin_3
#define STD_USART_TX_PIN            GPIO_Pin_2
#define STD_USART_RX_PIN_SOURCE     GPIO_PinSource3
#define STD_USART_TX_PIN_SOURCE     GPIO_PinSource2
#define STD_USART_AF_PPP            GPIO_AF_USART2
#define STD_USART_INT               STM32F4xx_INT_USART2
#define STD_USART_GPIO_PERIPH_ID    RCC_AHB1Periph_GPIOA
#define STD_USART_PERIPH_ID         RCC_APB1Periph_USART2
#define STD_USART_GPIO_RCC_ENABLE() RCC_AHB1PeriphClockCmd(STD_USART_GPIO_PERIPH_ID, ENABLE)
#define STD_USART_RCC_ENABLE()      RCC_APB1PeriphClockCmd(STD_USART_PERIPH_ID, ENABLE)
#define STD_USART_RCC_DISABLE()     RCC_APB1PeriphClockCmd(STD_USART_PERIPH_ID, DISABLE)
#elif USE_USART3
#define STD_USART                   USART3
#define STD_USART_IRQn              USART3_IRQn
#define STD_USART_GPIO              GPIOB
#define STD_USART_RX_PIN            GPIO_Pin_11
#define STD_USART_TX_PIN            GPIO_Pin_10
#define STD_USART_RX_PIN_SOURCE     GPIO_PinSource11
#define STD_USART_TX_PIN_SOURCE     GPIO_PinSource10
#define STD_USART_AF_PPP            GPIO_AF_USART3
#define STD_USART_INT               STM32F4xx_INT_USART3
#define STD_USART_GPIO_PERIPH_ID    RCC_AHB1Periph_GPIOB
#define STD_USART_PERIPH_ID         RCC_APB1Periph_USART3
#define STD_USART_GPIO_RCC_ENABLE() RCC_AHB1PeriphClockCmd(STD_USART_GPIO_PERIPH_ID, ENABLE)
#define STD_USART_RCC_ENABLE()      RCC_APB1PeriphClockCmd(STD_USART_PERIPH_ID, ENABLE)
#define STD_USART_RCC_DISABLE()     RCC_APB1PeriphClockCmd(STD_USART_PERIPH_ID, DISABLE)
#endif

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
