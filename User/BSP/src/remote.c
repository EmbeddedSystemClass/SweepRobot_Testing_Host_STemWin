#include "remote.h"
#include "delay.h"
#include "usart.h"

void Remote_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM1_ICInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);

    TIM_TimeBaseStructure.TIM_Prescaler=167;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period=10000;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM1_ICInitStructure.TIM_ICFilter = 0x03;
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);

    TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);
    TIM_Cmd(TIM1,ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

u8     RmtSta=0;
u16 Dval;
u32 RmtRec=0;
u8  RmtCnt=0;

void TIM1_UP_TIM10_IRQHandler(void)
{

  if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
    {
        if(RmtSta&0x80)
        {
            RmtSta&=~0X10;
            if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;
            if((RmtSta&0X0F)<14)RmtSta++;
            else
            {
                RmtSta&=~(1<<7);
                RmtSta&=0XF0;
            }
        }
    }
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}

void TIM1_CC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1,TIM_IT_CC1)==SET)
    {
        if(RDATA)
        {
            TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);
            TIM_SetCounter(TIM1,0);
            RmtSta|=0X10;
        }else
        {
            Dval=TIM_GetCapture1(TIM1);
            TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);
            if(RmtSta&0X10)
            {
                 if(RmtSta&0X80)
                {
                    if(Dval>300&&Dval<800)
                    {
                        RmtRec<<=1;
                        RmtRec|=0;
                    }else if(Dval>1400&&Dval<1800)
                    {
                        RmtRec<<=1;
                        RmtRec|=1;
                    }else if(Dval>2200&&Dval<2600)
                    {
                        RmtCnt++;
                        RmtSta&=0XF0;
                    }
                 }else if(Dval>4200&&Dval<4700)
                {
                    RmtSta|=1<<7;
                    RmtCnt=0;
                }
            }
            RmtSta&=~(1<<4);
        }
    }
    TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
}

u8 Remote_Scan(void)
{
    u8 sta=0;
    u8 t1,t2;
    if(RmtSta&(1<<6))
    {
        t1=RmtRec>>24;
        t2=(RmtRec>>16)&0xff;
         if((t1==(u8)~t2)&&t1==REMOTE_ID)
        {
            t1=RmtRec>>8;
            t2=RmtRec;
            if(t1==(u8)~t2)sta=t1;
        }
        if((sta==0)||((RmtSta&0X80)==0))
        {
             RmtSta&=~(1<<6);
            RmtCnt=0;
        }
    }
    return sta;
}
