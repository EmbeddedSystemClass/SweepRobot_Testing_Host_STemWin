#include "rtc.h"

#include "sweeprobot_testing.h"

NVIC_InitTypeDef   NVIC_InitStructure;

ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;

    RTC_TimeTypeInitStructure.RTC_Hours=hour;
    RTC_TimeTypeInitStructure.RTC_Minutes=min;
    RTC_TimeTypeInitStructure.RTC_Seconds=sec;
    RTC_TimeTypeInitStructure.RTC_H12=ampm;

    return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
}

ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
    RTC_DateTypeDef RTC_DateTypeInitStructure;
    RTC_DateTypeInitStructure.RTC_Date=date;
    RTC_DateTypeInitStructure.RTC_Month=month;
    RTC_DateTypeInitStructure.RTC_WeekDay=week;
    RTC_DateTypeInitStructure.RTC_Year=year;
    return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

void RTC_TIME_Disp(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    char *str;
    
    str = mymalloc(SRAMIN, sizeof(char)*30);
    sprintf(str,"%d/%d/%d %d:%d:%d", date->RTC_Year, date->RTC_Month, date->RTC_Date, time->RTC_Hours, time->RTC_Minutes, time->RTC_Seconds);
    if(gSwrbTestMode == SWRB_TEST_MODE_SET){
        Edit_Set_Text(hWin_SWRB_TIMESETTING, ID_TIMESET_EDIT_ACTVALUE, str);
    }else{
        Edit_Set_Text(hWin_SWRB_MAIN, ID_MAIN_EDIT_DATE, str);
    }
    myfree(SRAMIN, str);
}

u8 My_RTC_Init(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    u16 retry=0X1FFF;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050){
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){
            retry++;
            delay_ms(10);
        }
        if(retry==0)return 1;

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);

        RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
        RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
        RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
        RTC_Init(&RTC_InitStructure);

        RTC_Set_Time(6,0,0,RTC_H12_PM);
        RTC_Set_Date(15,12,03,49);

        RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);
    }

    return 0;
}

void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;

    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A

    RTC_TimeTypeInitStructure.RTC_Hours=hour;//Сʱ
    RTC_TimeTypeInitStructure.RTC_Minutes=min;//����
    RTC_TimeTypeInitStructure.RTC_Seconds=sec;//��
    RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;

    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//����
    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//��������
    RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//��ȷƥ�����ڣ�ʱ����
    RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
    RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);


    RTC_ClearITPendingBit(RTC_IT_ALRA);//���RTC����A�ı�־
    EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ

    RTC_ITConfig(RTC_IT_ALRA,ENABLE);//��������A�ж�
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//��������A

    EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE17
    EXTI_Init(&EXTI_InitStructure);//����

    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
}

//�����Ի��Ѷ�ʱ������
/*wksel:  @ref RTC_Wakeup_Timer_Definitions
#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
*/
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{
    EXTI_InitTypeDef   EXTI_InitStructure;

    RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP

    RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��

    RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���

    RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
    EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ

    RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
    RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����

    EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
    EXTI_Init(&EXTI_InitStructure);//����


    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
}

void RTC_Alarm_IRQHandler(void)
{
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET){
        RTC_ClearFlag(RTC_FLAG_ALRAF);
        printf("ALARM A!\r\n");
    }
    EXTI_ClearITPendingBit(EXTI_Line17);
}

void RTC_WKUP_IRQHandler(void)
{
    if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET){
        RTC_ClearFlag(RTC_FLAG_WUTF);
        LED1=!LED1;
    }
    EXTI_ClearITPendingBit(EXTI_Line22);
}
