/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
   
typedef void (*stm32f4xx_int_cb_t)(void);

enum stm32f4xx_int_type {

    STM32F4xx_INT_NMI,
    STM32F4xx_INT_HARDFAULT,
    STM32F4xx_INT_MEMMANAGE,
    STM32F4xx_INT_BUSFAULT,
    STM32F4xx_INT_USAEAGEFAULT,
    STM32F4xx_INT_SVC,
    STM32F4xx_INT_DEBUGMON,
    STM32F4xx_INT_PENDSV,
    STM32F4xx_INT_SYSTICK,

    STM32F4xx_INT_WWDG,
    STM32F4xx_INT_PVD,
    STM32F4xx_INT_TAMPER,
    STM32F4xx_INT_RTC_SEC,
    STM32F4xx_INT_RTC_ALRA,
    STM32F4xx_INT_RTC_ALRB,
    STM32F4xx_INT_FLASH,
    STM32F4xx_INT_RCC,
    STM32F4xx_INT_EXTI0,
    STM32F4xx_INT_EXTI1,
    STM32F4xx_INT_EXTI2,
    STM32F4xx_INT_EXTI3,
    STM32F4xx_INT_EXTI4,
    STM32F4xx_INT_DMA1_Channel1,
    STM32F4xx_INT_DMA1_Channel2,
    STM32F4xx_INT_DMA1_Channel3,
    STM32F4xx_INT_DMA1_Channel4,
    STM32F4xx_INT_DMA1_Channel5,
    STM32F4xx_INT_DMA1_Channel6,
    STM32F4xx_INT_ADC,

#ifdef STM32F40_41xxx
    STM32F4xx_INT_CAN1_TX,
    STM32F4xx_INT_CAN1_RX0,
    STM32F4xx_INT_CAN1_RX1,
    STM32F4xx_INT_CAN1_SCE,
    STM32F4xx_INT_EXTI9_5_5,
    STM32F4xx_INT_EXTI9_5_6,
    STM32F4xx_INT_EXTI9_5_7,
    STM32F4xx_INT_EXTI9_5_8,
    STM32F4xx_INT_EXTI9_5_9,
    STM32F4xx_INT_TIM1_BRK_TIM9,
    STM32F4xx_INT_TIM1_UP_TIM10,
    STM32F4xx_INT_TIM1_TRG_COM_TIM11,
    STM32F4xx_INT_TIM1_CC,
    STM32F4xx_INT_TIM2,
    STM32F4xx_INT_TIM3,
    STM32F4xx_INT_TIM4,
    STM32F4xx_INT_I2C1_EV,
    STM32F4xx_INT_I2C1_ER,
    STM32F4xx_INT_I2C2_EV,
    STM32F4xx_INT_I2C2_ER,
    STM32F4xx_INT_SPI1,
    STM32F4xx_INT_SPI2,
    STM32F4xx_INT_USART1,
    STM32F4xx_INT_USART2,
    STM32F4xx_INT_USART3,
    STM32F4xx_INT_EXTI15_10_10,
    STM32F4xx_INT_EXTI15_10_11,
    STM32F4xx_INT_EXTI15_10_12,
    STM32F4xx_INT_EXTI15_10_13,
    STM32F4xx_INT_EXTI15_10_14,
    STM32F4xx_INT_EXTI15_10_15,
    STM32F4xx_INT_RTCAlarm,
    STM32F4xx_INT_OTG_FS_WKUP,
    STM32F4xx_INT_TIM8_BRK_TIM12,
    STM32F4xx_INT_TIM8_UP_TIM13,
    STM32F4xx_INT_TIM8_TRG_COM_TIM14,
    STM32F4xx_INT_TIM8_CC,
    STM32F4xx_INT_DMA1_Stream7,
    STM32F4xx_INT_FSMC,
    STM32F4xx_INT_SDIO,
    STM32F4xx_INT_TIM5,
    STM32F4xx_INT_SPI3,
    STM32F4xx_INT_UART4,
    STM32F4xx_INT_UART5,
    STM32F4xx_INT_TIM6_DAC,
    STM32F4xx_INT_TIM7,
    STM32F4xx_INT_DMA2_Stream0,
    STM32F4xx_INT_DMA2_Stream1,
    STM32F4xx_INT_DMA2_Stream2,
    STM32F4xx_INT_DMA2_Stream3,
    STM32F4xx_INT_ETH,
    STM32F4xx_INT_ETH_WKUP,
    STM32F4xx_INT_CAN2_TX,
    STM32F4xx_INT_CAN2_RX0,
    STM32F4xx_INT_CAN2_RX1,
    STM32F4xx_INT_CAN2_SCE,
    STM32F4xx_INT_OTG_FS,
    STM32F4xx_INT_DMA2_Stream5,
    STM32F4xx_INT_DMA2_Stream6,
    STM32F4xx_INT_DMA2_Stream7,
    STM32F4xx_INT_USART6,
    STM32F4xx_INT_I2C3_EV,
    STM32F4xx_INT_I2C3_ER,
    STM32F4xx_INT_OTG_HS_EP1_OUT,
    STM32F4xx_INT_OTG_HS_EP1_IN,
    STM32F4xx_INT_OTG_HS_WKUP,
    STM32F4xx_INT_OTG_HS,
    STM32F4xx_INT_DCMI,
    STM32F4xx_INT_CRYP,
    STM32F4xx_INT_HASH_RNG,
    STM32F4xx_INT_FPU,
#endif /* STM32F40_41xxx */
    STM32F4xx_INT_BOUND
};

s8 plat_int_reg_cb(enum stm32f4xx_int_type type, void *cb);
s8 plat_int_dereg_cb(enum stm32f4xx_int_type type);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
