#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;

    if(mode) key_up=1;
    if(key_up&&(KEY0==0||WK_UP==1||KEY_TEST_START==0||KEY_TEST_STOP==0)){
        delay_ms(10);
        key_up=0;
        if(KEY0==0) return 1;
        else if(WK_UP==1) return 4;
        else if(KEY_TEST_START==0) return 5;
        else if(KEY_TEST_STOP==0) return 6;
    }else if(KEY0==1&&WK_UP==0&&KEY_TEST_START==1&&KEY_TEST_STOP==1)key_up=1;
    return 0;
}

